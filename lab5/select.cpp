/*************************************************************************
	> File Name: select.cpp
	> Author: zhongwenjie
	> Mail: zwj5582@gmail.com 
	> Created Time: Wed 13 Jul 2016 08:26:35 PM PDT
 ************************************************************************/

#include<iostream>
#include<sys/select.h>
#include<unistd.h>
#include<fcntl.h> 
using namespace std;

#define MAX_NUM 100
#define OPEN_FILE "/dev/input/mice"

int main(){
	char buf[MAX_NUM];
	fd_set rfds;
	int fd,selfd;
	fd=open(OPEN_FILE,O_RDONLY);
	FD_ZERO(&rfds);
	FD_SET(0,&rfds);
	FD_SET(fd,&rfds);
	struct timeval tv;
	tv.tv_sec=5;
	tv.tv_usec=0;
	selfd=select(fd+1,&rfds,NULL,NULL,&tv);
	if(selfd<0){
		cout<<"error"<<endl;
	}
	else if(selfd==0){
		cout<<"more 5s"<<endl;
	}
	else{
		if(FD_ISSET(0,&rfds)){
			cout<<"key"<<endl;
			read(0,buf,MAX_NUM);
		}
		if(FD_ISSET(fd,&rfds)){
			cout<<"mice"<<endl;
			read(fd,buf,MAX_NUM);
		}
	}
	return 0;
}

