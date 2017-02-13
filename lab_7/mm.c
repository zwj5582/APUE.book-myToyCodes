/*************************************************************************
	> File Name: mm.c
	> Author: zhongwenjie
	> Mail: zwj5582@gmail.com 
	> Created Time: Thu 18 Aug 2016 12:11:43 AM PDT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include"mm.h"
#include"memlib.h"

void* coalesce(void* p){
	size_t prev_alloc=GET_ALLOC(HDRP(PREV_BLKP(p)));
	size_t next_alloc=GET_ALLOC(HDRP(NEXT_BLKP(p)));
	size_t size=GET_SIZE(HDRP(p));
	if(prev_alloc&&next_alloc)
		return p;
	else if(!prev_alloc&&next_alloc){
		size+=GET_SIZE(HDRP(PREV_BLKP(p)));
		PUT(HDRP(PREV_BLKP(p)),PACK(size,0));
		PUT(FTRP(p),PACK(size,0));
		return p=PREV_BLKP(p);
	}else if(prev_alloc&&!next_alloc){
		size+=GET_SIZE(HDRP(NEXT_BLKP(p)));
		PUT(HDRP(p),PACK(size,0));
		//PUT(FTRP(NEXT_BLKP(p)),PACK(size,0));
		PUT(FTRP(p),PACK(size,0));
		return p;
	}else{
		size+=(GET_SIZE(HDRP(PREV_BLKP(p)))+GET_SIZE(HDRP(NEXT_BLKP(p))));
		PUT(HDRP(PREV_BLKP(p)),PACK(size,0));
		PUT(FTRP(NEXT_BLKP(p)),PACK(size,0));
		return p=PREV_BLKP(p);
	}
}

void* extend_heap(size_t words){
	size_t size;
	char* bp;
	size=((words%2) ? (words+1)*WSIZE : words*WSIZE);
	printf("lllllllllllll\n");
	if((long)(bp=mem_sbrk(size))==-1)
		return NULL;
	printf("kkkkkkkkkk\n");
	PUT(HDRP(bp),PACK(size,0));
	PUT(FTRP(bp),PACK(size,0));
	PUT(HDRP(NEXT_BLKP(bp)),PACK(0,1));
	return coalesce(bp);
}

int mm_init(void){
	if((heap_listp=mem_sbrk(4*WSIZE))==(void*)-1)
		return -1;
	PUT(heap_listp,0);
	PUT(heap_listp+(1*WSIZE),PACK(DSIZE,1));
	PUT(heap_listp+(2*WSIZE),PACK(DSIZE,1));
	PUT(heap_listp+(3*WSIZE),PACK(0,1));
	heap_listp+=(2*WSIZE);
	if(extend_heap(CHUNKSIZE/WSIZE)==NULL)
		return -1;
	return 0;
}

void mm_free(void* bp){
	size_t size=GET_SIZE(HDRP(bp));
	PUT(HDRP(bp),PACK(size,0));
	PUT(FTRP(bp),PACK(size,0));
	coalesce(bp);
}

void* find_fit(size_t size){
	size_t rsize;
	void* ptr=heap_listp;
	while((rsize=GET_SIZE(HDRP(ptr)))!=0){
		if(!GET_ALLOC(HDRP(ptr))&&rsize>=size)
			return ptr;
		ptr=NEXT_BLKP(ptr);
	}
	return NULL;
}

void place(void* bp,size_t size){
	size_t rsize=GET_SIZE(HDRP(bp));
	if((rsize-size)>=(2*DSIZE)){
		PUT(HDRP(bp),PACK(size,1));
		PUT(FTRP(bp),PACK(size,1));
		bp=NEXT_BLKP(bp);
		PUT(HDRP(bp),PACK(rsize-size,0));
		PUT(FTRP(bp),PACK(rsize-size,0));
	}else{
		PUT(HDRP(bp),PACK(rsize,1));
		PUT(FTRP(bp),PACK(rsize,1));
	}
}

void* mm_malloc(size_t size){
	void* bp;
	size_t rsize;
	size_t extendsize;
	if(size<=0)
		return NULL;
	if(size<=DSIZE)
		rsize=2*DSIZE;
	else
		rsize=DSIZE*((size+DSIZE+DSIZE-1)/DSIZE);
	if((bp=find_fit(rsize))!=NULL){
		place(bp,rsize);
		return bp;
	}
	extendsize=MAX(rsize,CHUNKSIZE);
	if((bp=extend_heap(extendsize/WSIZE))==NULL)
		return NULL;
	place(bp,rsize);
	return bp;
}
