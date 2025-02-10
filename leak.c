#include <stdio.h>

void main(){
	int buc_size,no_int,out_rate,in_size,store=0,discarded=0;
	printf("\nenter the size of the bucket,no of inputs and outgoing rates\n");
	scanf("%d%d%d",&buc_size,&no_int,&out_rate);
	
	while(no_int!=0){
		printf("\nenter the size of the incoming packet : ");
		scanf("%d",&in_size);
		
		if (in_size<=(buc_size-store)){
			store += in_size;
			printf("\nBucket Size : %d of %d \n",store,buc_size);
		}
		else {
			printf("Dropped %d packets\n", in_size - (buc_size - store));
			discarded += in_size - (buc_size - store); 
			store=buc_size;
			printf("Bucket full\n");
		}
		
		
		if (store>=out_rate){
			store-=out_rate;
		}
		else{
			store=0;
		}
		
		printf("After outgoing, there are %d out of %d packets left in buffer\n", store, buc_size);
		no_int--;
	}
	printf("\n\n%d\n\n",discarded);
}
