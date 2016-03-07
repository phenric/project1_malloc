#include <stdio.h>
#include <stdlib.h>

typedef struct block_header {
  unsigned int size : 29,
  zero : 2,
  alloc : 1;
} Block;

int count (void *sheap)
{
  int count = 0;
  while (*sheap != NULL) {
    count++;
  }
  return count;
}

void *mymalloc (size_t size)
{
  /*Mettre size au multiple de 4*/
  if (size % 4 != 0)
  {
    size = size + (4-(size%4));
  }

  /*first block_header of the heap*/
  void *sheap = sbrk(0);
  void *remember = sbrk(0);

  int i;
  for(i=0; i < count; i++) {
    if (sheap->alloc == 1 && sheap->size >= size)
    {
      if (sheap->size - size < remember->size - size)
      {
        *remember = &sheap;
      }
    }
    *sheap = &sheap + sheap->size;
  }

  if (remember->size - size < 0)
  {
    /*Extend heap and if it's impossible return NULL*/
  }else{
    remember->alloc = 0;
    return remember->size;
  }
  return NULL;
}

void myfree (void *ptr)
{
  if (*ptr =)
}

void mycalloc()
{

}


int main(int argc, char const *argv[]) {

  return 0;
}
