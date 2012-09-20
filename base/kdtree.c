#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "kdtree.h"
#include "queue.h"
#include "stack.h"

#define swap(x, y) {Node temp = *x; *x = *y; *y = temp;}

/* qsort x coordinate comparison function */
int x_cmp(const void *a, const void *b)
{
  const Node *ia = (const Node *)a;
  const Node *ib = (const Node *)b;
  return ia->x - ib->x;
}

/* qsort y coordinate comparison function */
int y_cmp(const void *a, const void *b)
{
  const Node *ia = (const Node *)a;
  const Node *ib = (const Node *)b;
  return ia->y - ib->y;
}

/* qsort z coordinate comparison function */
int z_cmp(const void *a, const void *b)
{
  const Node *ia = (const Node *)a;
  const Node *ib = (const Node *)b;
  return ia->z - ib->z;
}

int quickselect(Node *arr, int left, int right, int axis) {
  unsigned long i,ir,j,l,mid,k;
  Node a;

  l=left;
  ir=right;
  k = (l + ir)/2;
  for(;;) {
    if (ir <= l+1) { 
      if (axis == 0) {
        if (ir == l+1 && arr[ir].x < arr[l].x) {
	  swap(&arr[l],&arr[ir]);
        }
      } else if (axis == 1) {
        if (ir == l+1 && arr[ir].y < arr[l].y) {
          swap(&arr[l],&arr[ir]);
        }
      } else if (axis == 2) {
        if (ir == l+1 && arr[ir].z < arr[l].z) {
          swap(&arr[l],&arr[ir]);
        }
      } 

      return k;//arr[k];
    }
    else {
      mid=(l+ir) >> 1; 
      swap(&arr[mid],&arr[l+1]);

      if (axis == 0) {
        if (arr[l].x > arr[ir].x) {
	  swap(&arr[l],&arr[ir]);
        }
        if (arr[l+1].x > arr[ir].x) {
	  swap(&arr[l+1],&arr[ir]);
        }
        if (arr[l].x > arr[l+1].x) {
	  swap(&arr[l],&arr[l+1]);
        }
      } else if (axis == 1) {
        if (arr[l].y > arr[ir].y) {
          swap(&arr[l],&arr[ir]);
        }
        if (arr[l+1].y > arr[ir].y) {
          swap(&arr[l+1],&arr[ir]);
        }
        if (arr[l].y > arr[l+1].y) {
          swap(&arr[l],&arr[l+1]);
        }
      } else if (axis == 2) {
        if (arr[l].z > arr[ir].z) {
          swap(&arr[l],&arr[ir]);
        }
        if (arr[l+1].z > arr[ir].z) {
          swap(&arr[l+1],&arr[ir]);
        }
        if (arr[l].z > arr[l+1].z) {
          swap(&arr[l],&arr[l+1]);
        }
      }


      i=l+1; 
      j=ir;
      a=arr[l+1]; 

      if (axis == 0) {
        for (;;) { 
	  do i++; while (arr[i].x < a.x); 
	  do j--; while (arr[j].x > a.x); 
	  if (j < i) break; 
	  swap(&arr[i],&arr[j]);
        }
      } else if (axis == 1) {
        for (;;) {
          do i++; while (arr[i].y < a.y);
          do j--; while (arr[j].y > a.y);
          if (j < i) break;
          swap(&arr[i],&arr[j]);
        }
      } else if (axis == 2) {
        for (;;) {
          do i++; while (arr[i].z < a.z);
          do j--; while (arr[j].z > a.z);
          if (j < i) break;
          swap(&arr[i],&arr[j]);
        }
      }

      arr[l+1]=arr[j]; 
      arr[j]=a;
      if (j >= k) ir=j-1; 
      if (j <= k) l=i;
    }
  }
}

float distance2(Node *p1, Node *p2) {
        return (p1->x - p2->x) * (p1->x - p2->x) + (p1->y - p2->y) * (p1->y - p2->y) + (p1->z - p2->z) * (p1->z - p2->z);
}

int median(int left, int right) {
  if ((right - left + 1) % 2 == 0) {
    return (right - left + 1) / 2 - 1;
  } 
  return (right - left + 1) / 2;
}

int** generate_interval(int n) {
  int current, i = 0;
  int left = 0, right = n - 1;
  int ind[5];
  int *res;  
  Queue q;

  init_queue(&q, n);

	current = (left + right) / 2; 
  ind[0] = left; ind[1] = right; 
  ind[2] = current; 
  ind[3] = -1; ind[4] = -1;

  enqueue(&q, ind);

  while(!empty(&q)) {
    res = (int*)dequeue(&q);
    left = res[0];
    right = res[1];
    current = (left + right) / 2;
    if (left <= current - 1) {
			ind[0] = left; ind[1] = current - 1;
			ind[2] = (ind[0] + ind[1]) / 2;
      ind[3] = i;
      ind[4] = current;
      enqueue(&q, ind);
    }
    if (current + 1 <= right) {
		  ind[0] = current + 1;
      ind[1] = right;
			ind[2] = (ind[0] + ind[1]) / 2;
      ind[3] = i;
      ind[4] = current;
      enqueue(&q, ind);
    }
    i++;
  }

  return q.q;
}

void kd_nn_search_all(Kdtree *kd_tree, int r) {
  int i, n;
  int *neighbors;

  for(i = 0; i < kd_tree->size; i++) {
    n = kd_nn_search(kd_tree, &(kd_tree->nodes[i]), r, &neighbors);
    kd_tree->nodes[i].neighbors = neighbors;
    kd_tree->nodes[i].neighbors_size = n;
  }
}


int kd_nn_search(Kdtree *kd_tree, Node *point, int r, int **neighbors) {
        Stack s;
        int current, axis, i, j;
        int depth, level;
	float dist2;

        *neighbors = (int*)malloc((kd_tree->size - 1) * sizeof(int));

        depth = ceil(log(kd_tree->size + 1)/log(2));

        init_stack(&s, kd_tree->size);
        push_stack(&s, 0);

	for(i = 0; i < kd_tree->size; i++) {
		kd_tree->nodes[i].closed = false;
	}
        i = 0; j = 0;
        while(!empty_stack(&s)) {
                current = pop_stack(&s);

                level = (int)floor(log(2 * (current + 1))/log(2)) - 1;

                if (kd_tree->nodes[current].fill == false) continue;
                axis = level % 3;

		dist2 = distance2(&(kd_tree->nodes[current]), point);
		if (kd_tree->nodes[current].closed == false && r * r >= dist2 && dist2 != 0) {
                	(*neighbors)[i] = current; i++;
                }
		if (depth - 1 != level) {
                	if (axis == 0) {                        
                                if (kd_tree->nodes[current].x >= point->x - r && kd_tree->nodes[current].x <= point->x + r) {
                                        if (kd_tree->nodes[2 * current + 2].closed == false) push_stack(&s, 2 * current + 2);
                                        if (kd_tree->nodes[2 * current + 1].closed == false) push_stack(&s, 2 * current + 1);
                                } else if (kd_tree->nodes[current].x > point->x - r) {
					if (kd_tree->nodes[2 * current + 1].closed == false) push_stack(&s, 2 * current + 1);
				} else if (kd_tree->nodes[current].x < point->x + r) {
                                        if (kd_tree->nodes[2 * current + 2].closed == false) push_stack(&s, 2 * current + 2);
                                }
                	} else if (axis == 1) {
                                if (kd_tree->nodes[current].y >= point->y - r && kd_tree->nodes[current].y <= point->y + r) {
                                        if (kd_tree->nodes[2 * current + 2].closed == false) push_stack(&s, 2 * current + 2);
                                        if (kd_tree->nodes[2 * current + 1].closed == false) push_stack(&s, 2 * current + 1);
                                } else if (kd_tree->nodes[current].y > point->y - r) {
                                        if (kd_tree->nodes[2 * current + 1].closed == false) push_stack(&s, 2 * current + 1);
                                }
                                if (kd_tree->nodes[current].y < point->y + r) {
                                        if (kd_tree->nodes[2 * current + 2].closed == false) push_stack(&s, 2 * current + 2);
                                }
			} else if (axis == 2) {
                                if (kd_tree->nodes[current].z >= point->z - r && kd_tree->nodes[current].z <= point->z + r) {
                                        if (kd_tree->nodes[2 * current + 2].closed == false) push_stack(&s, 2 * current + 2);
                                        if (kd_tree->nodes[2 * current + 1].closed == false) push_stack(&s, 2 * current + 1);
                                } else if (kd_tree->nodes[current].z > point->z - r) {
                                        if (kd_tree->nodes[2 * current + 1].closed == false) push_stack(&s, 2 * current + 1);
                                } else if (kd_tree->nodes[current].z < point->z + r) {
                                        if (kd_tree->nodes[2 * current + 2].closed == false) push_stack(&s, 2 * current + 2);
                                }
                	}
		}
		kd_tree->nodes[current].closed = true;
		j++;
        }
        return i;

}

int kd_build(Kdtree *data, Kdtree *kd_tree) {
  int axis, i, level, c_level;
  int left, right, current, c_index, parent, parent_value;
	int ind[5];
	int *res;  
  Queue q;

  if (data->size == 0) {
    kd_tree->nodes = NULL;
    kd_tree->size = 0;
    return 0;
  }

  level = ceil(log(data->size + 1)/log(2));
  kd_tree->size = (int)pow(2, level) - 1;
  
  kd_tree->nodes = (Node*)calloc(kd_tree->size, sizeof(Node));

  if (NULL == kd_tree->nodes) return -1;

  init_queue(&q, data->size);
	
	left = 0; right = data->size - 1;
	
	current = (left + right) / 2; 
  ind[0] = left; ind[1] = right; 
  ind[2] = current; 
  ind[3] = -1; ind[4] = -1;

  enqueue(&q, ind);

	i = 0;
  while(!empty(&q)) {
    res = (int*)dequeue(&q);
    left = res[0];
    right = res[1];
    current = (left + right) / 2;

		c_level = ((int)floor(log(2 * (i + 1))/log(2)) - 1);

    axis = c_level % 3;
    c_index = i;

    current = res[2]; 
    parent = res[3]; parent_value = res[4];

    if (c_level == level - 1 && ((data->size + 1) & data->size) != 0) {
      c_index = 2 * parent + 2;
      if (current <= parent_value) {
        c_index = 2 * parent + 1;
      }
    } else {
      quickselect(data->nodes, left, right, axis);
    }
    memcpy(&(kd_tree->nodes[c_index]), &(data->nodes[current]), sizeof(Node));
    kd_tree->nodes[c_index].fill = true;

    if (left <= current - 1) {
			ind[0] = left; ind[1] = current - 1;
			ind[2] = (ind[0] + ind[1]) / 2;
      ind[3] = i;
      ind[4] = current;
      enqueue(&q, ind);
    }
    if (current + 1 <= right) {
		  ind[0] = current + 1;
      ind[1] = right;
			ind[2] = (ind[0] + ind[1]) / 2;
      ind[3] = i;
      ind[4] = current;
      enqueue(&q, ind);
    }
    i++;
  }

   // Free array of indexes
  /*for(i = 0; i < data->size; i++) {
    free(q.q[i]);
  }
  free(q.q);
  */
  return 0;
}

int kd_build_recursive(Kdtree *data, Kdtree *kd_tree) {
  kd_tree->nodes = NULL;
  if (data->size == 0) kd_tree->size = 0;
  return kd_build_recursive_iter(data->nodes, kd_tree, data->size, 0);
}

int kd_build_recursive_iter(Node *tree, Kdtree *kd_tree, int size, int c_index) {
  int m_index, m_size; // median index
  int axis;

  if (size == 0) return 0;

  if (kd_tree->nodes == NULL) {
    // Full number points in kd tree, by power 2
    kd_tree->size = (int)pow(2, ceil(log(size + 1)/log(2))) - 1;
    // Allocation memory block for kd tree
    kd_tree->nodes = (Node*)calloc(kd_tree->size, sizeof(Node));

    if (kd_tree->nodes == NULL) return 1;
  }
  
  axis = ((int)floor(log(2 * (c_index + 1))/log(2)) - 1) % 3;

  if (axis == 0) {
    qsort(tree, size, sizeof(Node), x_cmp);
  } else if (axis == 1) {
    qsort(tree, size, sizeof(Node), y_cmp);
  } else if (axis == 2) {
    qsort(tree, size, sizeof(Node), z_cmp);
  }

  m_index = size / 2;
  if (size % 2 != 0) m_index += 1;
  m_index--;
  m_size = m_index;
  if (size % 2 == 0) m_size += 1;

  // Add a new point
  memcpy(&(kd_tree->nodes[c_index]), &(tree[m_index]), sizeof(Node));
  kd_tree->nodes[c_index].fill = true;

  kd_build_recursive_iter(tree, kd_tree, m_index, 2 * (c_index + 1) - 1);
  kd_build_recursive_iter(tree + m_index + 1, kd_tree, m_size, 2 * (c_index + 1));

  return 0;
}


void kd_print(Kdtree *tree) {
  int i, j;

  printf("Print tree\n");
  for(i = 0; i < tree->size; i++) {
    printf("%d (%.2f, %.2f, %.2f) (%.2f, %.2f) ", i, tree->nodes[i].x, tree->nodes[i].y, 
           tree->nodes[i].z, tree->nodes[i].v, tree->nodes[i].p);
    printf("neighbors: ");
    for(j = 0; j < tree->nodes[i].neighbors_size; j++) {
	printf("%d ", tree->nodes[i].neighbors[j]);
    } 
    printf("\n");
  }
}

void kd_free(Kdtree *tree) {
  if (tree->nodes != NULL) {
    free(tree->nodes);
  }
}

int kd_read(char *filename, Kdtree *data) {
  int i, rc, size;
  float x, y, z, v, p;

  FILE *stream = fopen(filename, "r");
  if (stream == NULL) {
    data->nodes = NULL;
    data->size = 0;
    return -1;
  }    

  // Read number of points
  if (fscanf(stream, "%d", &size) != 1) {
    fclose(stream);
    return -2;
  }
   
  if (size == 0) {
    data->nodes = NULL;
    data->size = 0;
    return 0;
  }
 
  // Allocation memory block for all points
  data->nodes = (Node*)calloc(size, sizeof(Node));
  data->size = size;

  if (data->nodes == NULL) {
    fclose(stream);
    return -3;
  }
  
   // Read points from file
  for(i = 0; i < size; i++) {
    rc = fscanf(stream, "%f %f %f %f %f", &x, &y, &z, &v, &p);
    if (rc != 5) {
      fclose(stream);
      free(data->nodes);
      return -2;
    }
    data->nodes[i].x = x; data->nodes[i].y = y; data->nodes[i].z = z;
    data->nodes[i].v = v; data->nodes[i].p = p;
  }

  fclose(stream);

  return size;
}
