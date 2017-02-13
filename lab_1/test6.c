/*************************************************************************
	> File Name: test6.c
	> Author: zhongwenjie
	> Mail: zwj5582@gmail.com 
	> Created Time: Thu 11 Aug 2016 10:38:32 PM PDT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<inttypes.h>

int main(){
	uint32_t i=0x04030201;
	unsigned char* cp=(unsigned char*)&i;
	if(*cp==1){
		printf("little_type\n");
	}else if(*cp==4)
		printf("big_type\n");
	else
		printf("unkown\n");
	exit(0);
}
