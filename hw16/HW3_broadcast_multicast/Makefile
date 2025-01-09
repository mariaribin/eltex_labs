run:
	gcc -c broadcast_server.c
	gcc -o bserver broadcast_server.o

	gcc -c broadcast_client.c
	gcc -o bclient broadcast_client.o

	gcc -c multicast_server.c
	gcc -o mserver multicast_server.o

	gcc -c multicast_client.c
	gcc -o mclient multicast_client.o

clean:
	rm *.o
	rm mclient
	rm mserver
	rm bclient
	rm bserver