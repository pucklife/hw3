/*
 * mm_alloc.c
 *
 * Stub implementations of the mm_* routines. Remove this comment and provide
 * a summary of your allocator's design here.
 */

#include "mm_alloc.h"

#include <stdlib.h>
#include <stdio.h>

s_block_ptr head = NULL;


void* mm_malloc(size_t size)
{
	//printf("mm_malloc called: Size of data struct is %d.\nsize of ptr is %d\nSize of size_t is %d",BLOCK_SIZE,sizeof(s_block_ptr),sizeof(int));
	s_block_ptr current;
	s_block_ptr last=NULL;
	
	current = head;
	while (current!=NULL){
		if(current->free){
			if(current->size == size){
				current->free = 0;
				return current->data;	
			}
			if(current->size > size){
				printf("\ncheck\n");
				split_block (current, size);
				current->free = 0;
				return current->data;
			}
			
		}
		last=current;
		current=current->next;
	}
	
	//if mm_malloc has not yet returned then it has not found an appropriate block: extend sbrk
	current = extend_heap (last, size);
	current->free = 0;
	printf("sbrk neccessary\n");
	if (head==NULL){	//if its the first malloc
		head=current;
		printf("Head created: %p\n",head);
	}
	else last->next = current;
	return current->data;	
}


/*void* mm_realloc(void* ptr, size_t size)
{

}*/

void split_block (s_block_ptr b, size_t s){
	printf("split trial\n");
	if(b && b->size > s+BLOCK_SIZE+sizeof(char)){
		s_block_ptr extraBlock = b->data + s;
		extraBlock->size = b->size - BLOCK_SIZE - s;
		extraBlock->next = b->next;
		extraBlock->prev = b;
		extraBlock->free = 1;
		extraBlock->ptr = extraBlock;
		if(extraBlock->next){
			extraBlock->next->prev = extraBlock;
			
		};
		b->size = s;
		b->next = extraBlock;
		
	}
}


s_block_ptr fusion(s_block_ptr b){
	if(b && b->free){
		if(b->next && b->next->free){
			b->size = b->size + BLOCK_SIZE + b->next->size;
			b->next = b->next->next;
			if(b->next->prev){
				b->next->prev = b;
			}
		}
		if(b->prev && b->prev->free){
			b->prev->size = b->size + BLOCK_SIZE + b->prev->size;
			b->prev->next = b->next;
			if(b->next && b->next->prev){
				b->next->prev = b->prev;
			}
			b = b->prev;
		}
	}
	return b;
}

void mm_free(void* ptr)
{
	s_block_ptr freeblock = get_block(ptr);
	if (freeblock){
		freeblock->free = 1;
		fusion(freeblock);
	}
}

s_block_ptr extend_heap (s_block_ptr last , size_t s){
	s_block_ptr newSp;
	if((newSp = (s_block_ptr) sbrk(BLOCK_SIZE + s)) == (void*)-1){
		return NULL;
	}
	
	newSp->size=s;
	newSp->next = NULL;
	newSp->prev = last;
	newSp->free = 1;	//if free==0 then it is not free, if free==1 then it is free
	newSp->ptr = newSp;		
    	//printf("New block at %p. It's size is %d. Pointer to next is %p. Pointer to block pointer is %d\n", newSp,newSp->size,newSp->next,newSp->data);
	return newSp;

}

s_block_ptr get_block (void *p){

	s_block_ptr current = head;
	while(current!=NULL){
		if (current->data == p){
			return current;
		}
		else{
			current = current->next;
		}
	}
	return NULL;
}

void print_blocks(){
	s_block_ptr current = head;
	while(current!=NULL){
		printf("Block address: %p. block free %d. Data size: %d. Next ptr: %p. Prev ptr: %p. Data @ %p. Date value is %d\n",current,current->free,current->size,current->next,current->prev,current->data,*current->data);
		current=current->next;
	}
	printf("Finish printing memery block list\n");









}






