#ifndef _KDTREE_H_
#define _KDTREE_H_

#define true 1
#define false 0
typedef char bool;

typedef struct Node {
  float x;
  float y;
  float z;
  bool fill;
  bool closed;
  int *neighbors;
  int neighbors_size;
	
  // data
  float v; // velocity
  float p; // pressure
		
} Node;


typedef struct Kdtree {
  Node *nodes;
  int size;

} Kdtree;

int kd_read(char *filename, Kdtree *p);

int kd_build(Kdtree *tree, Kdtree *kd_tree);

int kd_recursive(Kdtree *tree, Kdtree *kd_tree);

int kd_build_recursive_iter(Node *tree, Kdtree *kd_tree, int size, int c_index);

int kd_nn_search(Kdtree *kd_tree, Node *point, int r, int **neighbors);

void kd_nn_search_all(Kdtree *kd_tree, int r);

float distance2(Node *p1, Node *p2);

void kd_print(Kdtree *tree);

void kd_free(Kdtree *tree);



#endif	/* _KDTREE_H_ */

