/*************************************************************************
	> File Name: opend.c
	> Author: zhongwenjie
	> Mail: zwj5582@gmail.com 
	> Created Time: Sat 13 Aug 2016 08:06:59 PM PDT
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

#define BUFFERSIZE 8196

#define CL_OPEN "open"

#define LINESIZE 4096

#define MAXARGC 50

#define WHITE " \t\n"

#define CONTROLLEN CMSG_LEN(sizeof(int))


#define CREDOPT SO_PASSCRED

static struct cmsghdr* cmptr=NULL;

int send_err(int fd,int status,const char* mag);

int send_fd(int fd,int newfd);

char errmsg[LINESIZE];

int oflags;

char* pathname;

int cli_args(int,char**);

int buf_args(char* buf,int (*userfunc)(int,char**));

void handler_request(char*,int,int);

int main(void){
	int nread;
	char buf[LINESIZE];
	while(1){
		if((nread=read(STDIN_FILENO,buf,LINESIZE))<0)
			exit(0);
		else if(nread==0)
			break;
		handler_request(buf,nread,STDOUT_FILENO);
	}
	exit(0);
}

void handler_request(char* buf,int nread,int fd){
	int newfd;
	if(buf[nread-1]!=0){
		snprintf(errmsg,LINESIZE-1,"request not null terminated: %*.*\n",nread,nread,buf);
		send_err(fd,-1,errmsg);
		return;
	}
open("./ooooo",O_RDWR|O_CREAT,0666);
	if(buf_args(buf,cli_args)<0){
		send_err(fd,-1,errmsg);
		return;
	}
	open("./aaa",O_RDONLY|O_CREAT,0666);
	if((newfd=open(pathname,oflags))<0){
		open("./ooooo",O_RDWR|O_CREAT,0666);
		snprintf(errmsg,LINESIZE-1,"can't open %s \n",pathname);
		send_err(fd,-1,errmsg);
		return;
	}
	if(send_fd(fd,newfd)<0)
		return;
	open("./yyyyy",O_RDWR|O_CREAT,0666);
	close(newfd);
}

int buf_args(char* buf,int (*userfunc)(int,char**)){
	char* ptr;
	int argc;
	char* argv[MAXARGC];
	if(strtok(buf,WHITE)==NULL)
		return -1;
	argv[argc=0]=buf;
	while((ptr=strtok(NULL,WHITE))!=NULL){
		if(++argc>=MAXARGC-1)
			return -1;
		argv[argc]=ptr;
	}
	argv[argc++]=NULL;
	return (*userfunc)(argc,argv);
}

int cli_args(int argc,char** argv){
	int fd;
	if(argc!=3||strcmp(argv[0],CL_OPEN)!=0){
		strcpy(errmsg,"usage: <pathname> <oflag>\n");return -1;
	}
fd=open("./bbbb",O_RDWR|O_CREAT,0666);
	pathname=argv[1];
	write(fd,pathname,strlen(pathname));
open("./ddddd",O_RDWR|O_CREAT,0666);
	oflags=O_RDONLY;
open("./cccc",O_RDWR|O_CREAT,0666);
	return 0;
}

int send_err(int fd,int errcode,const char* msg){
	int n;
	if((n=strlen(msg))>0)
		if((write(fd,msg,n))!=n)
			return -1;
	if(errcode>=0)
		errcode=-1;
	if(send_fd(fd,errcode)<0)
		return -1;
	return 0;
}

int send_fd(int fd,int newfd){
	struct iovec iov[1];
	struct cmsghdr* cmp;
	struct msghdr msg;
	struct CREDSTRUCT* credp;
	char buf[2];
open("./yyyy",O_RDWR|O_CREAT,0666);
	iov[0].iov_base=buf;
	iov[0].iov_len=2;
	msg.msg_iov=iov;
	msg.msg_iovlen=1;
	msg.msg_name=NULL;
	msg.msg_namelen=0;
	msg.msg_flags=0;
	if(newfd<0){
		msg.msg_control=NULL;
		msg.msg_controllen=0;
		buf[1]=-newfd;
		if(buf[1]==0)
			buf[1]=1;
	}else{
		if(cmptr==NULL&&(cmptr=malloc(CONTROLLEN))==NULL)
			return -1;
		msg.msg_control=cmptr;
		msg.msg_controllen=CONTROLLEN;
		cmp=cmptr;
		cmp->cmsg_level=SOL_SOCKET;		
		cmp->cmsg_type=SCM_RIGHTS;
		cmp->cmsg_len=RIGHTSLEN;
		*(int*)CMSG_DATA(cmp)=newfd;
	/*	cmp=CMSG_NXTHDR(&msg,cmp);
		cmp->cmsg_level=SOL_SOCKET;
		cmp->cmsg_type=SCM_CREDTYPE;
		cmp->cmsg_len=CREDSLEN;
		credp=(struct CREDSTRUCT*)CMSG_DATA(cmp);
		credp->uid=geteuid();
		credp->gid=getegid();
		credp->pid=getpid();*/
		buf[1]=0;
	}
	buf[0]=0;
open("./xxxxx",O_RDWR|O_CREAT,0666);
	if(sendmsg(fd,&msg,0)!=2)
		return -1;
open("./zzzz",O_RDWR|O_CREAT,0666);
	return 0;
}
