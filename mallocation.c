#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


/*
If we use mymalloc, don't use malloc !!!
*/

typedef struct block_header {
  unsigned int size : 29,
  zero : 2,
  alloc : 1;
} Block;

/*Increase the size of the heap*/
Block *increase(size_t size)
{
  Block *add = (Block*) sbrk(size);

  if ((void*) add == (void*) -1) {return NULL;}

  add->size = size;
  add->zero = 0;
  add->alloc = 1;

  return add;
}

void *mymalloc (size_t size)
{
  /*ptr to the first allocated block*/
  static Block *start_heap = NULL;

  /*Set size to a multiple of 4*/
  if (size % 4 != 0)
  {
    size = size + (4-(size%4));
  }

  if (start_heap != NULL)
  {
    /*first block_header of the heap*/
    Block *sheap = start_heap;

    while ((void*)sheap < sbrk(0)) {
      if (sheap->alloc == 0 && sheap->size >= size + 4)
      {
        sheap->alloc = 1;
        return (char*) sheap+4;
      }
      else /*Move to the next Block*/
      {
        sheap = (Block*)((char*) sheap + sheap->size);
      }
    }
  }

  Block *b = increase(size+4);

  /*if increase() fails*/
  if(b == NULL) {return NULL;}
  else
  {
    if(start_heap == NULL) {start_heap = b;}
    return (char*) b+4;
  }

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

void *mycalloc()
{
  return NULL;
}


int main(int argc, char const *argv[]) {
  void *a = mymalloc(64);
  void *b = mymalloc(8);

  printf("%p\n", a);
  printf("%p\n", b);

  myfree(a);
  myfree(b);

  void *c = mymalloc(8);
  void *d = mymalloc(8);

  printf("%p\n", c);
  printf("%p\n", d);

  return 0;
}
