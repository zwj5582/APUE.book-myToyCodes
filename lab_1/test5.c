/*************************************************************************
	> File Name: test5.c
	> Author: zhongwenjie
	> Mail: zwj5582@gmail.com 
	> Created Time: Thu 11 Aug 2016 05:30:42 PM PDT
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

#define BUFFERSIZE 4096

#define MAXSLEEP 128

#define BUFLEN 128

#define QLEN 10

#define TIMEOUT

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 256
#endif

#define MAXADDRLEN 256

#define STALE 30

#define PATHNAME "/var/tmp/""

#define CREDSTRUCT ucred

#define SCM_CREDTYPE SCM_CREDENTALS

#define RIGHTSLEN CMSG_LEN(sizeof(int))

#define CREDSLEN CMSG_LEN(sizeof(struct CREDSTRUCT))

#define CONTROLLEN RIGHTSLEN+CREDSLEN

void serve(int sockfd);

int connect_retry(int domain,int type,int protocol,const struct sockaddr* addr,socklen_t alen);

int init_service(int type,const struct sockaddr* addr,socklen_t alen,int qlen);

void print_uptime(int);

static struct cmsghdr* cmptr=NULL;

void daemonize(const char* cmd);

/*int main(int argc,char** argv){
	struct addrinfo* ailist;
	struct addrinfo* aip;
	struct addeinfo hint;
	int err,sockfd;
	memset(&hint,0,sizeof(hint));
	hint.ai_socktype=SOCK_STREAM;
	hint.ai_addr=NULL;
	hint.ai_cannoname=NULL;
	hint.ai_next=NULL;
	err=getaddrinfo(argv[1],"ruptime",&hint,&ailist);
	if(err!=0){
		printf("getaddrinfo error\n");exit(-1);
	}
	for(aip=ailist;aip!=NULL;aip=aip->ai_next){
		if((sockfd=connect_retry(aip->ai_family,SOCK_STREAM,0,aip->ai_addr,aip->ai_addrlen))<0)
			err=errno;
		else{
			print_uptime(sockfd);exit();
		}
	}
	exit(-1);
}*/

/*int main(int argc,char** argv){
	struct addrinfo* ailist;
	struct addrinfo* aip;
	struct addrinfo hint;
	int sockfd,err,n,i=0;
	char* host;
	fd_set read_set;
	int sockfdarr[BUFLEN];
	FD_ZERO(&read_set);
	if((n=sysconf(_SC_HSOT_NAME_MAX))<0)
		n=HOST_NAME_MAX;
	if((host=malloc(n))==NULL){
		printf("malloc error\n");exit(-1);
	}
	if(gethostname(host,n)<0){
		printf("gethostname error\n");exit(-2);
	}
	daemonize("ruptime");
	memset(&hint,0,sizeof(hint));
	hint.ai_socktype=SOCK_STREAM;
	hint.ai_flags=AI_CANNONAME;
	hint.ai_addr=NULL;
	hint.ai_cannoname=NULL;
	hint.ai_next=NULL;
	if((err=getaddrinfo(host,"ruptime",&hint,&ailist))<0){
		printf("getaddrinfo error\n");exit(-3);
	}
	for(aip=ailist;aip!=NULL;aip=aip->next){
		if((sockfd=init_service(SOCK_STREAM,aip->ai_addr,aip->ai_addrlen,QLEN))>=0){
			FD_SET(sockfd,&read_set);
			sockfdarr[i++]=sockfd;
		}
	}
	while(1){
		if(select(FD_SETSIZE,&read_set,NULL,NULL,NULL)>0){
			int j;
			for(j=0;j<i;j++){
				if(FD_ISSET(sockfdarr[j],&read_set))
					serve(socjfdarr[j]);
			}
		}
	}
	exit(1);
}*/

/*int main(int argc,char** argv){
	struct addrinfo* ailist;
	struct addrinfo* aip;
	struct addrinfo hint;
	int sockfd,err;
	struct sigaction sa;
	sa.sa_handler=sig_arlm;
	sa.sa_flags=0;
	sigemptyset(&sa.sa_mask);
	if((sigaction(SIGALRM,&sa,NULL))<0){
		printf("sigaction error\n");exit(-1);
	}
	memset(&hint,0,sizeof(hint));
	hint.ai_socktype=SOCK_DGRAM;
	hint.ai_addr=NULL;
	hint.ai_cannoname=NULL;
	hint.ai_next=NULL;
	if((err=getaddrinfo(argv[1],"ruptime",&hint,&ailist))<0){
		printf("getaddrinfo error\n");exit(-1);
	}
	for(aip=ailist;aip!=NULL;aip=aip->ai_next){
		if((sockfd=socket(aip->ai_family,SOCK_DGRAM,0))>0){
			printf_uptime(sockfd,aip);exit(0);
		}else{
			printf("socket error\n");exit(-1);
		}
	}
	exit(1);
}*/

/*int main(int argc,char** argv){
	struct addrinfo* ailist;
	struct addrinfo* aip;
	struct addrinfo hint;
	int sockfd,err,n;
	char* host;
	if((n=sysconf_SC_HOST_NAME_MAX)<0)
		n=HOST_NAME_MAX;
	if((host=malloc(n))==NULL){
		printf("malloc error\n");exit(-1);
	}
	if((gethostname(host,n))<0){
		printf("gethostname error\n");exit(-1);
	}
	daemonize("ruptime");
	memset(&hint,0,sizeof(hint));
	hint.ai_socktype=SOCK_DGRAM;
	hint.ai_cannoname=AI_CANNONAME;
	hint.ai_cannoname=NULL; 
	hint.ai_addr=NULL;
	hint.ai_next=NULL;
	if((err=getaddrinfo(host,"ruptime",&hint,&ailist))<0){
		printf("getaddrinfo error\n");exit(-1);
	}
	for(aip=ailist;aip!=NULL;aip=aip->ai_next){
		if((sockfd=socket(aip->ai_family,SOCK_DGRAM,0))<0){
			printf("socket error\n");exit(-1);
		}
		serve2(sockfd);exit(0);
	}
	exit(1);
}*/

int main(int argc,char** argv){
	int fd,size;
	struct sockaddr_un un;
	un.sun_family=AF_UNIX;
	strcpy(un.sun_path,"foo.h");
	if((fd=socket(AF_UNIX,SOCK_STREAM,0))<0){
		printf("socket error\n");exit(-1);
	}
	size=offsetof(struct sockaddr_un,sun_path)+sizeof(un.sun_path);
	if(bind(fd,(struct sockaddr*)&un,size)<0){
		printf("bind error\n");exit(-1);
	}
	printf("bind!\n");
	exit(0);
}

int my_send_err(int fd,int status,const char* msg){
	int n;
	if((n=strlen(msg))>0){
		if(write(fd,msg,n)<0)
			return -1;
	}
	if(status>=0)
		status=-1;
	if(send_fd(fd,status)<0)
		return -1;
	return 0;
}

int my_send_fd(int fd,int fd_to_send){
	struct iovec iov[1];
	struct msghdr msg;
	char buf[2];
	iov[0].iov_base=buf;
	iov[0].iov.len=2;
	msg.msg_iov=iov;
	msg.msh_iovlen=1;
	if(fd_to_send<0){
		msg.msg_control=NULL;
		msg.msg_controlen=0;
		buf[1]=-fd_to_send;
		if(buf[1]==0)
			buf[1]=1;
	}else{
		if(cmptr==NULL&&(cmptr=malloc(CONTROLLEN))==NULL)
			return -1;
		cmptr->cmsg_len=CONTROLLEN;
		cmptr->cmsg_level=SOL_SOCKET;
		cmptr->cmsg_type=SCM_RIGHTS;
		msg.msg_control=cmptr;
		msg.msg_controllen=CONTROLLEN;
		*(int*)CMSG_DATA(cmptr)=fd_to_send;
		buf[1]=0;
	}
	buf[0]=0;
	if(sendmsg(fd,&msg,0)!=2)
		return -1;
	return 0;
}

int my_recv_fd(int fd,ssize_t (*userfunc)(int,const void*,size_t)){
	int newfd,nr,status;
	char* ptr;
	char buf[BUFFERSIZE];
	struct iovec iov[1];
	struct msghdr msg;
	status=-1;
	while(1){
		iov[0].iov_base=buf;
		iov[0].iov_len=sizeof(buf);
		msg.msg_iov=iov;
		msg.msg_iovlen=1;
		msg.msg_name=NULL;
		msg.msg_namelen=0;
		if(cmptr==NULL&&(cmptr=malloc(CONTROLLEN))==NULL)
			return -1;
		msg.msg_control=cmptr;
		msg.msg_controllen=CONTROLLEN;
		if((nr=recvmsg(fd,&msg,0))<0)
			return -1;
		else if(nr==0)
			return -1;
		for(ptr=buf;ptr!=&buf[nr];){
			if(*ptr++==0){
				if(ptr!=&buf[nr-1])
					return -1;
				status=*ptr&0xFF;
				if(status==0){
					if(msg.msg_controllen<CONTROLLEN)
						return -1;
					newfd=*(int*)GMSG_DATA(cmptr);
				}else{
					newfd=-status;
				}
				nr-=2;
			}
		}
		if(nr>0&&(*userfunc)(STDOUT_FILENO,buf,nr)!=nr)
			return -1;
		if(status>0)
			return newfd;
	}
}

int send_fd(int fd,int fd_to_send){
	struct iovec iov[1];
	struct CREDSTRUCT *credp;
	struct cmsghdr *cmp;
	struct msgjhdr msg;
	char buf[2];
	iov[0].iov_base=buf;
	iov[0].iov_len=2;
	msg.msg_iov=iov;
	msg.msg_iovlen=1;
	msg.msg_name=NULL;
	msg.msg_namelen=0;
	if(fd_to_send<0){
		msg.msg_control=NULL;
		msg.msg_controllen=CONTROLLEN;
		buf[1]=-fd_to_send;
		if(buf[0]==0)
			buf[0]=1;
	}else{
		if(cmptr==NULL&&(cmptr=malloc(CONTROLLEN))==NULL)
			return -1;
		msg.msg_control=cmptr;
		msg.msg_controllen=CONTROLLEN;
		cmp=cmptr;
		cmp->cmsg_level=SOL_SOCKET;
		cmp->cmsg_type=SCM_RIGHTS;
		cmp->cmsg_len=RIGHTSLEN;
		*(int*)CMSG_DATA(cmp)=fd_to_send;
		cmp=CMSG_NXTHDR(&msg,cmp);
		cmp->cmsg_level=SOL_SOCKET;
		cmp->cmsg_type=SCM_CREDTYPE;
		cmp->cmdg_len=CREDSLEN;
		credp=(struct CREDSTRUCT*)CMSG_DATA(cmp);
		credp->uid=geteuid();
		credp->gid=getegid();
		credp->pid=getpid();
		buf[1]=0;
	}
	buf[0]=0;
	if(sendmsg(fd,&msg,0)<0)
		return -1;
	return 0;
}

int main(int argc,char** argv){
	int n,fd;
	char buf[BUFFERSIZE];
	char line[BUFFERSIZE];
	while((fgets(line,BUFFERSIZE,stdin))!=NULL){
		if(line[strlen(line)-1]=='\n'){
			line[strlen(line)-1]=0;
		}
	}
	if((fd=csopen(line,O_RDONLY))<0)
		continue;
}

int csopen(char* name,int oflag){
	pid_t pid;
	int len;
	char buf[10];
	struct iovec iov[3];
	static int fd[2]={-1,-1};
	if(fd[0]<0){
		if(pipe(fd)<0)
			return -1;
		if((pid=fork())<0){
			return -1;
		}else if(pid==0){
			close(fd[0]);
			if(fd[1]!=STDIN_FILENO&&dup(fd[1],STDIN_FILENO)!=STDIN_FILENO)
				return -1;
			if(fd[1]!=STDOUT_FILENO&&dup(fd[1],STDOUT_FILENO)!=STDOUT_FILENO)
				return -1;
			if((execl("./opend","opend",(char*)0))<0)
				return -1;
		}
		close(fd[1]);
	}
	sprintf(buf," %d",oflag);
	iov[0].iov_base="open ";
	iov[0].iov_len=5;
	iov[1].iov_base=name;
	iov[1].iov_len=strlen(name);
	iov[2].iov_base=buf;
	iov[2].iov_len=strlen(buf)+1;
	len=iov[0].iov_len+iov[1].iov_len+iov[2].iov_len;
	if(writev(fd[0],&iov[0],3)!=len)
		return -1;
	return (recv_fd(fd[0],write));
}

int recv_ufd(int fd,uid_t* uidptr,ssize_t (*userfunc)(int,const void*,size_t)){
	int newfd=-1,status=-1,nr;
	struct CREDSTRUCT* credp;
	struct iovec iov[1];
	struct cmsghdr* cmp;
	struct msghdr msg;
	char buf[BUFFERSIZE];
	char* ptr;
	while(1){
		iov[0].iov_base=buf;
		iov[0].iov_len=1;
		msg.msg_iov=iov;
		msg.msg_len=1;
		msg.msg_name=NULL;
		msg.msg_namelen=0;
		if(cmptr==NULL&&(cmptr=malloc(CONTROLLEN))==NULL)
			return -1;
		msg.msg_control=cmptr;
		msg.msg_controllen=CONTROLLEN;
		if((nr=recvmsg(fd,&msg,0))<0)
			return -1;
		else if(nr==0)
			return -1;
		for(ptr=buf;ptr<&buf[nr];){
			if(*ptr++==0){
				if(ptr!=buf[nr-1])
					return -1;
				status=*ptr&0xFF;
				if(status==0){
					if(msg.msg_controllen<CONTROLLEN)
						return -1;
					for(cmp=CMSG_FIRSTHDR(&msg);cmp!=NULL;cmp=CMSG_NXTHDR(&msg,cmp)){
						if(cmp->cmsg_level!=SOL_SOCKET)
							continue;
						if(cmp->cmsg_type==SCM_RIGHTS)
							newfd=*(int*)CMSG_DATA(cmp);
						else if(cup->cmsg_type==SCM_CREDTYPE){
							credp=(struct CREDSTRUCT*)CMSG_DATA(cmp);
							uidptr=credp->CR_UID;
						}
					}
				}else{
					newfd=-status;
				}
				nr-=2;
			}
		}
		if(nr>0&&(*userfunc)(STDOUT_FILENO,buf,nr)!=nr)
			return -1;
		if(status>0)
			return newfd;
	}
}

int my_serv_listen(const char* name){
	int err,fd,len,rval;
	struct sockaddrun un;
	memset(&nu,0,sizeof(nu));
	nu.sun_family=AF_UNIX;
	if(strlen(name)>sizeof(nu.sun_path)){
		printf("too long\n");return -4;
	}
	unlink(name);
	strcpy(nu.sun_path,name);
	if((fd=socket(AF_UNIX,SOCK_STREAM,0))<0){
		printf("socket error\n");return -3;
	}
	len=offsetof(struct sockaddr_un,sun_path)+sizeof(un.sun_path);
	if(bind(fd,(struct sockaddr*)&un,len)<0){
		printf("bind error\n");return -2;
	}
	if(listen(fd,qlen)<0){
		printf("listen error\n");return -1;
	}
	return fd;
}

int serv_accept(int listenfd,uid_t* uidptr){
	int nfd,err,rval;
	struct sockaddr_un un;
	time_t stlatetime;
	socklen_t len;
	struct stat statbuf;
	char* name;
	if((name=malloc(sizeof(un.sun_path)))==NULL){
		printf("malloc error\n");return -1;
	}
	len=sizeof(un);
	if((nfd=accept(listenfd,(struct sockaddr*)&un,len))<0){
		printf("accept error\n");return -1;
	}
	len-=offsetof(struct sockaddr_un,sun_path);
	strcpy(name,un.sun_path);
	name[len]=0;
	if(stat(name,&statbuf)<0){
		printf("stat error\n");return -1;
	}
	if(S_ISSOCK(statbud.st_mode)==0)
		return -1;
	if(statbuf.st_mode&(S_IRWXG|S_IRWXO)||(statbuf.st_mode&S_IRWXU)!=S_IRWXU)
		return -1;
	stlatetime=time(NULL)-STALE;
	if(statbuf.st_atime<stlatetime||statbuf.st_ctime<stlatetime||statbuf.st_mtime<stlatetime)
		return -1;
	if(uidptr!=NULL)
		*uidptr=statbuf.st_uid;
	unlink(name);
	free(name);
	return nfd;
}

int cli_conn(const char* name){
	struct sockaddr_un un;
	struct sockaddr_un sun;
	int err,fd,nfd,len;
	if(strlen(name)>sizeof(un.sun_path)){
		printf("too long \n");return -1;
	}
	if((fd=socket(AF_UNIX,SOCK_DREAM,0))<0){
		printf("socket error\n");return -1;
	}
	memset(&un,0,sizeof(un));
	sprintf(un.un_path,"%s%s",PATHNAME,getpid());
	sun.un_family=AF_UNIX;
	len=offsetof(struct sockaddr_un,sun_path)+strlen(un.sun_path);
	unlink(un.sun_path);
	if(bind(fd,(struct sockaddr*)&un,len)<0){
		printf("bind error\n");return -1;
	}
	chmod(un.sun_path,S_IRWXU);
	memset(&sun,0,sizeof(sun));
	strcpy(sun.sun_path,name);
	len=offsetof(struct sockaddr_un,sun_path)+strlen(name);
	if((connect(fd,(struct sockaddr*)&sun,len))<0){
		printf("connect error\n");return -1;
	}
	return fd;
}

/*void serve2(int sockfd){
	char buf[BUFLEN];
	socklen_t alen;
	FILE* fp;
	char abuf[MAXADDRLEN];
	struct sockaddr* addr=(struct sockaddr*)abuf;
	set_cloexec(sockfd);
	alen=MAXADDRLEN;
	while(1){
		if((recvfrom(sockfd,buf,BUFLEN,0,addr,alen))<0){
			printf("recvfrom error\n");exit(-1);
		}
		if((fp=popen("/usr/bin/uptime","r"))==NULL){
			sprintf(buf,"error: %s\n",strerror(errno));
			sendto(sockfd,buf,strlen(buf),0,addr,alen);
		}else{
			if((fgets(buf,BUFLEN,fp))!=NULL)
				sendto(sockfd,buf,BUFLEN,0,addr,alen);
			pclose(fp);
		}
	}
}

int main(int argc,char** argv){
	ket_t key;
	long qid;
	size_t nbtype;
	struct mymesg m;
	key=strtol(argv[1],NULL,0);
	if((qid=msgget(key,0))<0){
		printf("msgget error\n");exit(-1);
	}
	memset(&m,0,sizeof(m));
	strncpy(m.mtext,argv[2],MAXSZ);
	nbtype=strlen(m.mtext);
	m.mtype=1;
	if(msgsnd(qid,&m,nbtype,0)<0){
		printf("msgsnd error\n");exit(-1);
	}
	exit(0);
}

void sig_alrm(int signo){
	exit(0);
}

void printf_uptime(int sockfd,struct addrinfo* aip){
	char buf[BUFLEN];
	int n;
	buf[0]=0;
	if((sendto(sockfd,buf,1,aip->ai_addr,aip->ai_addrlen))<0){
		printf("sendto error\n");exit(-1);
	}
	alarm(TIMEOUT);
	if((n=recvfrom(sockfd,buf,BUFLEN,0,NULL,NULL))<0){
		if(errno!=ENTER)
			alarm(0);
		printf("recvfrom error\n");exit(-1);
	}
	alarm(0);
	write(STDOUT_FILENO,buf,n);
}

void serve(sockfd){
	int fd;
	FILE* FP;
	char buf[BUFLEN];
	set_cloexec(sockfd);
	while(1){
		if((fd=accept(sockfd,NULL,NULL))<0){
			printf("accept error\n");exit(-1);
		}
		set_cloexec(fd);
		if((fp=popen("/usr/bin/uptime","r"))==NULL){
			sprintf(buf,"error: %s\n",strerror(errno));
			send(fd,buf,strlen(buf),0);
		}else{
			while(fgets(buf,BUFLEN,fp)!=NULL)
				send(fd,buf,strlen(buf),0);
			pclose(fp);
		}
		close(fd);
	}
}

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
	for(i=0;i<rl.rlim;i++)
		close(i);
	fd0=open("/dev/null",O_RDWR);
	fd1=dup(0);
	fd2=dup(0);
	openlog(cmd,LOG_CONS,LOG_DAEMON);
}

int connect_retry(int domain,int type,int protocol,const struct sockaddr* addr,socklen_t alen){
	int numsec,sockfd;
	for(numsec=1;numsec<=MAXSLEEP;numsec<<=1){
		if((sockfd=socket(domain,type,protocol))<0)
			return -1;
		if((connect(sockfd,addr,alen))==0)
			return sockfd;
		close(sockfd);
		if(numsec<=MAXSLEEP/2)
			sleep(numsec);
	}
	return -1;
}

int init_service(int type,const struct sockaddr* addr,socklen_t alen,int qlen){
	int fd,err=0;
	int res=1;
	if((fd=socket(addr->sa_family,type,0))<0)
		return -1;
	if((setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&res,sizeof(int)))<0)
		goto ERROR;
	if((bind(fd,addr,alen))<0)
		goto ERROR;
	if(type==SOCK_STREAM||type==SOCK_SEQPACKET){
		if((listen(fd,qlen))<0)
			goto ERROR;
	}
	return fd;
ERROR:
	err=errno;
	close(fd);
	errno=err;
	return -1;
}

void print_uptime(int sockfd){
	int n;
	char buf[BUFLEN];
	while((n=recv(sockfd,buf,BUFLEN,0))>0)
		write(STDOUT_FILENO,buf,n);
	if(n<0){
		printf("recv error\n");exit(-1);
	}
}*/
