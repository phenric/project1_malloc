#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "mallocation.h"

void tests ()
{
	/*test du increase du heap*/
	int heap = increase(-8);
	CU_ASSERT_EQUAL(heap,0);
	heap = increase(1024);
	CU_ASSERT_EQUAL(heap,1);
	/*test du malloc*/
	void *a = mymalloc(47);
	CU_ASSERT_PTR_NOT_NULL(a);
	Block *bl = (Block*) ((char*) a - 4);
	CU_ASSERT_EQUAL(bl->alloc,1);
	/*taille mise en multiple de 4 (48) + header = 52*/
	CU_ASSERT_EQUAL(bl->size,52);
	CU_ASSERT_EQUAL(bl->zero,0);
	/*test du free*/
	myfree(a);
	CU_ASSERT_EQUAL(bl->alloc,0);
	CU_ASSERT_EQUAL(bl->size,52);
	/*test de mallocs trop grands ou négatifs */
	void *b = mymalloc(1200);
	CU_ASSERT_PTR_NULL(b);
	void *b2 = mymalloc(-9);
	CU_ASSERT_PTR_NULL(b);
	/*test du calloc, le calloc appelant malloc, les tests pour callocs trop grands ou négatifs ne sont pas effectués */
	int *c =(int*) mycalloc(81);
	Block *bl3 = (Block*) ((char*) c-4);
	CU_ASSERT_EQUAL(bl3->alloc,1);
	CU_ASSERT_EQUAL(bl3->size,88);
	CU_ASSERT_EQUAL(bl3->zero,0);
	CU_ASSERT_PTR_NOT_NULL(c);
	/*vérification des initialisations à 0*/
	CU_ASSERT_EQUAL(c[0],0);
	CU_ASSERT_EQUAL(c[(bl3->size)/2],0);
	/*vérification d'un free de calloc*/
	myfree(c);
	CU_ASSERT_EQUAL(bl3->alloc,0);
}



int main(int argc, char const *argv[]) 
{
	CU_pSuite pSuite = NULL;
	if (CUE_SUCCESS != CU_initialize_registry())
	{
    	return CU_get_error();
	}	
	pSuite = CU_add_suite("tests", NULL, NULL);
	if(NULL == CU_add_test(pSuite, "tests" , tests))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
	CU_basic_run_tests();
	CU_basic_show_failures(CU_get_failure_list());
	CU_cleanup_registry();
	return 0;
	
}
