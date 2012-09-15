#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "kdtree.h"



int main(int argc, char **argv) {
  Kdtree data, kd_tree;
  Node *neighbors = NULL;
  Node point;
  float radius;

  kd_read("tests/data/data1.txt", &data);
  kd_build(&data, &kd_tree);
  kd_print(&kd_tree);
  kd_free(&data);
  
  point.x = 0.0;
  point.y = 1.0;
  point.z = 2.0;
  radius = 10.0;
  kd_nn_search(&kd_tree, &point, radius, 0, &neighbors, 0);

  kd_free(&kd_tree);

  

  return 0;
}
