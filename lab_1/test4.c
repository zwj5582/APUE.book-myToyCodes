/*************************************************************************
	> File Name: test4.c
	> Author: zhongwenjie
	> Mail: zwj5582@gmail.com 
	> Created Time: Mon 08 Aug 2016 06:11:14 AM PDT
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

#define BUFFERSIZE 4096

#define COPYSIZE 1024*1024*1024

#define BSZ 4096

#define NBUF 8

#define open_max() 256

#define DF_PAGER "/bin/more"

#define NUMSEC 128

static pid_t* childpid=NULL;

static int maxfd;

static int fd1[2];

static int fd2[2];

enum rwop{
	UNUSED=0,
	READ_PENDING=1,
	WRITE_PENDING=2
};

struct buf{
	enum rwop op;
	struct aiocb aiocb;
	int last;
	unsigned char data[BSZ];
};

struct buf bufs[NBUF];

unsigned char translate(unsigned char);

char buf[500000];

void set_fl(int fd,int flags);

void clr_fl(int fd,int flags);

int lock_reg(int fd,int cmd,int type,off_t off,int whence,off_t len);

#define RWRWR S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH

#define read_lock(fd,off,whence,len) \
	lock_reg((fd),F_SETLK,F_RDLCK,(off),(whence),(len))

#define readw_lock(fd,off,whence,len) \
	lock_reg((fd),F_SETLKW,F_RDLCK,(off),(whence),(len))

#define write_lock(fd,off,whence,len) \
	lock_reg((fd),F_SETLK,F_WRLCK,(off),(whence),(len))

#define writew_lock(fd,off,whence,len) \
	lock_reg((fd),F_SETLKW,F_WRLCK,(off),(whence),(len))

pid_t lock_test(int fd,int type,off_t off,int whence,off_t len);

#define is_read_lockable(fd,off,whence,len) \
	(lock_test((fd),F_RDLCK,(off),(whence),(len))==0)

#define id_write_lock_able(fd,off,whence,len) \
	(lock_test((fd),F_WRLCK,(off),(whence),(len))==0)

struct mymesg{
	long mtype;
	char mtext[512];
};

int lockfile(int fd);

void sig_fn(int);

FILE* my_popen(const char*,const char*);

void sig_pipe(int);

void print_type(struct addrinfo*);

void print_protocol(struct addrinfo*);

void print_flags(struct addrinfo*);

void print_family(struct addrinfo*);

int main(int argc,char** argv){
//	int fd,fd1,fd2,i;
//	for(i=0;i<1024;i++){
//		close(i);
//	}
	/*fd=open("/dev/null",O_RDWR);
	fd1=dup(0);
	fd2=dup(0);
	char* p=getlogin();
	printf("%s\n",p);*/


/*	int ntowrite,nwrite;
	char* ptr;
	ntowrite=read(STDIN_FILENO,buf,sizeof(buf));
	fprintf(stderr,"read %d bytes\n",ntowrite);
	set_fl(STDOUT_FILENO,O_NONBLOCK);
	ptr=buf;
	while(ntowrite>0){
		errno=0;
		nwrite=write(STDOUT_FILENO,ptr,ntowrite);
		fprintf(stderr,"write %d bytes\n",nwrite);
		if(nwrite>0){
			ntowrite-=nwrite;
			ptr+=nwrite;
		}
	}
	clr_fl(STDOUT_FILENO,O_NONBLOCK);
	exit(0);*/

	/*pid_t pid;
	int fd;
	umask(0);
	if((fd=open("zhong",O_RDWR|O_CREAT,RWRWR))<0){
		printf("open error\n");exit(-1);
	}
	write(fd,"panjiaqi",9);
	if((write_lock(fd,0,SEEK_SET,100))<0){
		printf("parent lock error\n");exit(-2);
	}
	if((pid=fork())<0){
		printf("fork error\n");exit(-2);
	}else if(pid==0){
		if(read_lock(fd,0,SEEK_SET,100)<0){
			printf("read_lock error\n");
		}
		if(write(fd,"zhong",6)<0)
			printf("child write error\n");
	}else{
		sleep(2);
		if(write(fd,"wenjie",7)<0)
			printf("parent write error\n");
	}
	exit(0);*/


	/*int fd;
	pid_t pid;
	struct stat statbuf;
	char buf[5];
	if((fd=open(argv[1],O_RDWR|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH))<0){
		printf("open error\n");exit(-1);
	}
	if((write(fd,"abcdef",6))!=6){
		printf("write error\n");exit(-2);
	}
	if(fstat(fd,&statbuf)<0){
		printf("fstat error\n");exit(-3);
	}
	if(fchmod(fd,(statbuf.st_mode&~S_IXGRP)|S_ISGID)<0){
		printf("fchmod error\n");exit(-4);
	}
	if((pid=fork())<0){
		printf("fork error\n");exit(-5);
	}else if(pid==0){
		sleep(10);
		set_fl(fd,O_NONBLOCK);
		if((read_lock(fd,0,SEEK_SET,0))!=-1){
			printf("read_lock success\n");exit(-8);
		}
		printf("block again\n");
		if((lseek(fd,0,SEEK_SET))<0){
			printf("lseek error\n");exit(-9);
		}
		if((read(fd,buf,2))<0)
			printf("read block\n");
		else
			printf("read success\n");
		exit(0);
	}else{
		if((write_lock(fd,0,SEEK_SET,0))<0){
			printf("write_lock error\n");exit(-6);
		}
		if((waitpid(pid,NULL,0))<0){
			printf("waitpid error\n");exit(-7);
		}
	}
	exit(0);*/


	/*int fd,ret,len,fd2;
	char buf[BUFFERSIZE];
	fd_set setread,setwrite;
	struct timeval timeout;
	if((fd=open(argv[1],O_RDWR|O_CREAT|O_NONBLOCK),RWRWR)<0){
		printf("open error\n");exit(-1);
	}
	//fd2=open("/dev/tty",O_RDONLY|O_NONBLOCK);
	timeout.tv_sec=10;
	timeout.tv_usec=0;
	FD_ZERO(&setread);
	FD_ZERO(&setwrite);
	//FD_SET(,&setwrite);
	FD_SET(0,&setread);
	//FD_SET(fd,&setread);
	FD_SET(fd,&setwrite);
	while(1){
		ret=select(1024,&setread,NULL,NULL,&timeout);
		printf("%d\n",ret);
		if(FD_ISSET(0,&setread)){
			printf("hhhhhhhhhh\n");
			if((len=read(STDIN_FILENO,buf,BUFFERSIZE))<0){
				printf("read error\n");exit(-2);
			}
		}
	}
	while(1){
		ret=select(1024,NULL,&setwrite,NULL,&timeout);
		if(FD_ISSET(fd,&setwrite)){
			printf("llllllllll\n");
			if((write(fd,buf,len))!=len){
				printf("write error\n");exit(-3);
			}
		}
	}*/


	/*int err,ifd,ofd,i,j,n,nmop;
	struct stat sbuf;
	const struct aiocb *aiolist[NBUF];
	off_t off=0;
	if((ifd=open(argv[1],O_RDONLY))<0){
		printf("open error\n");exit(-1);
	}
	if((ofd=open(argv[2],O_RDWR|O_CREAT|O_TRUNC,RWRWR))<0){
		printf("open error\n");exit(-1);
	}
	if(fstat(ifd,&sbuf)<0){
		printf("lstat error\n");exit(-2);
	}
	for(i=0;i<NBUF;i++){
		bufs[i].op=UNUSED;
		bufs[i].aiocb.aio_sigevent.sigev_notify=SIGEV_NONE;
		bufs[i].aiocb.aio_buf=bufs[i].data;
		aiolist[i]=NULL;
	}
	nmop=0;
	while(1){
		for(i=0;i<NBUF;i++){
			switch(bufs[i].op){
				case UNUSED:
					if(off<sbuf.st_size){
						bufs[i].op=READ_PENDING;
						bufs[i].aiocb.aio_fildes=ifd;
						bufs[i].aiocb.aio_offset=off;
						off+=BSZ;
						if(off>=sbuf.st_size){
							bufs[i].last=1;
						}
						bufs[i].aiocb.aio_nbytes=BSZ;
						if(aio_read(&bufs[i].aiocb)<0){
							printf("aio_read error\n");exit(-3);
						}
						aiolist[i]=&bufs[i].aiocb;
						nmop++;
					}
					break;
				case READ_PENDING:
					if((err=aio_error(&bufs[i].aiocb))==EINPROGRESS)
						continue;
					if(err!=0){
						if(err==-1){
							printf("aio_error error\n");exit(-4);
						}else{
							printf("aio_error return\n");exit(-5);
						}
					}
					if((n=aio_return(&bufs[i].aiocb))<0){
						printf("aio_return error\n");exit(-6);
					} 
					if(n!=BSZ&&!bufs[i].last){
						printf("read error\n");exit(-7);
					}
					for(j=0;j<n;j++){
						//bufs[i].data[j]=translate(bufs[i].data[j]);
						((char*)bufs[i].aiocb.aio_buf)[j]=translate(((char*)bufs[i].aiocb.aio_buf)[j]);
					}
					bufs[i].op=WRITE_PENDING;
					bufs[i].aiocb.aio_fildes=ofd;
					bufs[i].aiocb.aio_nbytes=n;
					if((aio_write(&bufs[i].aiocb))<0){
						printf("aio_write error\n");exit(-8);
					}
					break;
				case WRITE_PENDING:
					if((err=aio_error(&bufs[i].aiocb))==EINPROGRESS)
						continue;
					if(err!=0){
						if(err=-1){
							printf("aio_error error\n");exit(-9);
						}else{
							printf("aio_error return\n");exit(-10);
						}
					}
					if((n=aio_return(&bufs[i].aiocb))<0){
						printf("aio_return error\n");exit(-11);
					}
					if(n!=bufs[i].aiocb.aio_nbytes){
						printf("write error\n");exit(-12);
					}
					aiolist[i]=NULL;
					bufs[i].op=UNUSED;
					nmop--;
					break;
			}
		}
		if(nmop==0){
			if(off>=sbuf.st_size){
				break;
			}
		}else{
			if(aio_suspend(aiolist,NBUF,NULL)<0){
				printf("aio_supspend error\n");exit(-13);
			}
		}
	}
	//bufs[0].aiocb.aio_fildes=ofd;
	if((aio_fsync(O_SYNC,&bufs[0].aiocb))<0){
		printf("aio_fsync error\n");exit(-14);
	}
	exit(0);*/
	
	/*printf("%d\n",_SC_PAGESIZE);
	exit(0);*/

	
	/*int fdin,fdout;
	void* infd;
	void* outfd;
	size_t copysize;
	struct stat statbuf;
	off_t off=0;
	if((fdin=open(argv[1],O_RDONLY))<0){
		printf("open error\n");exit(-1);
	}
	if((fdout=open(argv[2],O_RDWR|O_CREAT|O_TRUNC,RWRWR))<0){
		printf("open error\n");exit(-2);
	}
	if(fstat(fdin,&statbuf)<0){
		printf("fstat error\n");exit(-3);
	}
	if((ftruncate(fdout,statbuf.st_size))<0){
		printf("ftruncate error\n");exit(-4);
	}
	while(off<statbuf.st_size){
		if(statbuf.st_size-off>COPYSIZE)
			copysize=COPYSIZE;
		else
			copysize=statbuf.st_size;
		if((infd=mmap(0,copysize,PROT_READ,MAP_SHARED,fdin,off))==MAP_FAILED){
			printf("mmap error\n");exit(-5);
		}
		if((outfd=mmap(0,copysize,PROT_READ|PROT_WRITE,MAP_SHARED,fdout,off))==MAP_FAILED){
			printf("mmap error\n");exit(-6);
		}
		memcpy(outfd,infd,copysize);
		munmap(infd,copysize);
		munmap(outfd,copysize);
		off+=copysize;
	}
	exit(0);*/


	/*pid_t pid1,pid2,pid3;
	int err,fd;
	if(signal(SIGINT,sig_fn)<0){
		printf("signal error\n");exit(-1);
	}
	setbuf(stdout,NULL);
	if((fd=open(argv[1],O_RDWR|O_CREAT|O_TRUNC,RWRWR))<0){
		printf("opne error\n");exit(-2);
	}
	if((pid1=fork())<0){
		printf("fork error\n");exit(-3);
	}else if(pid1==0){
		printf("in child 1\n");
		if((read_lock(fd,0,SEEK_SET,0))<0){
			printf("child 1: read lock error\n");
		}else
			printf("child 1: read lock\n");
		pause();
		printf("child 1: pause return\n");
		exit(0);
	}else{
		sleep(2);
		if((pid2=fork())<0){
			printf("fork error\n");
		}else if(pid2==0){
			printf("in child 2\n");
			if((read_lock(fd,0,SEEK_SET,0))<0){
				printf("child 2: read lock error\n");
			}else
				printf("child 2: read lock\n");
			pause();
			printf("child 2: pause return\n");
			exit(0);
		}else{
			sleep(2);
			if((pid3=fork())<0){
				printf("fork error\n");
			}else if(pid3==0){
				printf("in child 3\n");
				if((write_lock(fd,0,SEEK_SET,0))<0){
					printf("child 3: write lock error\n");
				}else
					printf("child 3; write lock\n");
				if((writew_lock(fd,0,SEEK_SET,0))<0){
					printf("child 3: writew lock error\n");exit(-1);
				}else
					printf("child 3: writew lock error\n");
				pause();
				exit(0);
			}else{
				sleep(2);
				if((read_lock(fd,0,SEEK_SET,0))<0){
					printf("parent: read lock error\n");
				}else
					printf("parent: read lock\n");
				printf("start kill child 1\n");
				kill(pid1,SIGINT);
				printf("start kill child 2\n");
				kill(pid2,SIGINT);
				printf("start kill child 3\n");
				kill(pid3,SIGINT);
				sleep(13);
				exit(0);
			}
		}
	}
	exit(0);*/
	
	
	//printf("%ld",_PC_PIPE_BUF);
	//
	//
	/*int n,fd[2];
	pid_t pid;
	char buf[BUFFERSIZE];
	if(pipe(fd)<0){
		printf("pipe error\n");exit(-1);
	}
	if((pid=fork())<0){
		printf("fork error\n");exit(-2);
	}else if(pid==0){
		printf("in child\n");
		close(fd[1]);
		n=read(fd[0],buf,BUFFERSIZE);
		write(STDOUT_FILENO,buf,n);
		exit(0);
	}else{
		printf("in parent\n");
		close(fd[0]);
		write(fd[1],"hello world\n",12);
		exit(0);
	}*/


	/*int n,fd[2];
	pid_t pid;
	char* pager;
	char* argv0;
	char buf[BUFFERSIZE];
	FILE* fp;
	if((fp=fopen(argv[1],"r"))==NULL){
		printf("fopen error\n");exit(-1);
	}
	if(pipe(fd)<0){
		printf("pipe error\n");exit(-2);
	}
	if((pid=fork())<0){
		printf("fork error\n");exit(-3);
	}else if(pid>0){
		close(fd[0]);
		while((fgets(buf,BUFFERSIZE,fp))!=NULL){
			n=strlen(buf);
			if((n=write(fd[1],buf,n))!=n){
				printf("write error\n");exit(-4);
			}
		}
		if(ferror(fp)){
			printf("fget error\n");exit(-5);
		}
		close(fd[1]);
		if((waitpid(pid,NULL,0))<0){
			printf("waitpid error\n");exit(-6);
		}
		exit(0);
	}else{
		close(fd[1]);
		if(fd[0]!=STDIN_FILENO){
			if(dup2(fd[0],STDIN_FILENO)!=STDIN_FILENO){
				printf("dup2 error\n");exit(-7);
			}
		}
		close(fd[0]);
	}
	if((pager=getenv("PAGER"))==NULL){
		pager=DF_PAGER;
	}
	if((argv0=strrchr(pager,'/'))!=NULL){
		argv0++;
	}else{
		argv0=pager;
	}
	if((execl(DF_PAGER,argv0,(char*)0))<0){
		printf("execl error\n");exit(-8);
	}*/


	/*char buf[BUFFERSIZE];
	FILE* fdin;
	FILE* fdout;
	if((fdin=fopen(argv[1],"r"))==NULL){
		printf("fopen error\n");exit(-1);
	}
	if((fdout=popen("/bin/more","w"))==NULL){
		printf("popen error\n");exit(-2);
	}
	while((fgets(buf,BUFFERSIZE,fdin))!=NULL){
		if(fputs(buf,fdout)==EOF){
			printf("fputs error\n");exit(-3);
		}
	}
	if(ferror(fdin)){
		printf("fgets error\n");exit(-4);
	}
	if(pclose(fdout)==-1){
		printf("pclose error\n");exit(-5);
	}
	exit(0);*/


	/*char buf[BUFFERSIZE];
	FILE* fp;
	if((fp=popen("./mych","r"))==NULL){
		printf("popen error\n");exit(-1);
	}
	while(1){
		fputs("> ",stdout);
	//	fflush(stdout);
		if((fgets(buf,BUFFERSIZE,fp))==NULL)
			break;
		if(fputs(buf,stdout)==EOF){
			printf("fputs error\n");exit(-2);
		}
	}
	if(pclose(fp)==-1){
		printf("pclose error\n");exit(-3);
	}
	putchar('\n');
	exit(0);*/


	/*char buf[BUFFERSIZE];
	int n,fd1[2],fd2[2];
	pid_t pid;
	if((signal(SIGPIPE,sig_pipe))==SIG_ERR){
		printf("signal error\n");exit(-1);
	}
	if(pipe(fd1)<0||pipe(fd2)<0){
		printf("pipe error\n");exit(-2);
	}
	if((pid=fork())<0){
		printf("fork error\n");exit(-3);
	}else if(pid>0){
		close(fd1[0]);
		close(fd2[1]);
		while((fgets(buf,BUFFERSIZE,stdin))!=NULL){
			n=strlen(buf);
			if((write(fd1[1],buf,n))!=n){
				printf("write error\n");exit(-4);
			}
			if((n=read(fd2[0],buf,BUFFERSIZE))<0){
				printf("read error\n");exit(-5);
			}
			if(n==0){
				printf("child close\n");break;
			}
			buf[n]=0;
			if((fputs(buf,stdout))==EOF){
				printf("fputs error\n");exit(-6);
			}
		}
		if(ferror(stdin)){
			printf("fgets error\n");exit(-7);
		}
		exit(0);
	}else{
		close(fd1[1]);
		close(fd2[0]);
		if(fd1[0]!=STDIN_FILENO)
			dup2(fd1[0],STDIN_FILENO);
		close(fd1[0]);
		if((fd2[1]!=STDOUT_FILENO))
			dup2(fd2[1],STDOUT_FILENO);
		close(fd2[1]);
		if((execl("./mych","mych",(char*)0))<0){
			printf("execl error\n");exit(-8);
		}
	}
	exit(0);*/

	/*key_t key;
	pid_t pid;
	char buf[512]="abcd";
	struct mymesg mymesg;
	struct mymesg mymesg1;
	strcpy(mymesg.mtext,buf);
	key=ftok("./test4.c",245);
	int k=msgget(key,IPC_CREAT);
	setbuf(stdout,NULL);
	printf("%ld\n",k);
	msgsnd(k,&mymesg,4,IPC_NOWAIT);
	if((pid=fork())<0){
		printf("fork error\n");exit(-1);
	}else if(pid==0){
		msgrcv(k,&mymesg1,512,0,IPC_NOWAIT);
		printf("%s\n",mymesg1.mtext);
		sleep(10);
		system("ipcs");
		system("./test");
		exit(0);
	}else{
		sleep(20);
	}
	exit(0);*/

	/*int a=0;
	pid_t pid;
	pid=fork();
	if(pid==0){
		sleep(2);
		while(1){
			sleep(100);
			a++;
			printf("%d\n",a);
		}
		exit(0);
	}else{
		exit(0);
	}*/


	/*struct hostent* host;
	host=gethostent();
	printf("%s\n",host->h_name);*/
	//int i=0,j=0;
	//while(host->h_aliases[i]!=NULL)
	//	printf("%s\n",host->h_aliases[i++]);
	//printf("%d\n",host->h_addrtype);
	//while(host->h_addr_list[j]!=NULL)
	//	printf("%s\n",host->h_addr_list[j++]);
	//endhostent();
	//exit(0);

	/*struct servent *serv;
	serv=getservbyname("ftp","tcp");
	printf("%s\n",serv->s_name);
	printf("%d\n",serv->s_port);
	printf("%s\n",serv->s_proto);*/

	/*char buf[BUFFERSIZE];
	char bufr[BUFFERSIZE];
	struct addrinfo hint;
	struct addrinfo* res;
	getaddrinfo(host->h_name,NULL,&hint,&res);
	bufr=inet_ntop(AF_INET,&res->ai_addr->sin_addr,buf,BUFFERSIZE);
	printf("%s\n",bufr);*/

	struct addrinfo* ailist;
	struct addrinfo* aip;
	struct addrinfo hint;
	const char* addr;
	struct sockaddr_in *sinp;
	char buf[BUFFERSIZE];
	int err;
	/*int i;
	struct hostent* host;
	host=gethostent();
	printf("%s\n",host->h_name);
	for(i=0;host->h_aliases[i]!=NULL;i++)
		printf("%s\n",host->h_aliases[i]);*/
	hint.ai_flags=AI_CANONNAME;
	hint.ai_flags=0;
	hint.ai_protocol=0;
	hint.ai_socktype=0;
	hint.ai_addrlen=0;
	hint.ai_addr=NULL;
	hint.ai_canonname=NULL;
	hint.ai_next=NULL;
	err=getaddrinfo(NULL,argv[2],&hint,&aip);
	if(err!=0){
		printf("%s\n",gai_strerror(err));exit(-1);
	}
	for(ailist=aip;ailist!=NULL;ailist=ailist->ai_next){
		print_flags(ailist);
		print_family(ailist);
		print_type(ailist);
		print_protocol(ailist);
		printf("\nhost: %s",ailist->ai_canonname?ailist->ai_canonname:"-");
		if(ailist->ai_family==AF_INET){
			sinp=(struct sockaddr_in*)ailist->ai_addr;
			addr=inet_ntop(AF_INET,&sinp->sin_addr,buf,BUFFERSIZE);
			printf(" add: %s",addr?addr:"unknow");
			printf(" port: %d",ntohs(sinp->sin_port));
		}
		printf("\n");
	}
	exit(0);
}

int connect_retry(int domain,int type,int protocol,const struct sockaddr* addr,socklen_t alen){
	int fd,numsec;
	for(numsec=1;numsec<=NUMSEC;numsec<<=1){
		if((socket(domain,type,protocol))==-1)
			return -1;
		if((connect(fd,addr,alen))==0){
			return fd;
		}
		if(numsec<=NUMSEC/2)
			sleep(numsec);
		close(fd);
	}
	return -1;
}

int initservice(int type,const struct sockaddr* addr,socklen_t alen,int qlen){
	int fd;
	int err=0;
	if(fd=socket(addr->sa_family,type,0)==-1)
		return -1;
	if(bind(fd,addr,alen)<0)
		goto ERROR;
	if(type==SOCK_STREAM||type==SOCK_SEQPACKET){
		if(listen(fd,qlen)<0)
			goto ERROR;
	}
	return fd;
ERROR:
	err=errno;
	close(fd);
	errno=err;
	return -1;
}

void print_family(struct addrinfo* aip){
	printf(" family: ");
	switch(aip->ai_family){
		case AF_INET:
			printf("ipv4");
			break;
		case AF_INET6:
			printf("ipv6");
			break;
		case AF_UNIX:
			printf("unix");
			break;
		default:
			printf("unknow");
			break;
	}
}

void print_type(struct addrinfo* aip){
	printf(" type: ");
	switch(aip->ai_socktype){
		case SOCK_DGRAM:
			printf("dgram");
			break;
		case SOCK_RAW:
			printf("raw");
			break;
		case SOCK_SEQPACKET:
			printf("seqpacket");
			break;
		case SOCK_STREAM:
			printf("stream");
			break;
		default:
			printf("unknow");
			break;
	}
}

void print_protocol(struct addrinfo* aip){
	printf(" protocol: ");
	switch(aip->ai_protocol){
		case 0:
			printf("default");
			break;
		case IPPROTO_TCP:
			printf("TCP");
			break;
		case IPPROTO_UDP:
			printf("UDP");
			break;
		case IPPROTO_RAW:
			printf("RAW");
			break;
		default:
			printf("unknow");
			break;
	}
}

void print_flags(struct addrinfo* aip){
	printf(" flags: ");
	if(aip->ai_flags==0){
		printf("0");
	}else{
		if(aip->ai_flags&AI_ADDRCONFIG)
			printf("addrconfig");
		if(aip->ai_flags&AI_ALL)
			printf("all");
		if(aip->ai_flags&AI_CANONNAME)
			printf("canonname");
	}
}

void sig_pipe(int signo){
	printf("in sig_pipe\n");exit(-1);
}

FILE* my_popen(const char* cmd,const char* type){
	int i,fd[2];
	pid_t pid;
	FILE* fp;
	if((type[0]!='r'||type[0]!='w')&&type[1]!=0)
		return NULL;
	if(childpid==NULL){
		maxfd=open_max();
		if((childpid=calloc(maxfd,sizeof(pid_t)))==NULL)
			return NULL;
	}
	if(pipe(fd)<0){
		printf("pipe error\n");return NULL;
	}
	if(fd[0]>=maxfd||fd[1]>=maxfd){
		close(fd[0]);
		close(fd[1]);
		return NULL;
	}
	if((pid=fork())<0){
		printf("fork error\n");return NULL;
	}else if(pid==0){
		if(type[0]=='r'){
			close(fd[0]);
			if(fd[1]!=STDOUT_FILENO){
				dup2(fd[1],STDOUT_FILENO);
				close(fd[1]);
			}
		}else{
			close(fd[1]);
			if(fd[0]!=STDIN_FILENO){
				dup2(fd[0],STDIN_FILENO);
				close(fd[0]);
			}
		}
		for(i=0;i<maxfd;i++){
			if(childpid[i]>0)
				close(i);
		}
		if((execl("/bin/bash","bash","-c",cmd,(char*)0))<0){
			printf("exexl error\n");return NULL;
		}
	}else{
		if(type[0]=='r'){
			close(fd[1]);
			if((fp=fdopen(fd[0],type))==NULL)
				return NULL;
		}
		else{
			close(fd[0]);
			if((fp=fdopen(fd[1],type))==NULL)
				return NULL;
		}
	}
	childpid[fileno(fp)]=pid;
	return fp;
}

int fclose(FILE* fp){
	int status,fd;
	pid_t pid;
	if(childpid==NULL)
		return -1;
	fd=fileno(fp);
	if(fd>=maxfd)
		return -1;
	if(fclose(fp)==EOF)
		return -1;
	if((pid=childpid[fd])==0)
		return -1;
	while(waitpid(pid,&status,0)<0)
		return -1;
	return status;
}

void sig_fn(int signo){}

void WAIT(void){
	if(pipe(fd1)<0|pipe(fd2)<0){
		printf("pipe error\n");exit(-1);
	}
}

void WAIT_PARENT(void){
	char c;
	if((read(fd1[0],&c,1))!=1){
		printf("read error\n");exit(-1);
	}
	if(c!='p'){
		printf("wait_parent error\n");exit(-2);
	}
}

void TELL_CHILD(void){
	if((write(fd1[1],"p",1))!=1){
		printf("write error\n");exit(-1);
	}
}

void WIAT_CHILD(void){
	char c;
	if((read(fd2[0],&c,1))!=1){
		printf("read error\n");exit(-1);
	}
	if(c!='c'){
		printf("wait_child error\n");exit(-2);
	}
}

void TELL_PARENT(void){
	if((write(fd2[1],"c",1))!=1){
		printf("write error\n");exit(-1);
	}
}

ssize_t my_readn(int fd,void* ptr,size_t n){
	int nread,nleft=n;
	while(nleft>0){
		if((nread=read(fd,ptr,nleft))<0){
			if(nleft==n)
				return -1;
			else
				break;
		}else if(nread==0)
			break;
		ptr+=nread;
		nleft-=nread;
	}
	return n-nleft;
}

ssize_t my_writen(int fd,void* ptr,size_t n){
	int nwrite,nleft=n;
	while(nleft>0){
		if((nwrite=write(fd,ptr,nleft))<0){
			if(n==nleft)
				return -1;
			else
				break;
		}else if(nwrite==0)
			break;
		ptr+=nwrite;
		nleft-=nwrite;
	}
	return n-nleft;
}

unsigned char translate(unsigned char c){
	if(isalpha(c)){
		if(c>='n'){
			c-=13;
		}else if(c>='a'){
			c+=13;
		}else if(c>='N')
			c-=13;
		else
			c+=13;
	}
	return c;
}

void set_fl(int fd,int flags){
	int val;
	if((val=fcntl(fd,F_GETFL,0))<0){
		printf("fcntl error\n");exit(-1);
	}
	val|=flags;
	if(fcntl(fd,F_SETFL,val)<0){
		printf("fcntl error\n");exit(-2);
	}
}

void clr_fl(int fd,int flags){
	int val;
	if((val=fcntl(fd,F_GETFL,0))<0){
		printf("fcntl error\n");exit(-3);
	}
	val&=~flags;
	if(fcntl(fd,F_SETFL,val)<0){
		printf("fcntl error\n");exit(-4);
	}
}

int lock_reg(int fd,int cmd,int type,off_t off,int whence,off_t len){
	struct flock lock;
	lock.l_type=type;
	lock.l_start=off;
	lock.l_whence=whence;
	lock.l_len=len;
	return fcntl(fd,cmd,&lock);
}

pid_t lock_test(int fd,int type,off_t off,int whence,off_t len){
	struct flock lock;
	lock.l_type=type;
	lock.l_start=off;
	lock.l_whence=whence;
	lock.l_len=len;
	if(fcntl(fd,F_GETLK,&lock)<0){
		printf("fcntl error\n");exit(-1);
	}
	if(lock.l_type==F_UNLCK){
		return 0;
	}
	return lock.l_pid; 
}

int lockfile(int fd){
	struct flock lock;
	lock.l_type=F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=0;
	lock.l_len=0;
	return fcntl(fd,F_SETLKW,&lock);
}
