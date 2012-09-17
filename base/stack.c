#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stack.h"


void init_stack(Stack *s, int n) {
  
  s->ssize = n;
  s->last = -1;
  s->count = 0;
  s->s = (int*)malloc(n * sizeof(int));
  
}

void push_stack(Stack *s, int x) {
  if (s->count >= s->ssize) {
    printf("Stack overflow push\n");
  } else {
    s->last = s->last + 1;
		s->s[s->last] = x;
    s->count = s->count + 1;
  }
}

int pop_stack(Stack *s) {
  int x = 0;

  if (s->count <= 0) {
    printf("Empty stack pop.\n");
  } else {
    x = s->s[s->last];
    s->last = s->last - 1 ;
    s->count = s->count - 1;
  }

  return x;
}


int empty_stack(Stack *s) {
  if (s->count <= 0) {
    return 1;
  } else {
    return 0;
  }
}

void destroy_stack(Stack *s) {
  free(s->s);
}

void print_stack(Stack *s) {
  int i;

  if (empty_stack(s)) return;
 
	for(i = 0; i < s->count; i++) { 
  	printf("%d ", s->s[i]);
	}
	
  printf("\n");
}
