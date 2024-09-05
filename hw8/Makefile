run:
	gcc lib_add.c -fPIC -c
	gcc lib_sub.c -fPIC -c
	cc lib_mult.c -fPIC -c
	gcc lib_div.c -fPIC -c
	gcc -shared lib_add.o lib_sub.o lib_mult.o lib_div.o -o libmath.so
	gcc -c HW8.c -o HW8.o
	gcc -o run HW8.o -L. -lmath
	export LD_LIBRARY_PATH=$PWD

clean:
	rm run
	rm libmath.so
	rm *.o
	