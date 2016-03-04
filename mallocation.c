#include <stdio.h>
#include <stdlib.h>

typedef struct block_header {
  unsigned int size : 29,
  zero : 2,
  alloc : 1;
} Block;

void *mymalloc (size_t size)
{
  //Mettre size au multiple de 4
  if (size % 4 != 0)
  {
    size = size + (4-(size%4));
  }

  //first block_header of the heap
  void *sheap = sbrk(0);

  while (sheap != NULL)
  {
    //Todo make a function which checks if there is a free allocation
  }

}

void myfree (void *ptr)
{
  if (*ptr =)
}

int main(int argc, char const *argv[]) {

  return 0;
}
