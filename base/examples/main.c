#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "kdtree.h"
#include "stack.h"
/*void swap(Node *x, Node *y) {
  Node temp = *x;
  *x = *y;
  *y = temp;
}

void partition(Node *arr, int left, int right, int pivot_idx) {
  float pivot_val = arr[pivot_idx].x;
  int store_idx, i;

  swap(&arr[pivot_idx], &arr[right]);

  store_idx = left;

  for(i = left; i < right - 1; i++) {
    if (arr[i].x <= pivot_val) {
      swap(&arr[store_idx], &arr[i]);
      store_idx++;
    }
  }
  swap(&arr[right], &arr[store_idx]);
}

int quick_select(Node *arr, int low, int high) {
    int median;
    int middle, ll, hh;

    median = (low + high) / 2;
    for (;;) {
        if (high <= low)
            return median;//arr[median] ;

        if (high == low + 1) {
            if (arr[low].x > arr[high].x)
                swap(&arr[low], &arr[high]) ;
            return median;//arr[median] ;
        }

    middle = (low + high) / 2;
    if (arr[middle].x > arr[high].x)    swap(&arr[middle], &arr[high]) ;
    if (arr[low].x > arr[high].x)       swap(&arr[low], &arr[high]) ;
    if (arr[middle].x > arr[low].x)     swap(&arr[middle], &arr[low]) ;

    swap(&arr[middle], &arr[low+1]) ;

    ll = low + 1;
    hh = high;
    for (;;) {
        do ll++; while (arr[low].x > arr[ll].x) ;
        do hh--; while (arr[hh].x  > arr[low].x) ;

        if (hh < ll)
        break;

        swap(&arr[ll], &arr[hh]) ;
    }

    swap(&arr[low], &arr[hh]) ;

    if (hh <= median)
        low = ll;
        if (hh >= median)
        high = hh - 1;
    }
}
*/
int main(int argc, char **argv) {
  Kdtree data, kd_tree;
  int *neighbors;
  Node point;
  float radius;
	int i,j, n;
  int median;

  int left, right;

  kd_read("tests/data/data_size_500000.gprof", &data);
  kd_build(&data, &kd_tree);

/*  left = 100; right = data.size - 1;
  median = quick_select(data.nodes, left, right);
  printf("median = %d, value = %lf\n", median, data.nodes[median].x);
  partition(data.nodes, left, right, median);
  for(j = left; j < median; j++) {
    if (data.nodes[median].x < data.nodes[j].x) {
      printf("Error\n");
    }
  }
  for(j = median + 1; j < right + 1; j++) {
    if (data.nodes[median].x > data.nodes[j].x) {
      printf("Error\n");
    }
  }
 */

//  kd_print(&kd_tree);
 // kd_free(&data);
//size1=4, size2=3, radius=743.000000, x=83.000000, y=214.000000, z=15.000000
/*	point.x = 83;
	point.y = 214;
	point.z = 15;
	radius = 743;
*/
//	n = kd_nn_search(&kd_tree, &point, radius, &neighbors);

/*	printf("NN size %d\n", n); 
	for(i = 0; i < n; i++) {
		printf("%d ", neighbors[i]);
	}
	printf("\n");
*/
//  kd_nn_search_all(&kd_tree, radius);

  //kd_print(&kd_tree);
	//free(neighbors);
  //kd_free(&kd_tree);

  

  return 0;
}
