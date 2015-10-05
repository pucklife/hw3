/*
 * mm_alloc.c
 *
 * Memory allocation setup:
 * this memory allocater will create a double linked list of memomry blocks. Each block will contain
 * information about: 	the size of the data it holds
 *			a pointer to the next memory block (the last block of list will point to null)
 * 			a pointer to the previous memomry block
 *			an indicator if the block is reserved or not
 *			a pointer with it's own address (not a neccessity)
 *			a pointer to the data segment of the block (this is what is presented to the user of this m. allocator
 * mm_malloc function (for user of library (public)):
 * this function will search for an available block that is both free and adequit in size. If it does find one it will attempt 
 * to split the block if the block is larger in size than required. If no block is apropriate then it will use sbrk to make 
 * more mememory available and add the new block at the end of the block list. 
 * On success the function will return a pointer to the data segment of the block.
 *
 *
 * mm_realloc function (for user of library):
 * Given an already allocated data ptr and size, this function will provide a block for the new size:
 * 	if the size is bigger than the current size, then the program will find a new block space an memory copy the data
 *	to the new block. (it uses mm_malloc for this)
 *	if the size is smaller than the current size, then the program will attempt to split the current block (uses split_
 *	block function)
 *	if the size is identical then it returns the original pointer.
 *
 * mm_free function (for user of library):
 * this function frees up a data block. it also calls upon the fusion function to see if the neighbouring blocks are free
 *
 * split_block function (not meant for user of library):
 * in order to use a block that is bigger than necessary, this function attempts to split the block such that the first part
 * is big enough for the required size, and that the second part is big enough for an int or bigger.
 * 
 * fusion function (not meant for user of library):
 * when a block becomes free, this function can be called upon to check wether the neighbouring blocks are free. If they are
 * free, then this function will merge this block with those neightbouring blocks that are free.
 * If only the next block is free, then the function will return the block pointer it started with, but the data will have
 * increased.
 * If the previous block is free then the block pointer that is returned is that of the previous block, which now would
 * have increased in size to include the current block.
 *
 * extend_heap function (not meant for user of library):
 * uses sbrk() to extended heap size
 * 
 * get_block function (not meant for user of library):
 * given a data pointer, the function will return the pointer to the block holding that data.
 *
 * print_blocks:
 * this function prints out the list memeory blocks providing the following info:
 *		data block pointer
 *		data block pointer of next block
 *		data block pointer of previous block
 *		if the data block is free or not
 *		the size of the data space in the block
 *		the pointer to the data segment
 *		assuming the content of data is an int, it will print the value of the int.
 */

#include "mm_alloc.h"

#include <stdlib.h>
#include <stdio.h>

s_block_ptr head = NULL;


void* mm_malloc(size_t size)
{
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
	if (head==NULL){	//if its the first malloc
		head=current;
	}
	else last->next = current;
	return current->data;	
}


void* mm_realloc(void* ptr, size_t size){
	s_block_ptr reBlock = get_block(ptr);
	if(reBlock==NULL) return NULL;
	if(reBlock->size < size){
		void * new = mm_malloc(size);
		memcpy(new,reBlock->data,reBlock->size);
		reBlock->free = 1;
		fusion(reBlock);
		
	}
	else if(reBlock->size > size){
		split_block(reBlock, size);
		return (void*) ptr;
	}
	return (void*) ptr;
}

void split_block (s_block_ptr b, size_t s){
	if(b && b->size > s+BLOCK_SIZE+sizeof(char)){
		s_block_ptr extraBlock = (s_block_ptr) b->data + s;
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
	return newSp;

}

s_block_ptr get_block (void *p){
	if(p==NULL) return NULL;
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
		//assuming data type is an int
		printf("Block address: %p. block free %d. Data size: %d. Next ptr: %p. Prev ptr: %p. Data @ %p. Data value is %d\n",current,current->free,current->size,current->next,current->prev,current->data,*current->data);
		current=current->next;
	}
	printf("Finish printing memery block list\n");
}






