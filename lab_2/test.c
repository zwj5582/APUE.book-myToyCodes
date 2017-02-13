/*************************************************************************
	> File Name: test.c
	> Author: zhongwenjie
	> Mail: zwj5582@gmail.com 
	> Created Time: Sat 13 Aug 2016 06:08:45 AM PDT
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

struct ucred{
	pid_t pid;
	uid_t uid;
	gid_t gid;
};

//#define SCM_RIGHTS 0x01

#define CL_OPEN "open"

#define BUFFERSIZE 8192

#define LINESIZE 4096

#define CONTROLLEN CMSG_LEN(sizeof(int))

#define CREDOPT SO_PASSCRED

static struct cmsghdr* cmptr=NULL;

int csopen(char*,int);

int send_err(int fd,int status,const char* mag);

int send_fd(int fd,int newfd);

int recv_fd(int fd,ssize_t (*userfunc)(int,const void*,size_t));

int main(int argc,char** argv){
	int fd,n;
	char buf[BUFFERSIZE];
	char line[LINESIZE];
	while((fgets(line,LINESIZE,stdin))!=NULL){
		if(line[LINESIZE-1]=='\n')
			line[LINESIZE]=0;
	//	printf("jjjjjjjjjjjjjjjjjjjj\n");
		if((fd=csopen(line,O_RDONLY))<0)
			exit(-1);
	//	printf("ppppppppppppppppppppppppppppp\n");
		while((n=read(fd,buf,BUFFERSIZE))>0){
			if((write(STDOUT_FILENO,buf,n))!=n)
				exit(-1);
		}
		if(n<0)
			exit(-1);
		close(fd);
	}
	exit(0);
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
			buf[1]==1;
	}else{
		if(cmptr==NULL&&(cmptr=malloc(CONTROLLEN))==NULL)
			return -1;
		msg.msg_control=cmptr;
		msg.msg_controllen=CONTROLLEN;
		cmp=cmptr;
		cmp->cmsg_level=SOL_SOCKET;		
		cmp->cmsg_type=SCM_RIGHTS;
		cmp->cmsg_len=CONTROLLEN;
		*(int*)CMSG_DATA(cmp)=newfd;
		/*cmp=CMSG_NXTHDR(&msg,cmp);
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
	if(sendmsg(fd,&msg,0)!=2)
		return -1;
	return 0;
}

int recv_fd(int fd,ssize_t (*userfunc)(int,const void*,size_t)){
	struct iovec iov[1];
	char buf[LINESIZE];
	struct cmsghdr* cmp;
	struct msghdr msg;
	int status=-1;
	int newfd=-1;
	char* ptr;
	int nr;
	const int on=1;
	if(setsockopt(fd,SOL_SOCKET,CREDOPT,&on,sizeof(int))<0)
		return -1;
	while(1){
		iov[0].iov_base=buf;
		iov[0].iov_len=sizeof(buf);
		msg.msg_iov=iov;
		msg.msg_iovlen=1;
		msg.msg_name=NULL;
		msg.msg_namelen=0;
		if(cmptr==NULL&&(cmptr=malloc(sizeof(struct CONTROLLEN)))==NULL)
			return -1;
		msg.msg_control=cmptr;
		msg.msg_controllen=CONTROLLEN;
		if((nr=recvmsg(fd,&msg,0))<0)
			return -1;
		else if(nr==0)
			return -1;
		for(ptr=buf;ptr<&buf[nr];){
			if(*ptr++==0){
				if(ptr!=&buf[nr-1])
					return -1;
				status=*ptr&0xFF;
				if(status==0){
					if(msg.msg_controllen!=CONTROLLEN)
						return -1;
					for(cmp=CMSG_FIRSTHDR(&msg);cmp!=NULL;cmp=CMSG_NXTHDR(&msg,cmp)){
						if(cmp->cmsg_level!=SOL_SOCKET)
							continue;
						switch(cmp->cmsg_type){
							case SCM_RIGHTS:
								newfd=*(int*)CMSG_DATA(cmp);
								break;
						}
					}
				}else
					newfd=-status;
			}
			nr-=2;
		}
		if(nr>0&&(*userfunc)(STDERR_FILENO,buf,nr)!=nr)
			return -1;
		if(status>=0)
			return newfd;
	}
}

int csopen(char* name,int oflag){
	pid_t pid;
	uid_t uid;
	struct iovec iov[3];
	static int fd[2]={-1,-1};
	int len;
	char buf[10];
	if(fd[0]<0){
		if(socketpair(AF_UNIX,SOCK_STREAM,0,fd)<0)
			return -1;
	//	printf("pipuipipi\n");
		if((pid=fork())<0)
			return -1;
		else if(pid==0){
			close(fd[0]);
			if(fd[1]!=STDIN_FILENO&&dup2(fd[1],STDIN_FILENO)!=STDIN_FILENO)
				return -1;
			if(fd[1]!=STDOUT_FILENO&&dup2(fd[1],STDOUT_FILENO)!=STDOUT_FILENO)
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
	if((writev(fd[0],&iov[0],3))!=len)
		return -1;
	return (recv_fd(fd[0],&uid,write));
}
