kdtree: clean kdtree.c kdtree.h
	@gcc -o kdtree kdtree.c -lm

test: clean kdtree.c kdtree.h tests/test.c
	@gcc -o test tests/test.c kdtree.c kdtree.h -lm -lcunit -I.
	@./test

clean: 
	@rm -f kdtree test *.gch

