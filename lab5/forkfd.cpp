/*************************************************************************
	> File Name: forkfd.cpp
	> Author: zhongwenjie
	> Mail: zwj5582@gmail.com 
	> Created Time: Thu 14 Jul 2016 01:43:18 AM PDT
 ************************************************************************/

#include<iostream>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
using namespace std;

//#define err_sys(info) {cout<<"info"<<endl;exit(-1);}

int main(){
	int fd,pid;
	char buf[10];
	if((fd=open("./test",O_RDONLY))<0){
		perror("open");  exit(-1);
	}
	if((pid=fork())<0){
		exit(0);
		perror("fork");  exit(-1);
	}
	else if(pid==0){
		read(fd,buf,2);
		write(STDOUT_FILENO,buf,2);
	}
	else{
		sleep(2);
		//read(fd,buf,2);
		lseek(fd,SEEK_CUR,2);
		read(fd,buf,2);
		//read(fd,buf,2);
		write(STDOUT_FILENO,buf,2);
        write(STDOUT_FILENO, "\n", 1);
	}
	return 0;
}

