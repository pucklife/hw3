/* A simple test harness for memory alloction. */

//this is my test program (its a mess)
#include "mm_alloc.h"
#include <stdio.h>
int main(int argc, char **argv)
{


    char *data;
    char *data1;
    int *data2;
    int *data3;
    int *data4;
    long int *data5;
    data5 = (long int*) mm_malloc(80);
    *data5 =123;
    mm_realloc(data5, 90);
    //printf("break at: %p\n",(void*)sbrk(0));
    //print_blocks();
    data = (int*) mm_malloc(1);
    *data = 1;
    //print_blocks();
    data1 = (int*) mm_malloc(1);
    data1[0] = 2;
    //print_blocks();
    data2 = (int*) mm_malloc(4);
    *data2 = 3;
    data3 = (int*) mm_malloc(4);
    *data3 = 4;
    data4 = (int*) mm_malloc(4);
    *data4 = 5;
    //printf("data 5 is %ld", *data5);
    //print_blocks();
    mm_free(data4);
    mm_free(data5);
    //print_blocks();
    mm_free(data);
    //print_blocks();
    mm_free(data2);
    //print_blocks();
    mm_free(data1);
    //print_blocks();
    
    


    
    printf("malloc sanity test successful!\n");
    return 0;
}

/*
//this is Garrit's test program
#include "mm_alloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void mem_visualiser(){
	s_block_ptr current=head;
	while(current){
		size_t s=current->size;
		void * p=current->data;
		
		printf("#################\n");
		printf("size: %d\n",(int) s);
		printf("data ptr: %p\n",p);
		printf("free: %d\n",current->free);
		printf("-------------------\n");
		void* i;
		for(i=current->data; i < ( (current->next == NULL)?sbrk(0):current->next ) ;i++){
			printf("_____BYTE_____\n");
			
		}
		
		current=current->next;
	}
}

int main(int argc, char **argv)
{
    int *data1;
    char *data2;
    int *data3;
    int *data4;
    int *data5;
    int *data6;
    int *data7;
    
	//printf("break pt: %d\n",sbrk(0));
   	
   	data1= (int *) mm_malloc(8);
   	
   	data2= (char *) mm_malloc(5);
   	
   	data3=(int *) mm_malloc(60);
   	
   	data6= (int *) mm_malloc(4);
   	
   	data7= (int *) mm_malloc(4);
   	//data4=(int *) mm_realloc(data2,4);
   	
   	//mm_free(data2);
    
    data5=(int *) mm_realloc(data3,8);
    
    mm_free(data6);
    
    mem_visualiser();
    
    
   	
    printf("\n\nmalloc sanity test successful!\n");
   
    return 0;
}*/

