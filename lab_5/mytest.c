/*************************************************************************
	> File Name: mytest.c
	> Author: zhongwenjie
	> Mail: zwj5582@gmail.com 
	> Created Time: Tue 16 Aug 2016 09:07:17 PM PDT
 ************************************************************************/

#include<stdio.h>
#include"test.h"
#include<dlfcn.h>
#include<stdlib.h>

int main(){
	void (*add_1)(int*);
	void* handle;
	void* err;
	int a=1;
	handle=dlopen("/home/zhongwenjie/codes/lab_5/my.so",RTLD_LAZY);
	if(!handle){
		fprintf(stderr,"%s\n",dlerror());exit(-1);
	}
	add_1=dlsym(handle,"addadd");
	if((err=dlerror())!=NULL){
		fprintf(stderr,"%s\n",err);exit(-1);
	}
	add_1(&a);
	printf("%d\n",a);
	if((dlclose(handle))<0){
		fprintf(stderr,"%s\n",dlerror());
	}
	return 0;
}
