#include <stdio.h>
#include <stdlib.h>
#include "kdtree.h"


/* qsort x coordinate comparison function */ 
int x_cmp(const void *a, const void *b) 
{ 
	const Kdtree *ia = (const Kdtree *)a;  
    const Kdtree *ib = (const Kdtree *)b;
    return ia->x - ib->x;
}

/* qsort y coordinate comparison function */ 
int y_cmp(const void *a, const void *b) 
{ 
	const Kdtree *ia = (const Kdtree *)a;
    const Kdtree *ib = (const Kdtree *)b;
    return ia->y - ib->y;
}

/* qsort z coordinate comparison function */ 
int z_cmp(const void *a, const void *b) 
{ 
	const Kdtree *ia = (const Kdtree *)a;
    const Kdtree *ib = (const Kdtree *)b;
    return ia->z - ib->z;
}


Kdtree *kd_build(Kdtree *tree, int size, int axis) {
	// Sorting points by coordinate
	if (axis == 0) {
		qsort(tree, size, sizeof(Kdtree), x_cmp);
	} else if (axis == 1) {
		qsort(tree, size, sizeof(Kdtree), y_cmp);
	} else if (axis == 2) {
		qsort(tree, size, sizeof(Kdtree), z_cmp);
	}
	//kd_build(tree)
}

void kd_print(Kdtree *tree, int size) {
	int i;
	printf("Print tree\n");
	for(i = 0; i < size; i++) {
		printf("(%.2f, %.2f, %.2f) (%.2f, %.2f)\n", tree[i].x, tree[i].y, tree[i].z, tree[i].v, tree[i].p);
	}
}

void kd_free(Kdtree *tree) {
	free(tree);
}

int main(int argc, char **argv) {
	Kdtree *tree;
	int i, size;
	float x, y, z, v, p;
	
	FILE *stream = fopen("data.txt", "r");
	// Read number of points
	fscanf(stream, "%d", &size);
	
	// Allocation memory block for all points
	tree = (Kdtree*)malloc(size * sizeof(Kdtree));
	
	// Read points from file
	for(i = 0; fscanf(stream, "%f %f %f %f %f", &x, &y, &z, &v, &p) != EOF && i < size; i++) {
		tree[i].x = x; tree[i].y = y; tree[i].z = z; 
		tree[i].v = v; tree[i].p = p; 
	}
	
	// Print tree
	kd_print(tree, size);
	
	// Build tree from array
	kd_build(tree, size, 0); 
	
	kd_print(tree, size);
	
	kd_free(tree);

 	return 0;

}
