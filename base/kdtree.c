#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "kdtree.h"
#include "queue.h"
#include "stack.h"

#define swap(x, y) {Node temp = *x; *x = *y; *y = temp;}

int quickselect(Kdtree *kd_tree, int left, int right, int axis) {
  unsigned long i,ir,j,l,mid,k;
  Node a;
  Node *arr = kd_tree->nodes;

  l=left;
  ir=right;
  k = (l + ir)/2;
  for(;;) {
    if (ir <= l+1) { 
      
        if (ir == l+1 && arr[ir].coords[axis] < arr[l].coords[axis]) {
	  swap(&arr[l],&arr[ir]);
        }

      return k;
    }
    else {
      mid=(l+ir) >> 1; 
      swap(&arr[mid],&arr[l+1]);

        if (arr[l].coords[axis] > arr[ir].coords[axis]) {
	  swap(&arr[l],&arr[ir]);
        }
        if (arr[l+1].coords[axis] > arr[ir].coords[axis]) {
	  swap(&arr[l+1],&arr[ir]);
        }
        if (arr[l].coords[axis] > arr[l+1].coords[axis]) {
	  swap(&arr[l],&arr[l+1]);

        }
      i=l+1; 
      j=ir;
      a=arr[l+1]; 

        for (;;) { 
	  do i++; while (arr[i].coords[axis] < a.coords[axis]); 
	  do j--; while (arr[j].coords[axis] > a.coords[axis]); 
	  if (j < i) break; 
	  swap(&arr[i],&arr[j]);
        }

      arr[l+1]=arr[j]; 
      arr[j]=a;
      if (j >= k) ir=j-1; 
      if (j <= k) l=i;
    }
  }
}

float distance2(Node *p1, Node *p2) {
  int i;
  float dist = 0;
  for(i = 0; i < 3; i++) {
    dist += (p1->coords[i] - p2->coords[i]) * (p1->coords[i] - p2->coords[i]);
  }
  return dist;
}

void kd_nn_search_all(Kdtree *kd_tree, float r) {
  int i, n;
  int *neighbors;
  Stack s;

  init_stack(&s, kd_tree->size);

  for(i = 0; i < kd_tree->size; i++) {
    //kd_tree->nodes[i].neighbors = (int*)malloc(N_SIZE * sizeof(int));
    n = kd_nn_search(kd_tree, &(kd_tree->nodes[i]), r, &neighbors, &s);
    kd_tree->nodes[i].neighbors = neighbors;
    kd_tree->nodes[i].neighbors_size = n;
  }

  destroy_stack(&s);
}


int kd_nn_search(Kdtree *kd_tree, Node *point, float r, int **neighbors, Stack *s) {
        //Stack s;
        int current, axis, i, j, k;
        int depth, level;
        //int neighbor_num = 100;//kd_tree->size - 1;
	float dist2;

        //*neighbors = (int*)malloc((kd_tree->size - 1) * sizeof(int));
        *neighbors = (int*)malloc(N_SIZE * sizeof(int));

        depth = ceil(log(kd_tree->size + 1)/log(2));



        //init_stack(&s, kd_tree->size);
        
        push_stack(s, 0);

	for(i = 0; i < kd_tree->size; i++) {
		kd_tree->nodes[i].closed = false;
	}
        i = 0; j = 0;
        while(!empty_stack(s) && i < N_SIZE) {
                current = pop_stack(s);

                level = (int)floor(log(2 * (current + 1))/log(2)) - 1;

                if (kd_tree->nodes[current].fill == false) continue;
                axis = level % 3;

		dist2 = distance2(&(kd_tree->nodes[current]), point);
                /*for(k = 0; k < 3; k++) {
                  dist2 += (kd_tree->nodes[current].coords[k] - point->coords[k]) * (kd_tree->nodes[current].coords[k] - point->coords[k]);
                }*/

                 
		if (kd_tree->nodes[current].closed == false && r * r >= dist2 && dist2 != 0) {
                	(*neighbors)[i] = current; 
                        // point->neighbors[i] = current;
                         i++;
                         //printf("%d ", i);
			
                }
		if (depth - 1 != level) {
                                if (kd_tree->nodes[current].coords[axis] >= point->coords[axis] - r && kd_tree->nodes[current].coords[axis] <= point->coords[axis] + r) {
                                        if (kd_tree->nodes[2 * current + 2].closed == false) push_stack(s, 2 * current + 2);
                                        if (kd_tree->nodes[2 * current + 1].closed == false) push_stack(s, 2 * current + 1);
                                } else if (kd_tree->nodes[current].coords[axis] > point->coords[axis] - r) {
					if (kd_tree->nodes[2 * current + 1].closed == false) push_stack(s, 2 * current + 1);
				} else if (kd_tree->nodes[current].coords[axis] < point->coords[axis] + r) {
                                        if (kd_tree->nodes[2 * current + 2].closed == false) push_stack(s, 2 * current + 2);
                                }
		}
		kd_tree->nodes[current].closed = true;
		j++;
        }
        clear_stack(s);
        //destroy_stack(&s);
        return i;

}

int kd_build(Kdtree *data, Kdtree *kd_tree) {
  int axis, i, level, c_level;
  int left, right, current, c_index, parent;
  //int ind[3];
  Interval ind;
  Interval *res;  
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
  ind.left = left; ind.right = right; 
  ind.parent = -1; 
  enqueue(&q, &ind);
   
  i = 0;
  while(!empty(&q)) {
    res = (Interval*)dequeue(&q);

    left = res->left;
    right = res->right;
    current = (left + right) / 2;

    c_level = ((int)floor(log(2 * (i + 1))/log(2)) - 1);

    axis = c_level % 3;
    c_index = i;

    parent = res->parent; 

    if (c_level == level - 1 && ((data->size + 1) & data->size) != 0) {
      c_index = 2 * parent + 2;
    } else {
      quickselect(data, left, right, axis);
    }
    memcpy(&(kd_tree->nodes[c_index]), &(data->nodes[current]), sizeof(Node));
    kd_tree->nodes[c_index].fill = true;

    if (left <= current - 1) {
      ind.left = left; ind.right = current - 1;
      ind.parent = i;
      enqueue(&q, &ind);
    }
    if (current + 1 <= right) {
      ind.left = current + 1;
      ind.right = right;
      ind.parent = i;
      enqueue(&q, &ind);
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

void kd_print(Kdtree *tree) {
  int i, j;

  printf("Print tree\n");
  for(i = 0; i < tree->size; i++) {
    printf("%d coords (%.2f, %.2f, %.2f), velocity (%.2f, %.2f, %.2f), pressure %.2f ", i, tree->nodes[i].coords[0], tree->nodes[i].coords[1], tree->nodes[i].coords[2], tree->nodes[i].velocity[0], tree->nodes[i].velocity[1], tree->nodes[i].velocity[2], tree->nodes[i].p);
    printf("neighbors: ");
    for(j = 0; j < tree->nodes[i].neighbors_size; j++) {
	printf("%d ", tree->nodes[i].neighbors[j]);
    } 
    printf("\n");
  }
}

void kd_free(Kdtree *tree) {
  if (tree->nodes != NULL) {
    //free(tree->nodes);
  }
}

int kd_read(char *filename, Kdtree *data) {
  int i, rc, size;
  float x, y, z, v1, v2, v3, p;

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
  data->nodes = (Node*)calloc(size + 1, sizeof(Node));
  data->size = size;

  if (data->nodes == NULL) {
    fclose(stream);
    return -3;
  }
  
   // Read points from file
  for(i = 0; i < size; i++) {
    rc = fscanf(stream, "%f %f %f %f %f %f %f", &x, &y, &z, &v1, &v2, &v3, &p);
    if (rc != 7) {
      fclose(stream);
      free(data->nodes);
      return -2;
    }
    data->nodes[i].coords[0] = x; data->nodes[i].coords[1] = y; data->nodes[i].coords[2] = z;
    data->nodes[i].velocity[0] = v1; data->nodes[i].velocity[1] = v2; data->nodes[i].velocity[1] = v3;  
    data->nodes[i].p = p;
  }

  fclose(stream);

  return size;
}
