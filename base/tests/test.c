#include <stdio.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "kdtree.h"

int init_suite(void) {
  return 0;
}

int clean_suite(void) {
  return 0;
}


void testKD_READ_SUCCESS(void) {
  Kdtree *data;

  //CU_ASSERT(kd_read("tests/data/data.txt", data) > 0);
  //CU_ASSERT(NULL != data);
}

void testKD_READ_NO_EXIST_FILE(void) {
  Kdtree *data;
  int size;
//  CU_ASSERT(kd_read("tests/data/data_no_exist.txt", data) == -1);
}

void testKD_READ_NO_VALID(void) {
  Kdtree *data;
  //CU_ASSERT(kd_read("tests/data/data_no_valid.txt", data) == -2);
}

void testKD_BUILD(void) {
  Kdtree *data, *kd_tree;
  int size;

  CU_ASSERT(size = kd_read("tests/data/data1.txt", &data) > 0);
  CU_ASSERT(0 == kd_build(data, kd_tree, 3, 0, 0));

  kd_print(kd_tree, 3);
}

int main() {
  CU_pSuite pSuite = NULL;

   /* initialize the CUnit test registry */
  if (CUE_SUCCESS != CU_initialize_registry()) {
    return CU_get_error();
  }
  
  /* add a suite to the registry */
  pSuite = CU_add_suite("Suite_1", init_suite, clean_suite);
  if (NULL == pSuite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  /* add the tests to the suite */
  if ((NULL == CU_add_test(pSuite, "test of kd_read() [NO_EXIST_FILE]", testKD_READ_NO_EXIST_FILE)) ||
      (NULL == CU_add_test(pSuite, "test of kd_read() [NO_VALID]", testKD_READ_NO_VALID)) ||
      (NULL == CU_add_test(pSuite, "test of kd_read() [SUCCESS]", testKD_READ_SUCCESS))) {

    CU_cleanup_registry();
    return CU_get_error();
  }

  pSuite = CU_add_suite("Suite_2", init_suite, clean_suite);
  if (NULL == pSuite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if ((NULL == CU_add_test(pSuite, "test of kd_build() [BUILD]", testKD_BUILD))) {

    CU_cleanup_registry();
    return CU_get_error();
  }


  /* Run all tests using the CUnit Basic interface */
  CU_basic_set_mode(CU_BRM_NORMAL);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
