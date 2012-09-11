#ifndef _KDTREE_H_
#define _KDTREE_H_

#define true 1
#define false 0
typedef char bool;

typedef struct Kdtree {
  float x;
  float y;
  float z;
  bool fill;
	
  // data
  float v; // velocity
  float p; // pressure
		
} Kdtree;

int kd_read(char *filename, Kdtree **p);

/* Build the kd-tree */
int kd_build(Kdtree *tree, Kdtree **kd_tree, int size, int *kd_tree_size, int c_index);

/* Print the kdtree */
void kd_print(Kdtree *tree, int size);

/* Free the kdtree */
void kd_free(Kdtree *tree);



#endif	/* _KDTREE_H_ */

