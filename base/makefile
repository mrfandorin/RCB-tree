kdtree: clean kdtree.c kdtree.h
	@gcc -o kdtree kdtree.c -lm

test: clean kdtree.c kdtree.h queue.c queue.h tests/test.c
	@gcc -o test tests/test.c kdtree.c kdtree.h queue.c queue.h -lm -lcunit -I.
	@./test

example: clean kdtree.c kdtree.h queue.c queue.h
	@gcc -o example examples/main.c kdtree.c kdtree.h queue.c queue.h -lm -I.
	@./example
	
clean: 
	@rm -f kdtree test a.out example *.gch tests/data/data_size_*.txt

