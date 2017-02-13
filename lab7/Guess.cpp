/*************************************************************************
	> File Name: Guess.cpp
	> Author: zhongwenjie
	> Mail: zwj5582@gmail.com 
	> Created Time: Fri 15 Jul 2016 08:49:05 PM PDT
 ************************************************************************/

#include<iostream>
#include<stdio.h>
#include<memory.h>
#include<stdlib.h>
#include<time.h>
#include<cmath>
//#include"conio.h""
using namespace std;

#define OK 1
#define NO 0

#define NO_POSSIBLE_CHAR '#'

#define MAX_NUM 100000

#define DIGIT_NUM 10

#define ARR_LEN 4

int n=1;

int CreatNum(char  Num_List[]){
	srand((unsigned int)time(0));
	int num=rand();
	srand(num+(n++));
	int count=0;
	int inum;
	char saveNum[DIGIT_NUM];
	memset(saveNum,NO_POSSIBLE_CHAR,sizeof(saveNum));
	while(count<ARR_LEN){
		inum=rand()%10;
		if(saveNum[inum]==inum){
			continue;
		}
		else{
			saveNum[inum]=inum;
			Num_List[count++]=(char)(inum+48);
		}
	}
	return OK;
}

char* istrue(char true_num[],char input_num[],int len=ARR_LEN){
	char* output=new char[4];
	int a=0;
	int b=0;
	for(int i=0;i<len;i++){
		for(int j=0;j<len;j++){
			if(true_num[i]==input_num[j]){
				if(i==j){
					a++;
				}
				else{
					b++;
				}
				break;
			}
		}
	}
	output[0]=(char)(a+48);
	output[1]='A';
	output[2]=(char)(b+48);
	output[3]='B';
	return output;
}

int main(){
	//char* true_num=new char[ARR_LEN];
	char true_num[ARR_LEN];
	//bool flag=false;
	bool flag=false;
	CreatNum(true_num);
	for(int i=0;i<4;i++)
		cout<<true_num[i];
	cout<<endl;
	char input[ARR_LEN];
	int count=0;
	while((count++)<10){
		cout<<"please: ";
		for(int i=0;i<ARR_LEN;i++){
			cin>>input[i];
		}
		//for(int i=0;i<4;i++)
		//	cout<<true_num[i];
		//cout<<endl;
		
		char result[ARR_LEN];
		char* fuck=istrue(true_num,input);
		for(int i=0;i<ARR_LEN;i++){
			result[i]=fuck[i];
		}
		if(result[0]=='4'){
			cout<<"right!"<<endl;
			flag=true;
			break;
		}
		else{
			for(int i=0;i<ARR_LEN;i++){
				cout<<result[i];
			}
			cout<<endl;
		}
	delete[]fuck;
	}
	if(!flag){
		cout<<"over"<<endl;
	}
//	delete[]true_num;
	return 0;
}
