#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "mallocation.h"

 static Block *start_heap = NULL;

/*
If we use mymalloc, don't use malloc !!!
*/


int increase(size_t size)
{
  if((int)size <= 0)
  {
    return 0;
  }

  start_heap = (Block*) sbrk(size);

  if ((void*) start_heap == (void*) -1) {return 0;}

  start_heap->size = size;
  start_heap->zero = 0;
  start_heap->alloc = 0;

  return 1;
}

/*Merge some empty blocks*/
Block *merge (Block *b)
{
  Block *sheap = b;
  int count = 10;

  while ((void*)sheap < sbrk(0) && count) {
    if (sheap->alloc == 0)
    {
      Block *next = (Block*)((char*) sheap + sheap->size);
      while (next->alloc == 0 && (void*)next < sbrk(0)) {
        sheap->size = sheap->size + next->size;
        next = (Block*)((char*) next + next->size);
      }
    }
    else /*Move to the next Block*/
    {
      sheap = (Block*)((char*) sheap + sheap->size);
    }
    count--;
  }
  return sheap;
}

/*Split one block in two*/
void split (Block *b, size_t size)
{
  if(b->size < size+4){return;}

  Block *sp = (Block*) ((char*) b + size);

  sp->size = b->size - size;
  sp->zero = 0;
  sp->alloc = 0;

  b->size = size;
}

void *mymalloc (size_t size)
{
  /*ptr to the first allocated block
  static Block *start_heap = NULL;
  */

  Block *mem = start_heap;

  /*Optimization: before any mymalloc, merge some blocks*/
  if(mem == NULL || (void*)mem >= sbrk(0)) /*If mem is at the end of the heap*/
  {
    mem = start_heap;
  }

  if (start_heap != NULL)
  {
  merge(mem);
  }

  /*Set size to a multiple of 4*/
  if (size % 4 != 0)
  {
    size = size + (4-(size%4));
  }

  /*If it's the first call of mymalloc*/
  if (start_heap == NULL) {
    return NULL;
    
  }

  if (start_heap != NULL)
  {
    /*first block_header of the heap*/
    Block *sheap = start_heap;

    while ((void*)sheap < sbrk(0)) {
      if (sheap->alloc == 0 && sheap->size >= size + 4)
      {
        /*Optimization: before alloc, adjust size*/
        if (sheap->size != size + 4)
        {
          split(sheap, size+4);
        }

        sheap->alloc = 1;
        return (char*) sheap+4;
      }
      else /*Move to the next Block*/
      {
        sheap = (Block*)((char*) sheap + sheap->size);
      }
    }
  }

  /*If the heap is full, do not increase and return NULL*/
  return NULL;
}


void myfree (void *ptr)
{
  if (ptr == NULL)
  {
    return;
  }

  Block *bl = (Block*) ((char*) ptr - 4);
  bl->alloc = 0;
}

void *mycalloc (size_t size)
{
  void *newmalloc = mymalloc(size);
  Block *findsize = (Block*) ((char*) newmalloc - 4);
  size_t t= (findsize->size)-4;
  memset(newmalloc,0,t);
  return newmalloc;
}
