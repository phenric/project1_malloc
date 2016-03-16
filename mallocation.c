#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "mallocation.h"

static Block *start_heap = NULL;

/*
If we use mymalloc, don't use malloc !!!
*/


Block *increase(size_t size)
{
  if(size < 0)
  {
    return NULL;
  }

  start_heap = (Block*) sbrk(size);

  if ((void*) start_heap == (void*) -1) {return NULL;}

  start_heap->size = size;
  start_heap->zero = 0;
  start_heap->alloc = 0;

  return start_heap;
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
    /*
    start_heap = increase(mem_size);

    if (start_heap != NULL) {
      start_heap->alloc = 0;
    }*/
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

void *mycalloc (size_t size)
{
  void *newmalloc = mymalloc(size);
  Block *findsize = (Block*) ((char*) newmalloc - 4);
  size_t t= (findsize->size)-4;
  memset(newmalloc,0,t);
  return newmalloc;
}
/*
int main(int argc, char const *argv[]) {

  int num = atoi(argv[1]);
  /*Increase the heap
  printf("You are increasing the heap (%d)\n", num);
  /*
  Block *err = increase(num);
  if (err == NULL)
  {
    printf("Sorry, we can not increase the heap\n");
    return 0;
  }
  printf("Increase : check\n");
  printf("############################################\n");

  void *a = mymalloc(64, num);
  void *b = mymalloc(8, num);

  printf("mymalloc a (64): %p\n", a);
  printf("mymalloc b (8): %p\n", b);

  printf("############################################\n");

  myfree(a);
  myfree(b);

  printf("Appel de myfree\n");

  printf("############################################\n");

  void *c = mymalloc(8, num);
  void *d = mymalloc(8, num);

  printf("mymalloc c (8): %p\n", c);
  printf("mymalloc d (8): %p\n", d);

  printf("############################################\n");

  void *e = mycalloc(64, num);
  void *f = mycalloc(8, num);

  printf("mycalloc e (64): %p\n", e);
  printf("mycalloc f (8): %p\n", f);

  printf("############################################\n");

  myfree(e);
  myfree(f);

  printf("Appel de myfree\n");

  printf("############################################\n");

  void *g = mycalloc(8, num);
  void *h = mycalloc(8, num);

  printf("mycalloc g (8):%p\n", g);
  printf("mycalloc h (8):%p\n", h);

  printf("############################################\n");

  return 0;
}*/
