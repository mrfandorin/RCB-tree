#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "queue.h"


void init_queue(Queue *q, int n) {
  int i;

  q->first = 0;
  q->qsize = n;
  q->last = n - 1;
  q->count = 0;
  q->q = (int**)malloc(n * sizeof(int*));
  for(i = 0; i < n; i++) {
    q->q[i] = (int*)malloc(5 * sizeof(int));
  }  

}

void enqueue(Queue *q, int *x) {
  if (q->count >= q->qsize) {
    printf("Queue overflow enqueue\n");
  } else {
    q->last = (q->last + 1) % q->qsize;
//    memcpy(&q->q[q->last], &x, 2 * sizeof(int));
    q->q[q->last][0] = x[0];
    q->q[q->last][1] = x[1];
    q->q[q->last][2] = x[2];
    q->q[q->last][3] = x[3];
    q->q[q->last][4] = x[4];
    q->count = q->count + 1;
  }
}

int* dequeue(Queue *q) {
  int *x;

  if (q->count <= 0) {
    printf("Empty queue dequeue.\n");
  } else {
    x = q->q[q->first];
    q->first = (q->first + 1) % q->qsize;
    q->count = q->count - 1;
  }

  return x;
}


int empty(Queue *q) {
  if (q->count <= 0) {
    return 1;
  } else {
    return 0;
  }
}

void destroy_queue(Queue *q) {
  int i;
  for(i = 0; i < q->qsize; i++) {
    free(q->q[i]);
  }
  free(q->q);
}

void print_queue(Queue *q) {
  int i;

  if (empty(q)) return;

  i = q->first; 
        
  while (i != q->last) {
    printf("(%d, %d, %d, %d) ", q->q[i][0], q->q[i][1], q->q[i][2], q->q[i][3]);
    i = (i + 1) % q->qsize;
  }

  printf("(%d, %d, %d, %d) ", q->q[i][0], q->q[i][1], q->q[i][2], q->q[i][3]);
  printf("\n");
}
