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
	
  // data
  float v; // velocity
  float p; // pressure
		
} Node;


typedef struct Kdtree {
  Node *nodes;
  int size;

} Kdtree;

int kd_read(char *filename, Kdtree *p);

//int kd_build(Kdtree *tree, Kdtree **kd_tree, int size, int *kd_tree_size, int c_index);

void kd_print(Kdtree *tree);

//void kd_free(Kdtree *tree);



#endif	/* _KDTREE_H_ */

