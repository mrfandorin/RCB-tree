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
	kd_read("tests/data/data_size_500000.gprof", &data);
  ftime(&tmb2);
	printf("kd_read: %ds\n", (tmb2.time * 1000 + tmb2.millitm) - (tmb1.time * 1000 + tmb1.millitm));
  
  ftime(&tmb1);
  kd_build(&data, &kd_tree);
  ftime(&tmb2);
	printf("kd_build: %ds\n", (tmb2.time * 1000 + tmb2.millitm) - (tmb1.time * 1000 + tmb1.millitm));
	

  

  return 0;
}
