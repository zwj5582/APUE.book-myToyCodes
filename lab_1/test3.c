/*************************************************************************
	> File Name: test3.c
	> Author: zhongwenjie
	> Mail: zwj5582@gmail.com 
	> Created Time: Sun 07 Aug 2016 08:33:00 PM PDT
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

#define LOCKFILE "/var/run/daemon.pid"

#define LOCKMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

sigset_t mask;

extern int lcokfile(int);

extern char** environ;

void cleanup(void*);

void* myaction(void*);

void* th_fn(void*);

void* fn(void*);

void prepare(void);

void parent(void);

void child(void);

int already_runing(void);

int quitflag;

//sigset_t mask;

pthread_mutex_t my_lock=PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t my_mutex;

pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t waitloc=PTHREAD_COND_INITIALIZER;

pthread_once_t init_done=PTHREAD_ONCE_INIT;

int main(int argc,char** argv){
	/*int state,oldstate,err;
	void* tret;
	pthread_t tid;
	state=PTHREAD_CANCEL_ENABLE;
	if((pthread_setcancelstate(state,&oldstate))!=0){
		printf("pthread_cancel error\n");exit(-1);
	}
	if((pthread_create(&tid,NULL,myaction,NULL))!=0){
		printf("pthread_create error\n");exit(-2);
	}
	if((pthread_cancel(tid))!=0){
		printf("pthread_cancel error\n");exit(-3);
	}
	printf("in main\n");
	sleep(10);
	//pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,&oldstate);
	err=pthread_join(tid,&tret);
	sleep(2);
	printf("code : %ld\n",(unsigned long)tret);
	sleep(1);
	exit(0);*/


	/*sigset_t oldmask;
	pthread_t tid;
	int err;
	sigemptyset(&mask);
	sigaddset(&mask,SIGINT);
	sigaddset(&mask,SIGQUIT);
	if((err=pthread_sigmask(SIG_BLOCK,&mask,&oldmask))!=0){
		printf("pthread_sigmask error\n");exit(-2);
	}
	err=pthread_create(&tid,NULL,th_fn,NULL);
	if(err!=0){
		printf("pthread_create error\n");exit(-3);
	}
	pthread_mutex_lock(&lock);
	//while(quitflag==0){
		pthread_cond_wait(&waitloc,&lock);
	//}
	pthread_mutex_unlock(&lock);
	quitflag=0;
	sigprocmask(SIG_SETMASK,&oldmask,NULL);
	exit(0);*/



	/*int err;
	pid_t pid;
	pthread_t tid;
	setbuf(stdout,NULL);
	err=pthread_atfork(prepare,parent,child);
	if(err!=0){
		printf("pthread_atfork error\n");exit(-1);
	}
	err=pthread_create(&tid,NULL,fn,NULL);
	if(err!=0){
		printf("pthread_create error\n");exit(-2);
	}
	sleep(2);
	printf("parent fork about\n");
	if((pid=fork())<0){
		printf("fork error\n");exit(-3);
	}else if(pid==0){
		printf("child return\n");
	}else{
		sleep(2);
		printf("parent return\n");
	}
	exit(0);*/


	struct sigaction sa;
	int err;
	char* cmd;
	pthread_t tid;
	if((cmd=strrchr(argv[0],'/'))==NULL){
		cmd=argv[0];
	}else{
		cmd++;
	}
	daemonize(cmd);
	if(already_runing()){
		syslog(LOG_ERR,"alreay_running");
		exit(1);
	}
	sa.sa_handler=SIG_DFL;
	sa.sa_flags=0;
	sigemptyset(&sa.sa_mask);
	if((sigaction(SIGHUP,&sa,NULL))<0){
		printf("sigaction error\n");exit(-1);
	}
	sigfillset(&mask);
	if(pthread_sigmask(SIG_BLOCK,&mask,NULL)!=0){
		printf("pthread_sigmask error\n");exit(-2);
	}
	err=pthread_create(&tid,NULL,pthread_fn,NULL);
	if(err!=0){
		printf("pthread_create error\n");exit(-3);
	}
	exit(0);
}

int already_runing(void){
	int fd;
	char buf[16];
	if((fd=open(LOCKFILE,O_RDWR|O_CREAT,FILEMODE))<0){
		syslog(LOG_ERR,"can't open %s %s",LOCKFILE,strerror(errno));
		exit(1);
	}
	if(lockfile(fd)<0){
		if(errno==EACCES||ERRNO==EAGAIN){
			close(fd);
			return 1;
		}
		syslog(LOG_ERR,"can't lock %s %s",LOCKFILE,strerror(errno));
		exit(1);
	}
	ftruncate(fd,0);
	sprintf(buf,"%ld",(long)getpid());
	write(fd,buf,strlen(buf)+1);
	return 0;
}

void* pthread_fn(void* arg){
	int err,singo;
	while(1){
		err=sigwait(&mask,&signo);
		if(err!=0){
			printf("sigwait error\n");exit(-1);
		}
		switch(signo){
			case SIGHUP:
				syslog(LOG_INFO,"re-reading configuration file");
				r_read();
				break;
			case SIGTERM:
				syslog(LOG_INFO,"get SIGTERM,exiting");
				exit(0);
			default:
				syslog(LOG_INFO."unexpected signal %d\n",signo);
		}
	}
	return 0;
}

void pthread_init(void){
	int err;
	pthread_mutexattr_t attr;
	err=pthread_mutexattr_init(&attr);
	if(err!=0){
		printf("pthread_mutexattr_init error\n");exit(-1);
	}
	err=pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);
	if(err!=0){
		printf("pthread_mutexattr_settype error\n");exit(-2);
	}
	err=pthread_mutex_init(&my_mutex,&attr);
	if(err1=0){
		printf("pthread_mutex_init error\n");exit(-3);
	}
	pthread_mutexattr_destroy(&attr);
}

void daemonize(const char* cmd){
	int fd0,fd1,fd2,i;
	pid_t pid;
	struct rlimit rl;
	struct sigaction sa;
	umask(0);
	if(getrlimit(RLIMIT_NOFILE,&rl)<0){
		printf("getrlimit error\n");exit(-1);
	}
	if((pid=fork())<0){
		printf("fork error\n");exit(-2);
	}else if(pid!=0){
		exit(0);
	}
	setsid();
	sa.sa_handler=SIG_IGN;
	sa.sa_flags=0;
	sigemptyset(&sa.sa_mask);
	if((sigaction(SIGHUP,&sa,NULL))<0){
		printf("sigaction error\n");exit(-3);
	}
	if((pid=fork())<0){
		printf("fork error\n");exit(-4);
	}else if(pid!=0){
		exit(0);
	}
	if(chdir("/")<0){
		printf("chdir error\n");exit(-5);
	}
	if(rl.rlimit_max==RLIM_INFINITY){
		rl.rlimit=1024;
	}
	for(i=0;i<rl.rlimit;i++){
		close(i);
	}
	fd0=open("/dev/null",O_RDWR);
	fd1=due(0);
	fd2=due(0);
	openlog(cmd,LOG_CONS,LOG_DAEMON);
	if(fd0!=0||fd1!=1||fd2!=2){
		syslog(LOG_ERR,"unexpected file descriptors %d %d%d",fd0,fd1,fd2);
		exit(1);
	}
}

int my_putevn(const char* namevale){
	int err,i;
	
	pthread_once(&init_done,pthread_init);
	pthread_mutex_lock(&my_mutex);
	for(i=0;environ[i]!=NULL;i++);
	environ[i]=namevale;
	environ[i+1]=NULL;`
	pthread_mutex_unlock(&my_mutex);
}

void* fn(void* arg){
	printf("in fn\n");
}

void prepare(void){
	int err;
	printf("in prepare\n");
	err=pthread_mutex_lock(&my_lock);
	if(err!=0){
		printf("pthread_mutex_lock error\n");
	}
}

void parent(void){
	int err;
	printf("in parent\n");
	err=pthread_mutex_unlock(&my_lock);
	if(err!=0){
		printf("pthread_mutex_unlock error\n");
	}
}

void child(void){
	int err;
	printf("in child\n");
	err=pthread_mutex_unlock(&my_lock);
	if(err!=0){
		printf("pthread_mutex_unlock error\n");
	}
}

void* th_fn(void* arg){
	int err,signo;
	//while(1){
		err=sigwait(&mask,&signo);
		if(err!=0){
			printf("sigwait error\n");exit(-1);
		}
		switch(signo){
			case SIGINT:
				printf("\ninter\n");
				pthread_cond_signal(&waitloc);
				break;
			case SIGQUIT:
				pthread_mutex_lock(&lock);
				quitflag=1;
				pthread_mutex_unlock(&lock);
				pthread_cond_signal(&waitloc);
				return (void*)0;
			default:
				printf("\nno signo\n");
				exit(1);
		}
	//}
}

void cleanup(void* arg){
	printf("in cleanup\n");
}

void* myaction(void* arg){
	printf("in myaction\n");
	pthread_cleanup_push(cleanup,NULL);
	pthread_cleanup_pop(1);
	pthread_exit((void*)1);
}
