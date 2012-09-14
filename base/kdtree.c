#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "kdtree.h"
#include "queue.h"


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

  current = median(left, right);
  ind[0] = left; ind[1] = right; ind[2] = left + current; ind[3] = -1; ind[4] = -1;

  enqueue(&q, ind);

  while(!empty(&q)) {
    res = (int*)dequeue(&q);
    left = res[0];
    right = res[1];
    current = median(left, right);
    if (left <= left + current - 1) {
      ind[0] = left; ind[1] = left + current - 1; 
      ind[2] = ind[0] + median(ind[0], ind[1]);
      ind[3] = i;
      ind[4] = left + current;
      enqueue(&q, ind);
    }
    if (left + current + 1 <= right) {
      ind[0] = left + current + 1; ind[1] = right;
      ind[2] = ind[0] + median(ind[0], ind[1]);
      ind[3] = i;
      ind[4] = left + current;
      enqueue(&q, ind);
    }
    i++;
  }

  return q.q;
}


int kd_build(Kdtree *data, Kdtree *kd_tree) {
  int size2, axis, i, level, c_level, left, right, current, c_index, parent, parent_value;
  int **ind;

  if (data->size == 0) {
    kd_tree->nodes = NULL;
    kd_tree->size = 0;
    return 0;
  }

  level = ceil(log(data->size + 1)/log(2));
  size2 = (int)pow(2, level) - 1;
  
  kd_tree->nodes = (Node*)calloc(size2, sizeof(Node));
  kd_tree->size = size2;


  if (NULL == kd_tree->nodes) {
      return -1;
  }

  ind = generate_interval(data->size);

  for(i = 0; i < data->size; i++) {
    c_level = ((int)floor(log(2 * (i + 1))/log(2)) - 1);
   
    axis = c_level % 3;
    if (i == 0) {
      c_index = 0;
    } else {
      c_index = pow(2, level) - 1 + (i - (pow(2, level) - 2)) - 1;
    }

    //printf("(%d, %d, %d) parent %d data->size %d\n", ind[i][0], ind[i][1], ind[i][2], ind[i][3], data->size);
    left = ind[i][0]; right = ind[i][1]; current = ind[i][2]; parent = ind[i][3]; parent_value = ind[i][4];

    if (c_level == level - 1 && ((data->size + 1) & data->size) != 0) {
      if (current <= parent_value) {
        c_index = 2 * parent + 1;
      } else {
        c_index = 2 * parent + 2;
      }
      //printf("c_index %d current %d parent_value %d\n", c_index, current, parent_value);
    } else {
      if (axis == 0) {
        qsort(&(data->nodes[left]), right - left + 1, sizeof(Node), x_cmp);
      } else if (axis == 1) {
        qsort(&(data->nodes[left]), right - left + 1, sizeof(Node), y_cmp);
      } else if (axis == 2) {
        qsort(&(data->nodes[left]), right - left + 1, sizeof(Node), z_cmp);
      }
    }
    memcpy(&(kd_tree->nodes[c_index]), &(data->nodes[current]), sizeof(Node));
    
    kd_tree->nodes[c_index].fill = true;
    //kd_print(kd_tree);
  }

  return 0;
}

int kd_build_recursive(Kdtree *data, Kdtree *kd_tree) {
  kd_tree->nodes = NULL;
  if (data->size == 0) {
    kd_tree->size = 0;
  }
  return kd_build_recursive_iter(data->nodes, kd_tree, data->size, 0);
}

int kd_build_recursive_iter(Node *tree, Kdtree *kd_tree, int size, int c_index) {
  // Sorting points by coordinate
  int m_index, m_size; // median index
  int axis, size2;


  //int c_index = 0; // current index
  if (size == 0) {
    return 0;
  }

  if (NULL == kd_tree->nodes) {
    // Full number points in kd tree, by power 2
    size2 = (int)pow(2, ceil(log(size + 1)/log(2))) - 1;
    kd_tree->size = size2;
    // Allocation memory block for kd tree
    kd_tree->nodes = (Node*)calloc(size2, sizeof(Node));

    if (NULL == kd_tree->nodes) {
      return 1;
    }
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
  if (size % 2 != 0) {
    m_index += 1;
  }
  m_index--;
  m_size = m_index;
  if (size % 2 == 0) {
    m_size += 1;
  }

        // Add a new point
  memcpy(&(kd_tree->nodes[c_index]), &(tree[m_index]), sizeof(Node));
  kd_tree->nodes[c_index].fill = true;

  //printf("Step: m_index=%d, m_size=%d, c_index=%d\n", m_index, m_size, c_index);
  // Next axis
  axis = (axis + 1) % 3;
  kd_build_recursive_iter(tree, kd_tree, m_index, 2 * (c_index + 1) - 1);
  kd_build_recursive_iter(tree + m_index + 1, kd_tree, m_size, 2 * (c_index + 1));

  return 0;

}


void kd_print(Kdtree *tree) {
  int i;
  printf("Print tree\n");
  for(i = 0; i < tree->size; i++) {
    printf("%d (%.2f, %.2f, %.2f) (%.2f, %.2f)\n", i + 1, tree->nodes[i].x, tree->nodes[i].y, 
           tree->nodes[i].z, tree->nodes[i].v, tree->nodes[i].p);
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
