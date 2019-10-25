#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>
#include<iostream>
using namespace std;

#define send_data_tag 2001
#define return_data_tag 2002

int binarySearch(int a[],int x,int l,int r);

int main(int argc,char **argv)
{
	int rank,num_procs,my_id,elements_per_process,x,found,num_received_count;

	int SIZE=atoi(argv[2]);

	int n=SIZE,id;
	int a[SIZE],start,end,st;
	int t[SIZE];
	bool b[SIZE];
	
	for(int i=0;i<n;i++)
	{
		b[i]=false;
	}

	for(int i=0;i<n;i++)
	{
		a[i]=rand()%(SIZE);
		if(b[a[i]])
			i--;		
		else
			b[a[i]]=true;

	}
	
	sort(a,a+n);

	char *s=argv[1];
	x=atoi(s);
	//printf("x=%s",s);
	MPI_Status status;
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&num_procs);				
	MPI_Comm_rank(MPI_COMM_WORLD,&my_id);				
		
	//printf("num procs -> %d\n",num_procs);
	
	if(my_id==0)
	{
		//printf("Root -> %d\n",my_id);
		for(int i=0;i<n;i++)
		{
			printf("%d ",i);
		}
		printf("\n");
		for(int i=0;i<n;i++)
		{
			printf("%d ",a[i]);
		}
			
		printf("\n");	
		
		//elements_per_process=round((float)n/(float)num_procs);
		elements_per_process=n/num_procs;
				
		for(id=1;id<num_procs;id++)
		{
			start=id*elements_per_process;
			if((start+elements_per_process)<=n)			
			{	
				end=start+elements_per_process-1;
			}
			else
			{
				end=n-1;
			}
			
			if(id==num_procs-1 && end<n-1)
			{
				end=n-1;
			}

			printf("id:%d start:%d end:%d \n",id,start,end);
			
			//send data after dividing to respective processes
			int y=end-start+1;
			MPI_Send(&x,1,MPI_INT,id,send_data_tag,MPI_COMM_WORLD);
			MPI_Send(&y,1,MPI_INT,id,send_data_tag,MPI_COMM_WORLD);
			MPI_Send(&a[start],y,MPI_INT,id,send_data_tag,MPI_COMM_WORLD);			
			MPI_Send(&start,1,MPI_INT,id,send_data_tag,MPI_COMM_WORLD);
													
		}
		
		found=-1;		
		found=binarySearch(a,x,0,elements_per_process);
		printf("Found from root=%d",found);		
		if(found>=0)
		{
			printf("\nKey found at index (from root-process %d) = %d\n",my_id,found);	
		}
		else
		{
			for(id=1;id<n;id++)
			{
				MPI_Recv(&found,1,MPI_INT,MPI_ANY_SOURCE,return_data_tag,MPI_COMM_WORLD,&status);
				if(found>=0)
				{
					printf("\n---Acknowledged->Found index= %d---\n",found);
					break;
				}
			}
		}		
		
	}
	else
	{
		//printf("Process -> %d\n",my_id);
		MPI_Recv(&x,1,MPI_INT,0,send_data_tag,MPI_COMM_WORLD,&status);
		MPI_Recv(&num_received_count,1,MPI_INT,0,send_data_tag,MPI_COMM_WORLD,&status);
		MPI_Recv(&t,num_received_count,MPI_INT,0,send_data_tag,MPI_COMM_WORLD,&status);
		MPI_Recv(&st,1,MPI_INT,0,send_data_tag,MPI_COMM_WORLD,&status);

		found=binarySearch(t,x,0,num_received_count);
		int z=-1;
		//printf("z=%d",z);
		if(found>=0)
		{
			z=st+found;					
			printf("\nKey found at index (from process %d) = %d\n",my_id,z);				
		}			
		MPI_Send(&z,1,MPI_INT,0,return_data_tag,MPI_COMM_WORLD);					

	}	
	
	MPI_Finalize();
}


int binarySearch(int arr[],int key,int l,int r)
{
	int mid;
	while(l<=r)
	{
		mid=(l+r)/2;

		if(arr[mid]==key)
		{
			return mid;	
		}
		else if(arr[mid]>key)
		{
			r=mid-1;
		}
		else
		{
			l=mid+1;				
		}
	}
	return -1;
}

