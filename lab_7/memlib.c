/*************************************************************************
	> File Name: memlib.c
	> Author: zhongwenjie
	> Mail: zwj5582@gmail.com 
	> Created Time: Wed 17 Aug 2016 10:51:46 PM PDT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include"memlib.h"

void mem_init(void){
	mem_heap=(char*)malloc(MAX_HEAP);
	if(!mem_heap)
		printf("mmmmmmmmm\n");
	mem_brk=(char*)mem_heap;
	mem_max_addr=(char*)(mem_heap+MAX_HEAP);
}

void* mem_sbrk(int incr){
	char* old_brk=mem_brk;
	if(incr<0||(incr+mem_brk)>mem_max_addr){
		errno=ENOMEM;
		//fprintf(stderr,"mem_sbrk error\n");
		return (void*)-1;
	}
	mem_brk+=incr;
	return (void*)old_brk;
}
