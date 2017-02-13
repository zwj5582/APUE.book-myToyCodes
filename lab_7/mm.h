/*************************************************************************
	> File Name: mm.h
	> Author: zhongwenjie
	> Mail: zwj5582@gmail.com 
	> Created Time: Thu 18 Aug 2016 03:19:58 AM PDT
 ************************************************************************/

#ifndef MM_H
#define MM_H

#define WSIZE 4

#define DSIZE 8

#define CHUNKSIZE (1<<12)

#define MAX(x,y) ((x)>(y) ? (x) : (y) )

#define PACK(size,alloc) ((size)|(alloc))

#define GET(p)(*(unsigned int*)(p))

#define PUT(p,val) (*(unsigned int*)(p)=(val))

#define GET_SIZE(p) (GET(p) & ~0x7)

#define GET_ALLOC(p) (GET(p) & 0x1)

#define HDRP(bp) ((char*)(bp)-WSIZE)

#define FTRP(bp) ((char*)(bp)+GET_SIZE(HDRP(bp))-DSIZE)

#define NEXT_BLKP(bp) ((char*)(bp)+GET_SIZE(HDRP(bp)))

#define PREV_BLKP(bp) ((char*)(bp)-GET_SIZE((char*)(bp)-DSIZE))

static void* heap_listp;

static void* coalesce(void*);

static void* extend_heap(size_t);

int mm_init(void);

void mm_free(void*);

static void* find_fit(size_t);

static void place(void* bp,size_t size);

void* mm_malloc(size_t size);

#endif
