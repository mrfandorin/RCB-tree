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

void testKD_BUILD(void) {
  CU_ASSERT(0 == 0);
}

void testKD_READ_SUCCESS(void) {
  Kdtree *data;
  CU_ASSERT(0 == kd_read("tests/data/data.txt", data));
  CU_ASSERT(NULL != data);
}

void testKD_READ_NO_EXIST_FILE(void) {
  Kdtree *data;
  CU_ASSERT(1 == kd_read("tests/data/data_no_exist.txt", data));
}

void testKD_READ_NO_VALID(void) {
  Kdtree *data;
  CU_ASSERT(2 == kd_read("tests/data/data_no_valid.txt", data));
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

  /* Run all tests using the CUnit Basic interface */
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
