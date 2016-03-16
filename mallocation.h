#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


typedef struct block_header {
  unsigned int size : 29,
  zero : 2,
  alloc : 1;
} Block;

/*increase by the size of the heap by the size put in the arguments*/
Block *increase(size_t size);

/*Merge empty blocks and return a new block*/
Block *merge (Block *b);

/*Split one block in two*/
void split (Block *b, size_t size);

/*an other version of malloc*/
void *mymalloc (size_t size);

/*an other version of free*/
void myfree (void *ptr);

/*an other version of calloc*/
void *mycalloc (size_t size);
