/*
 * mm_alloc.h
 *
 * Exports a clone of the interface documented in "man 3 malloc".
 */


#pragma once

#ifndef _malloc_H_
#define _malloc_H_

 /* Define the block size since the sizeof will be wrong *//*but then the code is not cross platform compatible!*/

#define BLOCK_SIZE sizeof(struct s_block)

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>

void* mm_malloc(size_t size);

void* mm_realloc(void* ptr, size_t size);
void mm_free(void* ptr);



typedef struct s_block *s_block_ptr;

extern s_block_ptr head;


/* block struct */
struct s_block {
    size_t size;
    struct s_block *next;
    struct s_block *prev;
    int free;
    void *ptr;		
    char data [0];
 };

/* Split block according to size, b must exist */
void split_block (s_block_ptr b, size_t s);

/* Try fusing block with neighbors */
s_block_ptr fusion(s_block_ptr b);

/* Get the block from addr */
s_block_ptr get_block (void *p);

/* Add a new block at the of heap,
 * return NULL if things go wrong
 */
s_block_ptr extend_heap (s_block_ptr last , size_t s);

void print_blocks();


#ifdef __cplusplus
}
#endif

#endif
