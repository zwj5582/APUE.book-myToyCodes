/*************************************************************************
	> File Name: fork.cpp
	> Author: zhongwenjie
	> Mail: zwj5582@gmail.com 
	> Created Time: Thu 14 Jul 2016 12:53:36 AM PDT
 ************************************************************************/

#include<iostream>
#include<unistd.h>
#include<sys/types.h>
using namespace std;

#define err_sys(info){cout<<"info"<<endl;exit(-1);}

int glob=6;

char buf[]="a write_out";

int main(int argc,char** argv){
	int var;
	pid_t pid;
	var=88;
	if(write(STDOUT_FILENO,buf,sizeof(buf)-1)!=sizeof(buf)-1){
		err_sys(STDOUT_ERROR);
	}
	cout<<"fork before"<<endl;
	if((pid=fork())<0){
		err_sys(FORK_ERR);
	}
	if(pid==0){
		glob++;
		var++;
	}
	else{
		sleep(2);
	}
	cout<<getpid()<<"\t"<<getppid()<<"\t"<<glob<<"\t"<<var<<endl;
}
