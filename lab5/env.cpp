/*************************************************************************
	> File Name: env.cpp
	> Author: zhongwenjie
	> Mail: zwj5582@gmail.com 
	> Created Time: Wed 13 Jul 2016 11:48:57 PM PDT
 ************************************************************************/

#include<iostream>
#include<stdio.h>
#include<string>
#include<stdlib.h>
#include<unistd.h>
using namespace std;

int main(int argc,char** argv,char** envp){
	char** p=envp;
	for(;*p!=0;++p){
		cout<<*p<<endl;
	}
	cout<<"\n\n\n----------------environ"<<endl;
	//char* s=getenv("USERNAME");
	//string str(s);
	//cout<<"username: "<<str<<endl;
	putenv("USERNAME=zhongwenjie");
	char* s=getenv("USERNAME");
	string str1(s);
	cout<<"username: "<<str1<<endl;
	setenv("USERNAME","zhong",1);
	s=getenv("USERNAME");
	string str2(s);
	cout<<"username: "<<str2<<endl;
	return 0;
}
