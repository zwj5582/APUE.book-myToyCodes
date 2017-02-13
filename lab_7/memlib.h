/*************************************************************************
	> File Name: memlib.h
	> Author: zhongwenjie
	> Mail: zwj5582@gmail.com 
	> Created Time: Wed 17 Aug 2016 10:47:55 PM PDT
 ************************************************************************/

#ifndef MEMLIB_H
#define MEMLIB_H

#define MAX_HEAP 1024*1024

static char* mem_heap;

static char* mem_brk;

static char* mem_max_addr;

void  mem_init(void);

void* mem_sbrk(int);

#endif
