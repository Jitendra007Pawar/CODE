#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<iostream>
using namespace std;

#define send_data_tag 2001

void constructTree(int low,int high,int *arr,int offset,int k,int N);
void inorder(int *arr,int start,int end,int,int);

int main(int argc,char **argv)
{
	
	MPI_Status status;

	int num_procs,rank,my_id,N=atoi(argv[1]),i,j,children=4,offset,n;
	int arr[N],slave_arr[N];
	double begin,end;	
	MPI_Init(&argc,&argv);
	begin=MPI_Wtime();	
	MPI_Comm_size(MPI_COMM_WORLD,&num_procs);
	MPI_Comm_rank(MPI_COMM_WORLD,&my_id);
	int x=atoi(argv[2]);	
	if(my_id==0)
	{
		for(i=1;i<=children;i++)
		{
			offset=(i-1)*N;
			constructTree(0,N-2,arr,offset,0,N);			
			
			//printf("Sending to process %d, ",i);
			MPI_Send(&N,1,MPI_INT,i,send_data_tag,MPI_COMM_WORLD);
			MPI_Send(arr,N,MPI_INT,i,send_data_tag,MPI_COMM_WORLD);
			
		}		
	}
	else
	{
		MPI_Recv(&n,1,MPI_INT,0,send_data_tag,MPI_COMM_WORLD,&status);				
		MPI_Recv(slave_arr,n,MPI_INT,0,send_data_tag,MPI_COMM_WORLD,&status);				
		
		printf("\n");
		inorder(slave_arr,0,n-2,x,my_id);			
		printf("\n");
	}

	end=MPI_Wtime();
	printf("\nExecution time=%f where process-id=%d\n",(begin-end),my_id);

	MPI_Finalize();	

	return 0;
}


void constructTree(int low,int high,int *arr,int offset,int k,int N)
{
	if(low<=high && k<=N-2)
	{
		int mid=(low+high)/2;
		arr[k]=mid+offset+1;
		//printf("\nk=%d  arr[k]= %d offset=%d\n",k,arr[k],offset);
		constructTree(low,mid,arr,offset,k*2+1,N);
		constructTree(mid+1,high,arr,offset,k*2+2,N);
	}	
}

void inorder(int *arr,int start,int end,int x,int id)
{

	if(start>end)
		return;	
	
	inorder(arr,start*2+1,end,x,id);
	printf("%d ",arr[start]);
	if(arr[start]==x)
	{
		cout<<"(------Found out "<<x<<" from process : "<<id<<"----------)";
	}	
	inorder(arr,start*2+2,end,x,id);
	
	

}



