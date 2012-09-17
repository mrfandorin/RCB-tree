#ifndef _STACK_H_
#define _STACK_H_

typedef struct Stack {
  int *s;
  int last;                      
  int count;                     
  int ssize;

} Stack;

void init_stack(Stack *q, int n);

void push_stack(Stack *q, int x);

int pop_stack(Stack *q);

int empty_stack(Stack *q);

void destroy_stack(Stack *q);

void print_stack(Stack *q);


#endif  /* _STACK_H_ */

