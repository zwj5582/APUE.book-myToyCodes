/*************************************************************************
	> File Name: main.c
	> Author: zhongwenjie
	> Mail: zwj5582@gmail.com 
	> Created Time: Sun 21 Aug 2016 05:55:29 PM PDT
 ************************************************************************/

#include<stdio.h>
#include"rand.h"

int main(){
	mysrand(9);
	printf("%d\n",myrand());
	return 0;
}
