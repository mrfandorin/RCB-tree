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
  Kdtree data;

  CU_ASSERT(kd_read("tests/data/data.txt", &data) > 0);
  CU_ASSERT(NULL != data.nodes);
  kd_free(&data);
}

void testKD_READ_NO_EXIST_FILE(void) {
  Kdtree data;
  
  CU_ASSERT(kd_read("tests/data/data_no_exist.txt", &data) == -1);
}

void testKD_READ_NO_VALID(void) {
  Kdtree data;
  CU_ASSERT(kd_read("tests/data/data_no_valid.txt", &data) == -2);
}

void check_invariant(Kdtree *kd_tree) {
  int i;
  int level, level2;
  Node current, left, right;

  level = (int)floor(log(kd_tree->size)/log(2));
  for(i = 0; i < kd_tree->size - (int)pow(2, level); i++) {
    current = kd_tree->nodes[i];
    left = kd_tree->nodes[2 * (i + 1) - 1];
    right = kd_tree->nodes[2 * (i + 1)];
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

void check_equal_kd_tree(Kdtree *t1, Kdtree *t2) {
  int i;

  for(i = 0; i < t1->size; i++) {
    CU_ASSERT_DOUBLE_EQUAL(t1->nodes[i].x, t2->nodes[i].x, 0);
    CU_ASSERT_DOUBLE_EQUAL(t1->nodes[i].y, t2->nodes[i].y, 0);
    CU_ASSERT_DOUBLE_EQUAL(t1->nodes[i].z, t2->nodes[i].z, 0);
    CU_ASSERT_DOUBLE_EQUAL(t1->nodes[i].v, t2->nodes[i].v, 0);
    CU_ASSERT_DOUBLE_EQUAL(t1->nodes[i].p, t2->nodes[i].p, 0);
    CU_ASSERT_EQUAL(t1->nodes[i].fill, t2->nodes[i].fill);
  }

}

void generate_data(int n) {
  char args[1024];
 
  sprintf(args, "./tests/data/generate_data.rb data_size_%d.txt %d", n, n);
  system(args);  
}

void testKD_BUILD_STATIC(void) {
  Kdtree data, kd_tree1, kd_tree2;
  int data_size;
	
  CU_ASSERT((data_size = kd_read("tests/data/data1.txt", &data)) >= 0);
  CU_ASSERT(0 == kd_build(&data, &kd_tree1));
  check_invariant(&kd_tree1);

  CU_ASSERT((data_size = kd_read("tests/data/data1.txt", &data)) >= 0);
  CU_ASSERT(0 == kd_build(&data, &kd_tree2));
  check_invariant(&kd_tree2);

  kd_free(&data);

  check_equal_kd_tree(&kd_tree1, &kd_tree2);
  kd_free(&kd_tree1);
  kd_free(&kd_tree2);
  
}

void testKD_BUILD_RANDOM_100(void) {
  Kdtree data, kd_tree1, kd_tree2;
  int data_size, n;
  char filename[1024];

  n = 100;
  generate_data(n);
  sprintf(filename, "tests/data/data_size_%d.txt", n);

  CU_ASSERT((data_size = kd_read(filename, &data)) >= 0);
  CU_ASSERT(0 == kd_build(&data, &kd_tree1));
  check_invariant(&kd_tree1);

  CU_ASSERT((data_size = kd_read(filename, &data)) >= 0);
  CU_ASSERT(0 == kd_build(&data, &kd_tree2));
  check_invariant(&kd_tree2);

  kd_free(&data);

  check_equal_kd_tree(&kd_tree1, &kd_tree2);

  kd_free(&kd_tree1);
  kd_free(&kd_tree2);
}

void testKD_BUILD_RANDOM_123(void) {
  Kdtree data, kd_tree1, kd_tree2;
  int data_size, n;
  char filename[1024];

  n = 123;
  generate_data(n);
  sprintf(filename, "tests/data/data_size_%d.txt", n);

  CU_ASSERT((data_size = kd_read(filename, &data)) >= 0);
  CU_ASSERT(0 == kd_build(&data, &kd_tree1));
  check_invariant(&kd_tree1);

  CU_ASSERT((data_size = kd_read(filename, &data)) >= 0);
  CU_ASSERT(0 == kd_build(&data, &kd_tree2));
  check_invariant(&kd_tree2);

  kd_free(&data);

  check_equal_kd_tree(&kd_tree1, &kd_tree2);

  kd_free(&kd_tree1);
  kd_free(&kd_tree2);
}

void testKD_BUILD_RANDOM_256(void) {
  Kdtree data, kd_tree1, kd_tree2;
  int data_size, n;
  char filename[1024];

  n = 256;
  generate_data(n);
  sprintf(filename, "tests/data/data_size_%d.txt", n);

  CU_ASSERT((data_size = kd_read(filename, &data)) >= 0);
  CU_ASSERT(0 == kd_build(&data, &kd_tree1));
  check_invariant(&kd_tree1);

  CU_ASSERT((data_size = kd_read(filename, &data)) >= 0);
  CU_ASSERT(0 == kd_build(&data, &kd_tree2));
  check_invariant(&kd_tree2);

  kd_free(&data);

  check_equal_kd_tree(&kd_tree1, &kd_tree2);

  kd_free(&kd_tree1);
  kd_free(&kd_tree2);
}


void testKD_BUILD_RANDOM_500(void) {
  Kdtree data, kd_tree1, kd_tree2;
  int data_size, n;
  char filename[1024];

  n = 500;
  generate_data(n);
  sprintf(filename, "tests/data/data_size_%d.txt", n);

  CU_ASSERT((data_size = kd_read(filename, &data)) >= 0);
  CU_ASSERT(0 == kd_build(&data, &kd_tree1));
  check_invariant(&kd_tree1);

  CU_ASSERT((data_size = kd_read(filename, &data)) >= 0);
  CU_ASSERT(0 == kd_build(&data, &kd_tree2));
  check_invariant(&kd_tree2);

  kd_free(&data);

  check_equal_kd_tree(&kd_tree1, &kd_tree2);

  kd_free(&kd_tree1);
  kd_free(&kd_tree2);

}

void testKD_BUILD_RANDOM_1000(void) {
  Kdtree data, kd_tree1, kd_tree2;
  int data_size, n;
  char filename[1024];

  n = 1000;
  generate_data(n);
  sprintf(filename, "tests/data/data_size_%d.txt", n);

  CU_ASSERT((data_size = kd_read(filename, &data)) >= 0);
  CU_ASSERT(0 == kd_build(&data, &kd_tree1));
  check_invariant(&kd_tree1);

  CU_ASSERT((data_size = kd_read(filename, &data)) >= 0);
  CU_ASSERT(0 == kd_build(&data, &kd_tree2));
  check_invariant(&kd_tree2);

  kd_free(&data);

  check_equal_kd_tree(&kd_tree1, &kd_tree2);

  kd_free(&kd_tree1);
  kd_free(&kd_tree2);

}

void testKD_BUILD_RANDOM_1024(void) {
  Kdtree data, kd_tree1, kd_tree2;
  int data_size, n;
  char filename[1024];

  n = 1024;
  generate_data(n);
  sprintf(filename, "tests/data/data_size_%d.txt", n);

  CU_ASSERT((data_size = kd_read(filename, &data)) >= 0);
  CU_ASSERT(0 == kd_build(&data, &kd_tree1));
  check_invariant(&kd_tree1);

  CU_ASSERT((data_size = kd_read(filename, &data)) >= 0);
  CU_ASSERT(0 == kd_build(&data, &kd_tree2));
  check_invariant(&kd_tree2);

  kd_free(&data);

  check_equal_kd_tree(&kd_tree1, &kd_tree2);

  kd_free(&kd_tree1);
  kd_free(&kd_tree2);
}


void testKD_BUILD_RANDOM_5000(void) {
  Kdtree data, kd_tree1, kd_tree2;
  int data_size, n;
  char filename[1024];

  n = 5000;
  generate_data(n);
  sprintf(filename, "tests/data/data_size_%d.txt", n);

  CU_ASSERT((data_size = kd_read(filename, &data)) >= 0);
  CU_ASSERT(0 == kd_build(&data, &kd_tree1));
  check_invariant(&kd_tree1);

  CU_ASSERT((data_size = kd_read(filename, &data)) >= 0);
  CU_ASSERT(0 == kd_build(&data, &kd_tree2));
  check_invariant(&kd_tree2);

  kd_free(&data);

  check_equal_kd_tree(&kd_tree1, &kd_tree2);

  kd_free(&kd_tree1);
  kd_free(&kd_tree2);

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
      (NULL == CU_add_test(pSuite, "test of kd_build() [BUILD_RANDOM_123]", testKD_BUILD_RANDOM_123)) ||
      (NULL == CU_add_test(pSuite, "test of kd_build() [BUILD_RANDOM_256]", testKD_BUILD_RANDOM_256)) ||
      (NULL == CU_add_test(pSuite, "test of kd_build() [BUILD_RANDOM_500]", testKD_BUILD_RANDOM_500)) ||
      (NULL == CU_add_test(pSuite, "test of kd_build() [BUILD_RANDOM_1000]", testKD_BUILD_RANDOM_1000)) ||
      (NULL == CU_add_test(pSuite, "test of kd_build() [BUILD_RANDOM_1024]", testKD_BUILD_RANDOM_1024)) ||
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
