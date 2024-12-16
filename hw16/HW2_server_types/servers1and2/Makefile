run:
	gcc -c server1.c
	gcc -o server1 server1.o

	gcc -c client1.c
	gcc -o client1 client1.o

	gcc -c client2.c
	gcc -o client2 client2.o

	gcc -c client1_1.c
	gcc -o client11 client1_1.o

	gcc -c server2.c
	gcc -o server2 -g server2.o -lpthread


clean:
	rm *.o
	rm server1
	rm server2
	rm client1
	rm client11
	rm client2