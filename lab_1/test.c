/*************************************************************************
	> File Name: test.c
	> Author: zhongwenjie
	> Mail: zwj5582@gmail.com 
	> Created Time: Thu 04 Aug 2016 06:51:35 AM PDT
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

#define BUFFERSIZE 4096

#define MAXSTRSZ 4096

#define NHASH 29

#define hash(id) ((unsigned long)id/NHASH)

extern char** environ;

pthread_mutex_t env_mutex;

static pthread_once_t init_done=PTHREAD_ONCE_INIT;

static pthread_key_t key;

pthread_mutex_t env_lock;

static void pthread_init(void);

struct foo{
	int f_count;
	pthread_mutex_t f_lock;
	int f_id;
	struct foo* next;
};

struct foo *fh[NHASH];

//pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t hashlock=PTHREAD_MUTEX_INITIALIZER;

struct foo* init_foo(int id);

void foo_hold(struct foo* fp);

void foo_rele(struct foo* fp);

void* myaction(void*);

void* thr_fn1(void*);

void cleanup(void*);

static char* my_getenv2(const char*);

int my_getenv(const char*,char*,int);

void* thr_fn2(void*);

void* pth_fn1(void*);

void* pth_fn2(void*);

void printfoo(const char*,const struct foo*);

void mylock();

void* fn1(void*);

void* fn2(void*);

void cleantest(void*);

int main(int argc,char** argv){
	/*void* tret;
	pthread_t tid1,tid2;
	int err;
	err=pthread_create(&tid1,NULL,thr_fn1,NULL);
	if(err!=0){
		printf("pthread_create error\n");exit(-1);
	}
	err=pthread_join(tid1,&tret);
	if(err!=0){
		printf("pthread_join error\n");exit(-3);
	}
	printf("pthread 1 exit code %ld\n",(long)tret);
	err=pthread_create(&tid2,NULL,thr_fn2,NULL);
	if(err!=0){
		printf("pthread_create error\n");exit(-2);
	}
	err=pthread_join(tid2,&tret);
	if(err!=0){
		printf("pthread_join error\n");exit(-4);
	}
	printf("pthread 2 exit code %ld\n",(long)tret);
	exit(0);*/

	/*int err;
	pthread_t tid1,tid2;
	struct foo* fp;
	void* re;
	err=pthread_create(&tid1,NULL,pth_fn1,NULL);
	if(err!=0){
		printf("cant create new thread\n");exit(-1);
	}
	err=pthread_join(tid1,(void*)&fp);*/
	/*if(err!=0){
		printf("pthread_join error\n");exit(-2);
	}
	sleep(1);
	printf("parent starting create second pthread\n");
	err=pthread_create(&tid2,NULL,pth_fn2,NULL);
	if(err!=0){
		printf("cant create new thread\n");exit(-1);
	}
	err=pthread_join(tid2,&re);
	if(err!=0){
		printf("pthread_join error\n");exit(-3);
	}
	printf("pthread 2 exit code : %ld\n",re);
	sleep(1);*/
	//printfoo("parent",fp);
	//exit(0);
	


	/*int err;
	void* tret;
	pthread_t tid1,tid2;
	err=pthread_create(&tid1,NULL,fn1,(void*)1);
	if(err!=0){
		printf("pthread_create error\n");exit(-1);
	}
	err=pthread_create(&tid2,NULL,fn2,(void*)1);
	if(err!=0){
		printf("pthread_create error\n");exit(-2);
	}
	err=pthread_join(tid1,&tret);
	if(err!=0){
		printf("pthread_join error\n");exit(-3);
	}
	printf("pthread 1 exit code %ld\n",(long)tret);
	err=pthread_join(tid2,&tret);
	if(err!=0){
		printf("pthread_join error\n");
	}
	printf("pthread 2 exit code %ld\n",(long)tret);
	exit(0);*/

	//pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;
	//pthread_mutex_lock(&lock);
	//printf("fisrt locking ...\n");
	//pthread_mutex_lock(&lock);
	//printf("second locking ...\n");
	//pthread_mutex_unlock(&lock);
	//mylock();
	//pthread_mutex_unlock(&lock);
	//exit(0);
	


	/*pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;
	int err;
	struct timespec times;
	struct tm* tmp;
	char buf[64];
	pthread_mutex_lock(&lock);
	printf("locking ...\n");
	clock_gettime(CLOCK_REALTIME,&times);
	tmp=localtime(&times.tv_sec);
	strftime(buf,sizeof(buf),"%r",tmp);
	printf("current time : %s\n",buf);
	times.tv_sec+=10;
	err=pthread_mutex_timedlock(&lock,&times);
	clock_gettime(CLOCK_REALTIME,&times);
	tmp=localtime(&times.tv_sec);
	strftime(buf,sizeof(buf),"%r",tmp);
	printf("current time : %s\n",buf);
	if(err!=0){
		printf("can't lock mutex\n");
	}else{
		printf("mutex again\n");
	}
	exit(0);*/


	//char* buf;
	//buf=my_getenv2("USER");
	//my_getenv("USER",buf,BUFFERSIZE);
	//printf("%s\n",buf);
	

	int state,oldstate,err;
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
	err=pthread_join(tid,&tret);
	sleep(2);
	printf("code : %ld\n",(unsigned long)tret);
	sleep(1);
	exit(0);
}

void cleanup(void* arg){
	printf("in cleanup\n");
}

void* myaction(void* arg){
	printf("in myaction\n");
	pthread_cleanup_push(cleanup,NULL);
	//pthread_cleanup_pop(0);
	pthread_exit((void*)1);
}

/*void mylock(){
	pthread_mutex_lock(&lock);
	printf("second locking ...\n");
	pthread_mutex_unlock(&lock);
}*/

/*void cleantest(void* arg){
	printf("clean : %s\n",arg);
}*/

/*void pthread_init(void){
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&env_lock,&attr);
	pthread_mutexattr_destroy(&attr);
	pthread_key_create(&key,free);
}*/

/*char* my_getenv2(const char* name){
	int i,len;
	pthread_once(&init_done,pthread_init);
	pthread_mutex_lock(&env_lock);
	char* str=(char*)pthread_getspecific(key);
	if(str==NULL){
		str=(char*)malloc(MAXSTRSZ);
		if(str==NULL){
			pthread_mutex_unlock(&env_lock);
			return NULL;
		}
		pthread_setspecific(key,str);
	}
	len=strlen(name);
	for(i=0;environ[i]!=NULL;i++){
		if(strncmp(environ[i],name,len)==0&&environ[i][len]=='='){
			strncpy(str,&environ[i][len+1],MAXSTRSZ-1);
			pthread_mutex_unlock(&env_lock);
			return str;
		}
	}
	pthread_mutex_unlock(&env_lock);
	return NULL;
}

int my_getenv(const char* name,char* buf,int buffersize){
	int i,len,olen;
	pthread_once(&init_done,pthread_init);
	len=strlen(name);
	pthread_mutex_lock(&env_mutex);
	for(i=0;environ[i]!=NULL;i++){
		if(strncmp(environ[i],name,len)==0&&environ[i][len]=='='){
			olen=strlen(&environ[i][len]+1);
			if(olen>=buffersize){
				pthread_mutex_unlock(&env_mutex);
				return ENOSPC;
			}
			strcpy(buf,&environ[i][len+1]);
			pthread_mutex_unlock(&env_mutex);
			return 0;
		}
	}
	pthread_mutex_unlock(&env_mutex);
	return ENOENT;
}

struct foo* init_foo(int id){
	struct foo* fp;
	unsigned long idx;
	if((fp=malloc(sizeof(struct foo)))!=NULL){
		fp->f_count=1;
		fp->f_id=id;
		if(pthread_mutex_init(&fp->f_lock,NULL)!=0){
			free(fp);
			return NULL;
		}
		pthread_mutex_lock(&hashlock);
		idx=hash(id);
		fh[idx]=fp->next;
		fh[idx]=fp;
		pthread_mutex_lock(&fp->f_lock);
		pthread_mutex_unlock(&hashlock);
		pthread_mutex_unlock(&fp->f_lock);
	}
	return fp;
}

void foo_hold(struct foo* fp){
	pthread_mutex_lock(&fp->f_lock);
	fp->f_count++;
	pthread_mutex_unlock(&fp->f_lock);
}

struct foo* find(int id){
	struct foo* fp;
	pthread_mutex_lock(&hashlock);
	for(fp=fh[id];fp!=NULL;fp=fp->next){
		if(fp->f_id==id){
			foo_hold(fp);
			break;
		}
	}
	pthread_mutex_unlock(&hashlock);
	return fp;
}

void foo_rele(struct foo* fp){
	int idx;
	struct foo* tfp;
	pthread_mutex_lock(&fp->f_lock);
	if(fp->f_count==1){
		pthread_mutex_unlock(&fp->f_lock);
		pthread_mutex_lock(&hashlock);
		pthread_mutex_lock(&fp->f_lock);
		if(fp->f_count!=1){
			fp->f_count--;
			pthread_mutex_unlock(&fp->f_lock);
			pthread_mutex_unlock(&hashlock);
		}
		idx=hash(fp->f_id);
		tfp=fh[idx];
		if(tfp==fp){
			fh[idx]=fp->next;
		}else{
			while(tfp->next!=fp)
				tfp=tfp->next;
			tfp->next=fp->next;
		}
		pthread_mutex_unlock(&hashlock);
		pthread_mutex_unlock(&fp->f_lock);
		pthread_mutex_destroy(&fp->f_lock);
	}else{
		fp->f_count--;
		pthread_mutex_unlock(&fp->f_lock);
	}
}

void* fn1(void* arg){
	printf("pthread 1 starting...\n");
	pthread_cleanup_push(cleantest,"pthread 1 first clean");
	pthread_cleanup_push(cleantest,"pthread 1 second clean");
	printf("pthread push...\n");
	if(arg)
		pthread_exit((void*)1);
	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);
	return (void*)1;
}

void* fn2(void* arg){
	printf("pthread 2 starting...\n");
	pthread_cleanup_push(cleantest,"pthread 2 first clean");
	pthread_cleanup_push(cleantest,"pthread 2 second clean");
	printf("pthread push...\n");
	if(arg)
		pthread_exit((void*)2);
	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);
	pthread_exit((void*)2);
}

/*void printfoo(const char* str,const struct foo* fp){
	printf("%s :\n",str);
	printf("struct foo at (0x%lx)\n",(unsigned long)fp);
	printf("foo a = %d\n",fp->a);
	printf("foo b = %d\n",fp->b);
	printf("foo c = %d\n",fp->c);
	printf("foo d = %d\n",fp->d);
}

void* pth_fn1(void* arg){
	struct foo foo={1,2,3,4};
	printfoo("pthread 1",&foo);
	pthread_exit((void*)&foo);
}*/

/*void* pth_fn2(void* arg){
	printf("pthread 2 id : %ld\n",(unsigned long)pthread_self());
	return (void*)34;
}

void* thr_fn1(void* arg){
	printf("pthread 1 exit\n");
	return (void*)1;
}

void* thr_fn2(void* arg){
	printf("pthread 2 exit\n");
	return (void*)2;
}*/
