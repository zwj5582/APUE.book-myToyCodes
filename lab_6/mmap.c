/*************************************************************************
	> File Name: mmap.c
	> Author: zhongwenjie
	> Mail: zwj5582@gmail.com 
	> Created Time: Wed 17 Aug 2016 07:16:14 PM PDT
 ************************************************************************/

#include<stdio.h>
#include<sys/mman.h>
#include<unistd.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<fcntl.h>

int main(int argc,char** argv){
	int fdout;
	void* addr;
	struct stat statbuf;
	if((fdout=open(argv[1],O_RDONLY))<0){
		perror("open error\n");exit(-1);
	}
	if((fstat(fdout,&statbuf))<0){
		perror("stat error\n");exit(-1);
	}
	if((addr=mmap(0,statbuf.st_size,PROT_READ,MAP_PRIVATE,fdout,0))==MAP_FAILED){
		perror("mmap error\n");exit(-1);
	}
	write(STDOUT_FILENO,addr,statbuf.st_size);
}
