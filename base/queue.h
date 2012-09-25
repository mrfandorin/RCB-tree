#ifndef _QUEUE_H_
#define _QUEUE_H_

typedef struct Interval {
  int left;
  int right;
  int parent;

} Interval;

typedef struct Queue {
  Interval *q;
  int first;
  int last;
  int count;
  int qsize;
} Queue;


void init_queue(Queue *q, int n);

void enqueue(Queue *q, Interval *x);

Interval* dequeue(Queue *q);

int empty(Queue *q);

void destroy_queue(Queue *q);

void print_queue(Queue *q);



#endif  /* _QUEUE_H_ */

