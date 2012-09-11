#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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


int kd_build(Kdtree *tree, Kdtree **kd_tree, int size, int *kd_tree_size, int c_index) {
  // Sorting points by coordinate
  int m_index, m_size; // median index
  int axis, size2;
  Kdtree *tmp;
  int i;

  //int c_index = 0; // current index
  if (size == 0) {
    return;
  }
  
  if (NULL == *kd_tree) {
		
    // Full number points in kd tree, by power 2
    size2 = (int)pow(2, ceil(log(size)/log(2))) - 1;
		
    *kd_tree_size = size2;
		
    // Allocation memory block for kd tree
    *kd_tree = (Kdtree*)calloc(size2, sizeof(Kdtree));
			
    if (NULL == *kd_tree) {
      return 1;
    }
  } 

  axis = ((int)floor(log(2 * (c_index + 1))/log(2)) - 1) % 3;

  if (axis == 0) {
    qsort(tree, size, sizeof(Kdtree), x_cmp);
  } else if (axis == 1) {
    qsort(tree, size, sizeof(Kdtree), y_cmp);
  } else if (axis == 2) {
    qsort(tree, size, sizeof(Kdtree), z_cmp);
  }
  //printf("Step c_index=%d, axis=%d\n", c_index, axis);
  //kd_print(tree, size);	
  m_index = size / 2;
  if (size % 2 != 0) {
    m_index += 1;  
  } 
  m_index--;
  m_size = m_index;
  if (size % 2 == 0) {
    m_size += 1;
  }
  
	// Add a new point
  memcpy(&(*kd_tree)[c_index], &tree[m_index], sizeof(Kdtree));
  (*kd_tree)[c_index].fill = true; 
	
  // Next axis
  axis = (axis + 1) % 3;
  kd_build(tree, kd_tree, m_index, kd_tree_size, 2 * (c_index + 1) - 1);
  kd_build(tree + m_index + 1, kd_tree, m_size, kd_tree_size, 2 * (c_index + 1));	

  return 0;
}

void kd_print(Kdtree *tree, int size) {
  int i;
  printf("Print tree\n");
  for(i = 0; i < size; i++) {
    printf("%d (%.2f, %.2f, %.2f) (%.2f, %.2f)\n", i + 1, tree[i].x, tree[i].y, tree[i].z, tree[i].v, tree[i].p);
  }
}

void kd_free(Kdtree *tree) {
  free(tree);
}

int kd_read(char *filename, Kdtree **p_data) {
  int i, rc, size;
  float x, y, z, v, p;
  Kdtree *data;

  FILE *stream = fopen(filename, "r");
  if (stream == NULL) {
    return -1;
  }    

  // Read number of points
  if (fscanf(stream, "%d", &size) != 1) {
    fclose(stream);
    return -2;
  }
    
  // Allocation memory block for all points
  data = (Kdtree*)calloc(size, sizeof(Kdtree));
  
  if (data == NULL) {
    fclose(stream);
    return -3;
  }
  
   // Read points from file
  for(i = 0; i < size; i++) {
    rc = fscanf(stream, "%f %f %f %f %f", &x, &y, &z, &v, &p);
    if (rc != 5) {
      fclose(stream);
      free(data);
      return -2;
    }
    data[i].x = x; data[i].y = y; data[i].z = z;
    data[i].v = v; data[i].p = p;
  }
  
  *p_data = data;

  fclose(stream);

  return size;
}

/*
int main(int argc, char **argv) {
  Kdtree *tree, *kd_tree;
  int i, size, size2;
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
	
  // Full number points in kd tree, by power 2
  size2 = (int)pow(2, ceil(log(size)/log(2)));

  // Allocation memory block for kd tree
  kd_tree = (Kdtree*)calloc(size2, sizeof(Kdtree));

  // Build kd tree from array
  kd_build(tree, kd_tree, size, 0, 0); 
	
  kd_print(kd_tree, size2);
	
  kd_free(tree);
  kd_free(kd_tree);

   return 0;
}

*/

