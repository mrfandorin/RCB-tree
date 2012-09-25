#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <CUnit/Basic.h>
#include "stack.h"
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
           
      if (left.fill == true) {
        CU_ASSERT_TRUE(left.coords[level2] <= current.coords[level2]);
      }
      if (right.fill == true) {
        CU_ASSERT_TRUE(right.coords[level2] >= current.coords[level2]);
      }
  }
}

void generate_data(int n) {
  char args[1024];
 
  sprintf(args, "./tests/data/generate_data.rb data_size_%d.txt %d", n, n);
  system(args);  
}

void testKD_BUILD_STATIC(void) {
  Kdtree data, kd_tree1;
  int data_size;
	
  CU_ASSERT((data_size = kd_read("tests/data/data1.txt", &data)) >= 0);
  CU_ASSERT(0 == kd_build(&data, &kd_tree1));
  check_invariant(&kd_tree1);

  kd_free(&data);

  kd_free(&kd_tree1);
  
}

void testKD_BUILD_RANDOM_100(void) {
  Kdtree data, kd_tree1;
  int data_size, n;
  char filename[1024];

  n = 100;
  generate_data(n);
  sprintf(filename, "tests/data/data_size_%d.txt", n);

  CU_ASSERT((data_size = kd_read(filename, &data)) >= 0);
  CU_ASSERT(0 == kd_build(&data, &kd_tree1));
  check_invariant(&kd_tree1);

  kd_free(&data);

  kd_free(&kd_tree1);
}

void testKD_BUILD_RANDOM_123(void) {
  Kdtree data, kd_tree1;
  int data_size, n;
  char filename[1024];

  n = 123;
  generate_data(n);
  sprintf(filename, "tests/data/data_size_%d.txt", n);

  CU_ASSERT((data_size = kd_read(filename, &data)) >= 0);
  CU_ASSERT(0 == kd_build(&data, &kd_tree1));
  check_invariant(&kd_tree1);

  kd_free(&data);

  kd_free(&kd_tree1);
}

void testKD_BUILD_RANDOM_256(void) {
  Kdtree data, kd_tree1;
  int data_size, n;
  char filename[1024];

  n = 256;
  generate_data(n);
  sprintf(filename, "tests/data/data_size_%d.txt", n);

  CU_ASSERT((data_size = kd_read(filename, &data)) >= 0);
  CU_ASSERT(0 == kd_build(&data, &kd_tree1));
  check_invariant(&kd_tree1);

  kd_free(&data);

  kd_free(&kd_tree1);
}


void testKD_BUILD_RANDOM_500(void) {
  Kdtree data, kd_tree1;
  int data_size, n;
  char filename[1024];

  n = 500000;
  generate_data(n);
  sprintf(filename, "tests/data/data_size_%d.txt", n);

  CU_ASSERT((data_size = kd_read(filename, &data)) >= 0);
  CU_ASSERT(0 == kd_build(&data, &kd_tree1));
  check_invariant(&kd_tree1);

  kd_free(&data);

  kd_free(&kd_tree1);

}

void testKD_BUILD_RANDOM_1000(void) {
  Kdtree data, kd_tree1;
  int data_size, n;
  char filename[1024];

  n = 1000;
  generate_data(n);
  sprintf(filename, "tests/data/data_size_%d.txt", n);

  CU_ASSERT((data_size = kd_read(filename, &data)) >= 0);
  CU_ASSERT(0 == kd_build(&data, &kd_tree1));
  check_invariant(&kd_tree1);

  kd_free(&data);

  kd_free(&kd_tree1);

}

void testKD_BUILD_RANDOM_1024(void) {
  Kdtree data, kd_tree1;
  int data_size, n;
  char filename[1024];

  n = 1024;
  generate_data(n);
  sprintf(filename, "tests/data/data_size_%d.txt", n);

  CU_ASSERT((data_size = kd_read(filename, &data)) >= 0);
  CU_ASSERT(0 == kd_build(&data, &kd_tree1));
  check_invariant(&kd_tree1);

  kd_free(&data);

  kd_free(&kd_tree1);
}


void testKD_BUILD_RANDOM_5000(void) {
  Kdtree data, kd_tree1;
  int data_size, n;
  char filename[1024];

  n = 5000;
  generate_data(n);
  sprintf(filename, "tests/data/data_size_%d.txt", n);

  CU_ASSERT((data_size = kd_read(filename, &data)) >= 0);
  CU_ASSERT(0 == kd_build(&data, &kd_tree1));
  check_invariant(&kd_tree1);

  kd_free(&data);


  kd_free(&kd_tree1);

}

int kd_nn_search_simple(Kdtree *kd_tree, Node *point, int r, int **neighbors) {
  int i, j;
  float dist2;

  *neighbors = (int*)malloc((kd_tree->size - 1) * sizeof(int)); 

  j = 0;
  for(i = 0; i < kd_tree->size; i++) {
    if (kd_tree->nodes[i].fill == false) continue;
    dist2 = distance2(&(kd_tree->nodes[i]), point);
    if (r * r >= dist2 && dist2 != 0) {
	(*neighbors)[j] = i;
	j++;
    }
  }
  
  return j;
}

int sort_index(const void *x, const void *y) {
  return (*(int*)x - *(int*)y);
}

void testCOMPARE_SIMPLE_SEARCH_STATIC(void) {
  int n1, n2, i;
  Kdtree data, kd_tree;
  int data_size;
  int *neighbors1,* neighbors2;
  Node point;
  float radius;

  CU_ASSERT((data_size = kd_read("tests/data/data1.txt", &data)) >= 0);
  CU_ASSERT(0 == kd_build(&data, &kd_tree));
  kd_free(&data);

  srand(time(NULL));

  point.coords[0] = 1;
  point.coords[1] = 2;
  point.coords[2] = 2;
  radius = rand() % 20;
 
  n1 = kd_nn_search(&kd_tree, &point, radius, &neighbors1);
  qsort(neighbors1, n1, sizeof(int), sort_index); 

  n2 = kd_nn_search_simple(&kd_tree, &point, radius, &neighbors2);
  qsort(neighbors2, n2, sizeof(int), sort_index);

  CU_ASSERT(n1 == n2);

  for(i = 0; i < n1; i++) {
    CU_ASSERT(neighbors1[i] == neighbors2[i]); 
  }

  free(neighbors1);
  free(neighbors2);
  kd_free(&kd_tree);
}

void testCOMPARE_SIMPLE_SEARCH_RANDOM(void) {
  int n1, n2, i, n;
  Kdtree data, kd_tree;
  int data_size;
  int *neighbors1,* neighbors2;
  Node point;
  float radius;
  char filename[1024];

  n = 500;
  generate_data(n);
  sprintf(filename, "tests/data/data_size_%d.txt", n);

  CU_ASSERT((data_size = kd_read(filename, &data)) >= 0);
  CU_ASSERT(0 == kd_build(&data, &kd_tree));
  kd_free(&data);

  point.coords[0] = (float)(rand() % 1000);
  point.coords[1] = (float)(rand() % 1000);
  point.coords[2] = (float)(rand() % 1000);
  radius = (float)(rand() % 1000);

  n1 = kd_nn_search(&kd_tree, &point, radius, &neighbors1);
  qsort(neighbors1, n1, sizeof(int), sort_index);

  n2 = kd_nn_search_simple(&kd_tree, &point, radius, &neighbors2);
  qsort(neighbors2, n2, sizeof(int), sort_index);
  //printf("\nsize1=%d, size2=%d, radius=%lf, x=%lf, y=%lf, z=%lf\n", n1, n2, radius, point.x, point.y, point.z);
  CU_ASSERT_FATAL(n1 == n2);
  for(i = 0; i < n1; i++) {
    CU_ASSERT(neighbors1[i] == neighbors2[i]);
  }

  free(neighbors1);
  free(neighbors2);
  kd_free(&kd_tree);
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

  pSuite = CU_add_suite("Suite_3", init_suite, clean_suite);
  if (NULL == pSuite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if ((NULL == CU_add_test(pSuite, "test of kd_nn_search() [COMPARE_SIMPLE_SEARCH_STATIC]", testCOMPARE_SIMPLE_SEARCH_STATIC)) || 
      (NULL == CU_add_test(pSuite, "test of kd_nn_search() [COMPARE_SIMPLE_SEARCH_RANDOM]", testCOMPARE_SIMPLE_SEARCH_RANDOM))) {
  //    (NULL == CU_add_test(pSuite, "test of kd_nn_search() [COMPARE_SIMPLE_SEARCH]", testKD_READ_SUCCESS))) {

    CU_cleanup_registry();
    return CU_get_error();
  }

  /* Run all tests using the CUnit Basic interface */
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
