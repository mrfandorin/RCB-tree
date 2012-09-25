#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "queue.h"

#define SIZE_Q 3

void init_queue(Queue *q, int n) {
  int i;

  q->first = 0;
  q->qsize = n;
  q->last = n - 1;
  q->count = 0;

  q->q = (Interval*)malloc(n * sizeof(Interval));
}

void enqueue(Queue *q, Interval *x) {
  if (q->count >= q->qsize) {
    printf("Queue overflow enqueue\n");
  } else {
    q->last = (q->last + 1) % q->qsize;
    q->q[q->last] = *x;
//    memcpy(&(q->q[q->last]), x, sizeof(Interval));
    q->count = q->count + 1;
  }
}


Interval* dequeue(Queue *q) {
  Interval *x;
  
  if (q->count <= 0) {
    printf("Empty queue dequeue.\n");
  } else {
    x = &(q->q[q->first]);
    q->first = (q->first + 1) % q->qsize;
    q->count = q->count - 1;
  }

  return x;
}

int empty(Queue *q) {
  if (q->count <= 0) {
    return 1;
  }
  return 0;
}

/*void destroy_queue(Queue *q) {
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
    //printf("(%d, %d, %d, %d) ", q->q[i][0], q->q[i][1], q->q[i][2], q->q[i][3]);
    //i = (i + 1) % q->qsize;
  }

  //printf("(%d, %d, %d, %d) ", q->q[i][0], q->q[i][1], q->q[i][2], q->q[i][3]);
  printf("\n");
}
*/
