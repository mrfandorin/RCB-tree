#ifndef _KDTREE_H_
#define _KDTREE_H_

struct kdtree;

/* Create a kd-tree for k-dimensional data, pointer to array */
struct kdtree *kd_create();

/* Free the kdtree */
void kd_free(struct kdtree *tree);

/* Insert point into kdtree */
int kd_insert(struct kdtree *tree, double *point, void *data);

#endif	/* _KDTREE_H_ */

