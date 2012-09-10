#include <stdio.h>
#include <stdlib.h>
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
  Kdtree *data = NULL;

  CU_ASSERT(kd_read("tests/data/data.txt", &data) > 0);
  CU_ASSERT(NULL != data);
  free(data);
}

void testKD_READ_NO_EXIST_FILE(void) {
  Kdtree *data = NULL;
  int size;
  CU_ASSERT(kd_read("tests/data/data_no_exist.txt", &data) == -1);
  free(data);
}

void testKD_READ_NO_VALID(void) {
  Kdtree *data = NULL;
  CU_ASSERT(kd_read("tests/data/data_no_valid.txt", &data) == -2);
  free(data);
}

int check_invariant(Kdtree *kd_tree, int kd_tree_size) {
	int i;
	int level, level2;
	Kdtree current, left, right;

	level = (int)floor(log(kd_tree_size)/log(2));
        for(i = 0; i < kd_tree_size - (int)pow(2, level); i++) {
                current = kd_tree[i];
                left = kd_tree[2 * (i + 1) - 1];
                right = kd_tree[2 * (i + 1)];
                level2 = ((int)floor(log(2 * (i + 1))/log(2)) - 1) % 3;
                if (level2 == 0) {
                        CU_ASSERT_TRUE(left.x <= current.x && right.x >= current.x);			
                } else if (level2 == 1) {
                        CU_ASSERT_TRUE(left.y <= current.y && right.y >= current.y);
                }       else if (level2 == 2) {
                        CU_ASSERT_TRUE(left.z <= current.z && right.z >= current.z);
                }
        }

}


void testKD_BUILD(void) {
  Kdtree *data = NULL, *kd_tree = NULL;
  int kd_tree_size, data_size;
	
  CU_ASSERT((data_size = kd_read("tests/data/data1.txt", &data)) >= 0);
  CU_ASSERT(0 == kd_build(data, &kd_tree, data_size, &kd_tree_size, 0));
  check_invariant(kd_tree, kd_tree_size);
  free(data);
  free(kd_tree);
  
  data = NULL; kd_tree = NULL;
  CU_ASSERT((data_size = kd_read("tests/data/data2.txt", &data)) >= 0);
  CU_ASSERT(0 == kd_build(data, &kd_tree, data_size, &kd_tree_size, 0));
  check_invariant(kd_tree, kd_tree_size);
  free(data);
  free(kd_tree);

  data = NULL; kd_tree = NULL;
  CU_ASSERT((data_size = kd_read("tests/data/data3.txt", &data)) >= 0);
  CU_ASSERT(0 == kd_build(data, &kd_tree, data_size, &kd_tree_size, 0));
  check_invariant(kd_tree, kd_tree_size);
  free(data);
  free(kd_tree);

  //kd_print(kd_tree, kd_tree_size);

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
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
