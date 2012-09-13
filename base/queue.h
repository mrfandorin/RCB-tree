#ifndef _QUEUE_H_
#define _QUEUE_H_

typedef struct Queue {
  int **q;
  int first;                      
  int last;                      
  int count;                     
  int qsize;
} Queue;

#endif  /* _QUEUE_H_ */

