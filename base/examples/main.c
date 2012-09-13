#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kdtree.h"

int main(int argc, char **argv) {

  Kdtree data, kd_tree;

  printf("Iterative:\n");
  kd_read("tests/data/data1.txt", &data);
  kd_build(&data, &kd_tree);
  kd_print(&kd_tree);
  kd_free(&data);
  kd_free(&kd_tree);

  printf("Recursive:\n");
  kd_read("tests/data/data1.txt", &data);
  kd_tree.nodes = NULL;
  kd_build_recursive(data.nodes, &kd_tree, data.size, 0);
  kd_print(&kd_tree);
  kd_free(&data);
  kd_free(&kd_tree);


}
