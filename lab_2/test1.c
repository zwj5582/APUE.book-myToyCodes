/*************************************************************************
	> File Name: test1.c
	> Author: zhongwenjie
	> Mail: zwj5582@gmail.com 
	> Created Time: Sun 14 Aug 2016 02:04:00 AM PDT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stddef.h>
#include<string.h>
#include<signal.h>
#include<errno.h>
#include<sys/resource.h>
#include<fcntl.h>
#include<pthread.h>
#include<termios.h>
#include<time.h>
#include<sys/time.h>
#include<sys/utsname.h>
#include<pwd.h>
#include<grp.h>
#include<syslog.h>
#include<sys/select.h>
#include<ctype.h>
#include<aio.h>
#include<sys/mman.h>
#include<sys/wait.h>
#include<limits.h>
#include<sys/ioctl.h>
#include<sys/termios.h>
#include<sys/ipc.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<stdint.h>
#include<netdb.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<sys/uio.h>

#define QLEN 10

#define CLI_PATH "/var/tmp/"

#define CLI_PERM S_IRWXU

#define STALE 30

#define NALLOC 10

#define LINESIZE 4096

#define CS_OPEN "/tmp/opend.docket"

typedef struct{
	int fd;
	uid_t uid;
}Client;

Client* client;

int client_size;

void daemonize(const char* cmd){
	int i,fd0,fd1,fd2;
	pid_t pid;
	struct rlimit rl;
	struct sigaction sa;
	umask(0);
	if((getrlimit(RLIMIT_NOFILE,&rl))<0){
		printf("getrlimit error\n");exit(-1);
	}
	if((pid=fork())<0){
		printf("fork error\n");exit(-2);
	}else if(pid>0)
		exit(0);
	setsid();
	sa.sa_handler=SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags=0;
	if((sigaction(SIGHUP,&sa,NULL))<0){
		printf("sigaction error\n");exit(-4);
	}
	if((pid=fork())<0){
		printf("fork error\n");exit(-5);
	}
	if(pid!=0)
		exit(0);
	if(chdir("/")<0){
		printf("chdir error\n");exit(-3);
	}
	if(rl.rlim_max==RLIM_INFINITY)
		rl.rlim_max=1024;
	for(i=0;i<rl.rlim_max;i++)
		close(i);
	fd0=open("/dev/null",O_RDWR);
	fd1=dup(0);
	fd2=dup(0);
	openlog(cmd,LOG_CONS,LOG_DAEMON);
}

static void client_alloc(void){
	int i;
	if(client==NULL)
		client=malloc(NALLOC*sizeof(Client));
	else
		client=realloc(client,(client_size+NALLOC)*sizeof(Client));
	if(client==NULL){
		printf("alloc error\n");exit(-1);
	}
	for(i=client_size;i<client_size+NALLOC;i++)
		client[i].fd=-1;
	client_size+=NALLOC;
}

int client_add(int fd,uid_t uid){
	int i;
	if(client==NULL)
		client_alloc();
AGAIN:
	for(i=0;i<client_size;i++){
		if(client[i].fd==-1){
			client[i].fd=fd;
			client[i].uid=uid;
			return i;
		}
	}
	client_alloc();
	goto AGAIN;
}

void client_del(int fd){
	int i;
	for(i=0;i<client_size;i++){
		if(client[i].fd==fd){
			client[i].fd==-1;
			return;
		}
	}
	printf("can't find client\n");exit(1);
}

int serv_listen(const char* name){
	int err,fd,len,rval;
	struct sockaddr_un un;
	if(strlen(name)>=sizeof(un.sun_path)){
		errno=ENAMETOOLONG;return -1;
	}
	if((fd=socket(AF_UNIX,SOCK_STREAM,0))<0)
		return -2;
	memset(&un,0,sizeof(un));
	un.sun_family=AF_UNIX;
	strcpy(un.sun_path,name);
	len=offsetof(struct sockaddr_un,sun_path)+strlen(name);
	if(bind(fd,(struct sockaddr*)&un,len)<0){
		rval=-3;goto ERROR;
	}
	if(listen(fd,QLEN)<0){
		rval=-4;goto ERROR;
	}
	return fd;
ERROR:
	err=errno;
	close(fd);
	errno=err;
	return rval;
}

int serv_accept(int listenfd,uid_t* uidptr){
	int err,rval,newfd;
	struct sockaddr_un un;
	struct stat statbuf;
	socklen_t len;
	time_t staletime;
	char* name;
	if((name=malloc(sizeof(un.sun_path)+1))==NULL)
		return -1;
	len=sizeof(un);
	if(newfd=accept(listenfd,(struct sockaddr*)&un,&len)<0){
		free(name);return -2;
	}
	len-=offsetof(struct sockaddr_un,sun_path);
	memcpy(name,un.sun_path,len);
	name[len]=0;
	if(stat(name,&statbuf)<0){
		rval=-3;goto ERROR;
	}
	if(S_ISSOCK(statbuf.st_mode)==0){
		rval=-4;goto ERROR;
	}
	if((statbuf.st_mode&(S_IRWXG|S_IRWXO))||(statbuf.st_mode&S_IRWXU)!=S_IRWXU){
		rval=-5;goto ERROR;
	}
	staletime=time(NULL)-STALE;
	if(statbuf.st_ctime<staletime||statbuf.st_mtime<staletime||statbuf.st_atime<staletime){
		rval=-6;goto ERROR;
	}
	if(uidptr!=NULL)
		*uidptr=statbuf.st_uid;
	unlink(name);
	free(name);
	return newfd;
ERROR:
	err=errno;
	close(newfd);
	free(name);
	errno=err;
	return rval;
}

int cli_conn(const char* name){
	int fd,len,err,rval;
	struct sockaddr_un un,sun;
	int dounlink=0;
	if(strlen(name)>=sizeof(un.sun_path)){
		errno=ENAMETOOLONG;return -1;
	}
	if((fd=socket(AF_UNIX,SOCK_STREAM,0))<0)
		return -1;
	memset(&un,0,sizeof(un));
	un.sun_family=AF_UNIX;
	sprintf(un.sun_path,"%s%05ld",CLI_PATH,getpid());
	len=offsetof(struct sockaddr_un,sun_path)+strlen(un.sun_path);
	unlink(un.sun_path);
	if(bind(fd,(struct sockaddr*)&un,len)<0){
		rval=-2;goto ERROR;
	}
	if(chmod(un.sun_path,CLI_PERM)<0){
		rval=-3;dounlink=1;goto ERROR;
	}
	memset(&sun,0,sizeof(sun));
	sun.sun_family=AF_UNIX;
	strcpy(sun.sun_path,name);
	len=offsetof(struct sockaddr_un,sun_path)+strlen(name);
	if(connect(fd,(struct sockaddr*)&sun,len)<0){
		rval=-4;dounlink=1;goto ERROR;
	}
	return fd;
ERROR:
	err=errno;
	close(fd);
	if(dounlink)
		unlink(un.sun_path);
	errno=err;
	return rval;
}

void loop(void){
	int nread,newfd,listenfd,maxfd,maxi,i;
	char buf[LINESIZE];
	uid_t uid;
	fd_set rset,allset;
	FD_ZERO(&allset);
	if((listenfd=serv_listen(CS_OPEN))<0){
		printf("serv_listen error\n");exit(-1);
	}
	FD_SET(listenfd,&allset);
	maxfd=listenfd;
	maxi=-1;
	while(1){
		rset=allset;
		if(select(maxfd+1,&rset,NULL,NULL,NULL)<0){
			printf("select error\n");exit(-2);
		}
		if(FD_ISSET(listenfd,&rset)){
			if((newfd=serv_accept(listenfd,&uid))<0){
				printf("serv_accept error\n");exit(-3);
			}
			i=client_add(listenfd,uid);
			FD_SET(newfd,&allset);
			if(newfd>maxfd)
				maxfd=newfd;
			if(i>maxi)
				maxi=i;
			continue;
		}
		for(i=0;i<=maxi;i++){
			if((newfd=client[i].fd)<0)
				continue;
			if(FD_ISSET(newfd,&rset)){
				if((nread=read(newfd,buf,LINESIZE))<0){
					printf("read error\n");exit(-4);
				}else if(nread==0){
					client_del(newfd);
					FD_CLR(newfd,&allset);
					close(newfd);
				}else{
					handler_request(buf,nread,newfd,client[i].uid);
				}
			}
		}
	}
}

void handler_request(char* buf,int nread,int clifd,uid_t uid){
	int newfd;
	if(buf[nread-1]!=0){
		snprintf(errmsg,LINESIZE-1,"request from uid %d not null terminated\n");
		send_err(clifd,-1,errmsg);
		return;
	}
	if(buf_args(buf,cli_args)<0){
		send_err(chifd,-1,errmsg);
		return;
	}
	if((newfd=open(pathname,oflags))<0){
		snprintf(errmsg,LINESIZE-1,"can't open %s\n",pathname);
		send_err(chifd,-1,errmsg);
		return -1;
	}
	if(send_fd(chifd,newfd)<0)
		return;
	close(newfd);
}

int debug,oflag,log_to_stderr;

int main(int argc,char** argv){
	int c;
	opterr=0;
	while(c=getopt(argc,argv,"d")!=EOF){
		switch(c){
			case 'd':
				debug=log_to_stderr;
				break;
			case '?':
				exit(-1);
		}
	}
	if(debug==0)
		daemonize("opend");
	loop();
}
