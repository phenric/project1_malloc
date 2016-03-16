#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "mallocation.h"


/*
If we use mymalloc, don't use malloc !!!
*/


Block *increase(size_t size)
{
  Block *add = (Block*) sbrk(size);

  if ((void*) add == (void*) -1) {return NULL;}

  add->size = size;
  add->zero = 0;
  add->alloc = 0;

  return add;
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

void *mymalloc (size_t size, size_t mem_size)
{
  /*ptr to the first allocated block*/
  static Block *start_heap = NULL;

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
    start_heap = increase(mem_size);

    if (start_heap != NULL) {
      start_heap->alloc = 0;
    }
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

  /*Due the fact that we can't increase the heap for the project, we will not use it
  Block *b = increase(size+4);

  if increase() fails
  if(b == NULL) {return NULL;}
  else
  {
    if(start_heap == NULL) {start_heap = b;}
    return (char*) b+4;
  }*/

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

void *mycalloc (size_t size, size_t mem_size)
{
  void *newmalloc = mymalloc(size, mem_size);
  if (newmalloc == NULL)
  {
    return NULL;
  }
  Block *findsize = (Block*) ((char*) newmalloc - 4);
  size_t t= (findsize->size)-4;
  memset(newmalloc,0,t);
  return newmalloc;
}

int main(int argc, char const *argv[]) {
  void *a = mymalloc(64, 1024);
  void *b = mymalloc(8, 1024);

  printf("%p\n", a);
  printf("%p\n", b);

  myfree(a);
  myfree(b);

  void *c = mymalloc(8, 1024);
  void *d = mymalloc(8, 1024);

  printf("%p\n", c);
  printf("%p\n", d);

  void *e = mycalloc(64, 1024);
  void *f = mycalloc(8, 1024);

  printf("%p\n", e);
  printf("%p\n", f);

  myfree(e);
  myfree(f);

  void *g = mycalloc(8, 1024);
  void *h = mycalloc(8, 1024);

  printf("%p\n", g);
  printf("%p\n", h);

  return 0;
}
