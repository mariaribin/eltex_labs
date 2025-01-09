run:
	gcc -c server.c
	gcc -o server server.o -lrt
	gcc -c client.c
	gcc -o client client.o -lrt

clean:
	rm *.o
	rm client
	rm server