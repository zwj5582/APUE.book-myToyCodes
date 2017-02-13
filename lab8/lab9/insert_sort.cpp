/*************************************************************************
	> File Name: insert_sort.cpp
	> Author: zhongwenjie
	> Mail: zwj5582@gmail.com 
	> Created Time: Sun 17 Jul 2016 11:18:07 PM PDT
 ************************************************************************/

#include<iostream>
#include<bitset>
#include<string>
using namespace std;

#define TYPE int

#define parent(i) ((i-1)>>1)
#define lchild(i) ((i<<1)+1)
#define rchild(i) ((i<<1)+2)

void swap(TYPE& a,TYPE& b){
	TYPE tmp=a;
	a=b;
	b=tmp;
}

int three_max(TYPE arr[],int p,int len){
	int lc=lchild(p);
	int rc=rchild(p);
	if(lc<len&&rc<len){
		if(arr[lc]<arr[rc]){
			if(arr[rc]>arr[p])
				return rc;
		}
		else if(arr[lc]>arr[p])
			return lc;
	}
	else if(lc<len){
		if(arr[lc]>arr[p])
			return lc;
	}
	else if(rc<len){
		if(arr[rc]>arr[p])
			return rc;
	}
	return p;
}

int dou(int len){
	int l=1;
	while(l<len){
		l<<=1;
	}
	return l;
}

void insert_sort(TYPE arr[],int len){
	if(len<=1)
		return;
	for(int i=1;i<len;i++){
		TYPE tmp=arr[i];int j;
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

void per_insert_sort(TYPE arr[],int d,int len){
	for(int k=0;k<d;k++){
		for(int i=k+d;i<len;i+=d){
			TYPE tmp=arr[i];int j;
			for(j=i-d;j>=0;j-=d){
				if(arr[j]>tmp)
					arr[j+d]=arr[j];
				else{
					arr[j+d]=tmp;break;
				}
			}
			if(j<0)
				arr[k]=tmp;
		}
	}
}

void shell_insert(TYPE arr[],int len){
	int d=len;
	d>>=1;
	while(d!=0){
		per_insert_sort(arr,d,len);
		d>>=1;
	}
}

void choose_sort(TYPE arr[],int len){
	for(int i=0;i<len;i++){
		TYPE min=arr[i];int k=i;
		for(int j=i+1;j<len;j++){
			if(min>arr[j]){
				min=arr[j];k=j;
			}
		}
		swap(arr[k],arr[i]);
	}
}

void creat_heap(TYPE arr[],int len){
	//int l=dou(len);
	//int ll=(l>>1)-2;
	int ll=len-1;
	while(ll>=0){
		int p=three_max(arr,ll,len);
		swap(arr[p],arr[ll]);
		ll--;
	}
}

void change_heap(TYPE arr[],int len){
	int i=0;
	while(lchild(i)<len||rchild(i)<len){
		int p=three_max(arr,i,len);
		if(p==i)
			break;
		else{
			swap(arr[i],arr[p]);i=p;
		}
	}
}

void heap_sort(TYPE arr[],int len){
	int l=len;
	creat_heap(arr,l);
	while(l>1){
		swap(arr[0],arr[l-1]);
		change_heap(arr,l-1);
		l--;
	}
}

void merge(TYPE arr[],int lo,int mid,int hi){
	int i=lo;
	TYPE* tmpa=new TYPE[mid-lo];
	for(int j=lo,k=0;j<mid;j++,k++){
		tmpa[k]=arr[j];
	}
	TYPE* tmpb=arr+mid;
	int a=0,b=0;
	int mi=mid;
	while(lo<mi&&mid<hi){
		if(tmpa[a]>tmpb[b]){
			arr[i++]=tmpb[b++];mid++;
		}
		else{
			arr[i++]=tmpa[a++];lo++;
		}
	}
	while(lo<mi){
		arr[i++]=tmpa[a++];lo++;
	}
	delete[]tmpa;
}

void merge_sort(TYPE arr[],int lo,int hi){
	if((hi-lo)<=1)
		return;
	int mid=(lo+hi)/2;
	merge_sort(arr,lo,mid);
	merge_sort(arr,mid,hi);
	merge(arr,lo,mid,hi);
}

int partition(TYPE arr[],int lo,int hi){
	TYPE tmp_data=arr[lo];
	while(lo<hi){
		while(lo<hi){
			if(arr[hi]>tmp_data)
				hi--;
			else{
				arr[lo++]=arr[hi];break;
			}
		}
		while(lo<hi){
			if(arr[lo]<tmp_data)
				lo++;
			else{
				arr[hi--]=arr[lo];break;
			}
		}
	}
	arr[lo]=tmp_data;
	return lo;
}

void quick_sort(TYPE arr[],int lo,int hi){
	if((hi-lo)<=1)
		return;
	int p=partition(arr,lo,hi-1);
	quick_sort(arr,lo,p);
	quick_sort(arr,p+1,hi);
}

int main(){
	TYPE arr[10]={5,4,6,8,3,1,5,7,0,9};
	quick_sort(arr,0,10);
	for(int i=0;i<10;i++){
		cout<<arr[i]<<" ";
	}
	cout<<endl;
	cout<<endl;
	string str="ababa";
	bitset<32> bit(1U);
	bit.flip();
	bit.reset();
	bit.set();
	cout<<bit.to_ullong()<<endl;
	return 0;
}
