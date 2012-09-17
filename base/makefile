kdtree: clean kdtree.c kdtree.h
	@gcc -o kdtree kdtree.c -lm

test: clean kdtree.c kdtree.h queue.c queue.h stack.c stack.h tests/test.c
	@gcc -o test tests/test.c kdtree.c kdtree.h queue.c queue.h stack.c stack.h -lm -lcunit -I. -g -Wall
	@./test

example: clean kdtree.c kdtree.h queue.c queue.h
	@gcc -o example examples/main.c kdtree.c kdtree.h queue.c queue.h stack.c stack.h -lm -I. -g -Wall
	@./example

val: 
	@valgrind -v --leak-check=yes ./example
clean: 
	@rm -f kdtree test a.out example *.gch tests/data/data_size_*.txt

