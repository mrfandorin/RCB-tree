#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/timeb.h>
#include <math.h>
#include "kdtree.h"
#include "stack.h"

int main(int argc, char **argv) {
  Kdtree data, kd_tree;
  int *neighbors;
  Node point;
  float radius;
  int i,j, n;
  int median;

  int left, right;
  struct timeb tmb1, tmb2;

  ftime(&tmb1);
  kd_read("tests/data/data_size_5000.gprof", &data);
  ftime(&tmb2);
  printf("kd_read: %ds\n", (tmb2.time * 1000 + tmb2.millitm) - (tmb1.time * 1000 + tmb1.millitm));
  
  ftime(&tmb1);
  kd_build(&data, &kd_tree);
  ftime(&tmb2);
  printf("kd_build: %ds\n", (tmb2.time * 1000 + tmb2.millitm) - (tmb1.time * 1000 + tmb1.millitm));
	
  point.coords[0] = 1000;//rand() % 1000;
  point.coords[1] = 900;//rand() % 1000;
  point.coords[2] = 800;//rand() % 1000;
  radius = 1000;//rand() % 1000;
  
  ftime(&tmb1);
 // kd_nn_search(&kd_tree, &point, radius, &neighbors);
  kd_nn_search_all(&kd_tree, radius);
  ftime(&tmb2);
  printf("nn_search: %ds\n", (tmb2.time * 1000 + tmb2.millitm) - (tmb1.time * 1000 + tmb1.millitm));
  
//  kd_print(&kd_tree);

  return 0;
}
