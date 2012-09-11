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
      if (left.fill == true) {
        CU_ASSERT_TRUE(left.x <= current.x);
      }
      if (right.fill == true) {
        CU_ASSERT_TRUE(right.x >= current.x);
      }
    } else if (level2 == 1) { 
      if (left.fill == true) {
        CU_ASSERT_TRUE(left.y <= current.y);
      }
      if (right.fill == true) {
        CU_ASSERT_TRUE(right.y >= current.y);
      }			
    } else if (level2 == 2) {
      if (left.fill == true) {
        CU_ASSERT_TRUE(left.z <= current.z); 
      }
      if (right.fill == true) {
        CU_ASSERT_TRUE(right.z >= current.z);
      }                  
    }
  }
}

void generate_data(int n) {
  char args[1024];
 
  sprintf(args, "./tests/data/generate_data.rb data_size_%d.txt %d", n, n);
  system(args);  
}

void testKD_BUILD_STATIC(void) {
  Kdtree *data = NULL, *kd_tree = NULL;
  int kd_tree_size, data_size, n;
  char filename[1024];
	
  CU_ASSERT((data_size = kd_read("tests/data/data1.txt", &data)) >= 0);
  CU_ASSERT(0 == kd_build(data, &kd_tree, data_size, &kd_tree_size, 0));
  check_invariant(kd_tree, kd_tree_size);
  free(data);
  free(kd_tree);

}

void testKD_BUILD_RANDOM_100(void) {
  Kdtree *data = NULL, *kd_tree = NULL;
  int kd_tree_size, data_size, n;
  char filename[1024];

  n = 100;
  generate_data(n);
  sprintf(filename, "tests/data/data_size_%d.txt", n);

  CU_ASSERT((data_size = kd_read(filename, &data)) >= 0);
  CU_ASSERT(0 == kd_build(data, &kd_tree, data_size, &kd_tree_size, 0));
  check_invariant(kd_tree, kd_tree_size);
  free(data);
  free(kd_tree);
}

void testKD_BUILD_RANDOM_500(void) {
  Kdtree *data = NULL, *kd_tree = NULL;
  int kd_tree_size, data_size, n;
  char filename[1024];

  n = 500;
  generate_data(n);
  sprintf(filename, "tests/data/data_size_%d.txt", n);

  CU_ASSERT((data_size = kd_read(filename, &data)) >= 0);
  CU_ASSERT(0 == kd_build(data, &kd_tree, data_size, &kd_tree_size, 0));
  check_invariant(kd_tree, kd_tree_size);
  free(data);
  free(kd_tree);
}

void testKD_BUILD_RANDOM_1000(void) {
  Kdtree *data = NULL, *kd_tree = NULL;
  int kd_tree_size, data_size, n;
  char filename[1024];

  n = 1000;
  generate_data(n);
  sprintf(filename, "tests/data/data_size_%d.txt", n);

  CU_ASSERT((data_size = kd_read(filename, &data)) >= 0);
  CU_ASSERT(0 == kd_build(data, &kd_tree, data_size, &kd_tree_size, 0));
  check_invariant(kd_tree, kd_tree_size);
  free(data);
  free(kd_tree);
}

void testKD_BUILD_RANDOM_5000(void) {
  Kdtree *data = NULL, *kd_tree = NULL;
  int kd_tree_size, data_size, n;
  char filename[1024];

  n = 5000;
  generate_data(n);
  sprintf(filename, "tests/data/data_size_%d.txt", n);

  CU_ASSERT((data_size = kd_read(filename, &data)) >= 0);
  CU_ASSERT(0 == kd_build(data, &kd_tree, data_size, &kd_tree_size, 0));
  check_invariant(kd_tree, kd_tree_size);
  free(data);
  free(kd_tree);
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

  if ((NULL == CU_add_test(pSuite, "test of kd_build() [BUILD_STATIC]", testKD_BUILD_STATIC)) ||
      (NULL == CU_add_test(pSuite, "test of kd_build() [BUILD_RANDOM_100]", testKD_BUILD_RANDOM_100)) ||
      (NULL == CU_add_test(pSuite, "test of kd_build() [BUILD_RANDOM_500]", testKD_BUILD_RANDOM_500)) ||
      (NULL == CU_add_test(pSuite, "test of kd_build() [BUILD_RANDOM_1000]", testKD_BUILD_RANDOM_1000)) ||
      (NULL == CU_add_test(pSuite, "test of kd_build() [BUILD_RANDOM_5000]", testKD_BUILD_RANDOM_5000))) {

    CU_cleanup_registry();
    return CU_get_error();
  }


  /* Run all tests using the CUnit Basic interface */
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
