run:
	gcc -c server.c
	gcc -o server server.o -lpthread -lrt
	gcc -c client.c
	gcc -o client client.o -lpthread -lrt

clean:
	rm *.o
	rm client
	rm server
	sudo rm /dev/shm/shmemory 
	sudo rm /dev/shm/semaphore