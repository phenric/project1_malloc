#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "mallocation.h"

int main(int argc, char const *argv[]) {

  int num = atoi(argv[1]);
  /*Increase the heap*/
  printf("You are increasing the heap (%d)\n", num);

  Block *err = increase(num);
  if (err == NULL)
  {
    printf("Sorry, we can not increase the heap\n");
    return 0;
  }
  printf("Increase : check\n");
  printf("############################################\n");

  void *a = mymalloc(64);
  void *b = mymalloc(8);

  printf("mymalloc a (64): %p\n", a);
  printf("mymalloc b (8): %p\n", b);

  printf("############################################\n");

  myfree(a);
  myfree(b);

  printf("Appel de myfree\n");

  printf("############################################\n");

  void *c = mymalloc(8);
  void *d = mymalloc(8);

  printf("mymalloc c (8): %p\n", c);
  printf("mymalloc d (8): %p\n", d);

  printf("############################################\n");

  void *e = mycalloc(64);
  void *f = mycalloc(8);

  printf("mycalloc e (64): %p\n", e);
  printf("mycalloc f (8): %p\n", f);

  printf("############################################\n");

  myfree(e);
  myfree(f);

  printf("Appel de myfree\n");

  printf("############################################\n");

  void *g = mycalloc(8);
  void *h = mycalloc(8);

  printf("mycalloc g (8):%p\n", g);
  printf("mycalloc h (8):%p\n", h);

  printf("############################################\n");

  return 0;
}
