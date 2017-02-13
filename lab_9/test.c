/*************************************************************************
	> File Name: test.c
	> Author: zhongwenjie
	> Mail: zwj5582@gmail.com 
	> Created Time: Mon 22 Aug 2016 10:46:30 PM PDT
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include"test.h"
#include"myprintf.h"

int main(){
	write(STDOUT_FILENO,"hello world!\n",13);
	myprintf();
	return 0;
}
