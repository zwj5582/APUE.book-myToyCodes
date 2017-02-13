/*************************************************************************
	> File Name: rand.h
	> Author: zhongwenjie
	> Mail: zwj5582@gmail.com 
	> Created Time: Sun 21 Aug 2016 05:51:53 PM PDT
 ************************************************************************/

#ifndef RAND_H
#define RAND_H

unsigned int next=1;

int myrand(void){
	next=next*1103515245+123456;
	return (unsigned int)(next/65536)%32768;
}

void mysrand(unsigned int seek){
	next=seek;
}

#endif
