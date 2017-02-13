/*************************************************************************
	> File Name: test.cpp
	> Author: zhongwenjie
	> Mail: zwj5582@gmail.com 
	> Created Time: Sat 16 Jul 2016 06:53:56 PM PDT
 ************************************************************************/

#include<iostream>
using namespace std;

#define ROW 3
#define COL 4
#define Z   5

#define TYPE int

TYPE** _malloc(int row,int col){
	TYPE **p=(TYPE**)malloc(sizeof(TYPE*)*row);
	for(int i=0;i<row;++i)
		p[i]=(TYPE*)malloc(sizeof(TYPE)*col);
	return p;
}

typedef TYPE(*AR)[ROW][COL];

typedef TYPE(*ARR)[COL];

ARR _new(){
	ARR q=new TYPE[ROW][COL];
	return q;
}

void _printf(ARR q,int row,int col){
	for(int i=0;i<row;++i){
		for(int j=0;j<col;++j)
			cout<<q[i][j]<<" ";
		cout<<endl;
	}
}

void _ass(ARR q,int row,int col){
	for(int i=0;i<ROW;++i)
		for(int j=0;j<COL;++j)
			q[i][j]=i+j;
}

void _delete(ARR q){
	delete[]q;
}

int main(){

	int **p=(int**)malloc(sizeof(int*)*ROW);
	for(int i=0;i<ROW;++i)
		p[i]=(int*)malloc(sizeof(int)*COL);

	for(int i=0;i<ROW;++i)
		for(int j=0;j<COL;++j)
			p[i][j]=i+j;

	for(int i=0;i<ROW;++i){
		for(int j=0;j<COL;++j)
			printf("%d ",p[i][j]);
		printf("\n");
	}

	for(int i=0;i<ROW;++i)
		free(p[i]);
	free(p);

	cout<<endl;

	ARR q=new int[ROW][COL];

	for(int i=0;i<ROW;++i)
		for(int j=0;j<COL;++j)
			q[i][j]=i+j;

	for(int i=0;i<ROW;++i){
		for(int j=0;j<COL;++j)
			cout<<q[i][j]<<" ";
		cout<<endl;
	}

	delete[]q;

	cout<<endl;

	AR r=new int[Z][ROW][COL];

	for(int i=0;i<Z;++i)
		for(int j=0;j<ROW;j++)
			for(int k=0;k<COL;k++)
				r[i][j][k]=i+j+k;
	
	for(int i=0;i<Z;++i){
		for(int j=0;j<ROW;j++){
			for(int k=0;k<COL;k++)
				cout<<r[i][j][k];
			cout<<endl;
		}
		cout<<endl;
	}

	delete[]r;

	cout<<endl;

	ARR s=_new();

	_ass(s,ROW,COL);

	_printf(s,ROW,COL);

	_delete(s);

}
