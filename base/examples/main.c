#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "kdtree.h"
#include "stack.h"

int main(int argc, char **argv) {
  Kdtree data, kd_tree;
  int *neighbors;
  Node point;
  float radius;
	int i, n;

  kd_read("tests/data/data1.txt", &data);
  kd_build(&data, &kd_tree);
//  kd_print(&kd_tree);
  kd_free(&data);
//size1=4, size2=3, radius=743.000000, x=83.000000, y=214.000000, z=15.000000
	point.x = 83;
	point.y = 214;
	point.z = 15;
	radius = 743;
//	n = kd_nn_search(&kd_tree, &point, radius, &neighbors);

/*	printf("NN size %d\n", n); 
	for(i = 0; i < n; i++) {
		printf("%d ", neighbors[i]);
	}
	printf("\n");
*/
  kd_nn_search_all(&kd_tree, radius);

  kd_print(&kd_tree);
	//free(neighbors);
  kd_free(&kd_tree);

  

  return 0;
}
