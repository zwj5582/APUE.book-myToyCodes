/*************************************************************************
	> File Name: wait.cpp
	> Author: zhongwenjie
	> Mail: zwj5582@gmail.com 
	> Created Time: Thu 14 Jul 2016 03:40:55 AM PDT
 ************************************************************************/

#include<iostream>
#include<wait.h>
#include<sys/types.h>
#include<unistd.h>
using namespace std;
void pr_exit(int status);

int main(){
	int pid,status;
	if((pid=fork())<0){
		perror("fork");exit(-1);
	}
	if(pid==0){
		sleep(2);
		cout<<"in child"<<endl;
		exit(100);
	}
	if(wait(&status)!=pid){
		perror("wait");exit(-2);
	}
	cout<<"in parent"<<endl;
	pr_exit(status);
	if((pid=fork())<0){
		perror("fork");exit(-1);
	}
	if(pid==0){
		abort();
	}
	if(wait(&status)!=pid){
		perror("wait");exit(-2);
	}
	pr_exit(status);
	if((pid=fork())<0){
		perror("fork");exit(-1);
	}
	if(pid==0){
		status/=0;
	}
	pr_exit(status);
	exit (0);
}

void pr_exit(int status){
	if(WIFEXITED(status)){
		cout<<"y..."<<WEXITSTATUS(status)<<endl;
	}
	else if(WIFSIGNALED(status)){
		cout<<"n..."<<WTERMSIG(status)<<endl;
	}
}
