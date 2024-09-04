run:
	gcc -c lib_add.c -o lib_add.o
	gcc -c lib_sub.c -o lib_sub.o
	gcc -c lib_mult.c -o lib_mult.o
	gcc -c lib_div.c -o lib_div.o
	ar rcs libmath.a lib_add.o lib_sub.o lib_mult.o lib_div.o
	gcc -c HW7.c -o HW7.o
	gcc -o run HW7.o -L. -lmath

clean:
	rm run
	rm libmath.a
	rm *.o
	