/*************************************************************************
	> File Name: insert_sort.cpp
	> Author: zhongwenjie
	> Mail: zwj5582@gmail.com 
	> Created Time: Sun 17 Jul 2016 11:18:07 PM PDT
 ************************************************************************/

#define TYPE int

void insert_sort(TYPE arr[],int len){
	if(len<=1)
		return;
	int i,j;
	for(i=1;i<len;i++){
		TYPE tmp=arr[i];
		for(j=i-1;j>=0;j--){
			if(arr[j]>tmp)
				arr[j+1]=arr[j];
			else{
				arr[j+1]=tmp;break;
			}
		}
		if(j==-1)
			arr[0]=tmp;
	}
}
