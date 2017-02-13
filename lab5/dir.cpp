/*************************************************************************
	> File Name: dir.cpp
	> Author: zhongwenjie
	> Mail: zwj5582@gmail.com 
	> Created Time: Wed 13 Jul 2016 09:59:05 PM PDT
 ************************************************************************/

#include<iostream>
#include<dirent.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
using namespace std;

int main(int argc,char** argv){
	DIR* dir;
	struct dirent* dirent;
	struct stat buf;
	if(argc!=2){
		cout<<"error"<<endl;
		return 0;
	}
	dir=opendir(argv[1]);
	dirent=readdir(dir);
	while(dirent!=NULL){
		stat(dirent->d_name,&buf);
		cout<<buf.st_size<<endl;
		dirent=readdir(dir);
	}
	closedir(dir);
	cout<<endl;
	return 0;
}

