/*************************************************************************
	> File Name: test2.c
	> Author: zhongwenjie
	> Mail: zwj5582@gmail.com 
	> Created Time: Sun 07 Aug 2016 03:16:26 AM PDT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<stddef.h>
#include<time.h>
#include<sys/time.h>
#include<pthread.h>

struct to_info{
	void (*to_fn)(void*);
	void *to_arg;
	struct timespec to_wait;
};

int makepthread(void* (*fn)(void*),void* arg){
	printf("ooooooooooooooo\n");
	pthread_attr_t attr;
	int err;
	pthread_t tid;
	err=pthread_attr_init(&attr);
	if(err!=0)
		return err;
	err=pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
	if(err==0)
		err=pthread_create(&tid,&attr,fn,arg);
	sleep(50);
	if(err!=0)
		printf("yyyyyyyyyy\n");
	pthread_attr_destroy(&attr);
	return err;
}

#define SECTONSEC 1000000000

void* timeout_helper(void* arg){
	printf("..............\n");
	struct to_info *tip;
	tip=(struct to_info*)arg;
	clock_nanosleep(CLOCK_REALTIME,0,&tip->to_wait,NULL);
	printf("zzzzzzzzzzzzzz\n");
	(*tip->to_fn)(tip->to_arg);
	printf("pppppppppppppppppppppppppppppp\n");
	free(arg);
	return 0;
}

void timeout(const struct timespec *when,void (*func)(void*),void* arg){
	printf("jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjlll\n");
	struct timespec now;
	struct to_info *tip;
	int err;
	clock_gettime(CLOCK_REALTIME,&now);
	if((when->tv_sec>now.tv_sec)||(when->tv_sec==now.tv_sec&&when->tv_nsec>now.tv_nsec)){
		tip=malloc(sizeof(struct to_info));
		if(tip==NULL)
			printf("jjjjjjjjjjjj");
		tip->to_wait.tv_sec=when->tv_sec-now.tv_sec;
		if(when->tv_nsec>=now.tv_nsec){
			tip->to_wait.tv_nsec=when->tv_nsec-now.tv_nsec;
		}else{
			tip->to_wait.tv_sec--;
			tip->to_wait.tv_nsec=SECTONSEC-now.tv_nsec+when->tv_nsec;
		}
		tip->to_fn=func;
		tip->to_arg=arg;
		err=makepthread(timeout_helper,(void*)tip);
		if(err==0)
			return;
		else
			free(tip);
	}
	(*func)(arg);
}

pthread_mutexattr_t attr;

pthread_mutex_t mutex;

void retry(void* arg){
	pthread_mutex_lock(&mutex);
	printf("ooooooooooooooooooppppppppppppppppppppppppp\n");
	pthread_mutex_unlock(&mutex);
}

int main(){
	int err,arg;
	struct timespec when;
	setbuf(stdout,NULL);
	if((err=pthread_mutexattr_init(&attr))!=0){
		printf("pthread_mutexattr_init error\n");exit(-1);
	}
	if((err=pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE))!=0){
		printf("pthread_mutexattr_gettype error\n");exit(-2);
	}
	if((err=pthread_mutex_init(&mutex,&attr))!=0){
		printf("pthread_mutex_init error\n");exit(-3);
	}
	pthread_mutex_lock(&mutex);
	clock_gettime(CLOCK_REALTIME,&when);
	when.tv_sec+=10;
	timeout(&when,retry,(void*)((unsigned long)arg));
	//sleep(20);
	pthread_mutex_unlock(&mutex);
	exit(0);
}
