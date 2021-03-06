#ifndef _KDTREE_H_
#define _KDTREE_H_

#include "stack.h"

#define true 1
#define false 0

#define N_SIZE 100

typedef char bool;

typedef struct Node {
  float coords[3];
  bool fill;
  bool closed;
  int *neighbors;
  int neighbors_size;
	
  // data
  float velocity[3]; // velocity
  float p; // pressure
		
} Node;


typedef struct Kdtree {
  Node *nodes;
  int size;

} Kdtree;

int kd_read(char *filename, Kdtree *p);

int kd_build(Kdtree *tree, Kdtree *kd_tree);


int kd_nn_search(Kdtree *kd_tree, Node *point, float r, int **neighbors, Stack *s);

void kd_nn_search_all(Kdtree *kd_tree, float r);

float distance2(Node *p1, Node *p2);

void kd_print(Kdtree *tree);

void kd_free(Kdtree *tree);



#endif	/* _KDTREE_H_ */

