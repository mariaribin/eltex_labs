run:
	gcc -c server.c
	gcc -o server server.o -lpthread -lrt
	gcc -c client.c
	gcc -o client client.o -lpthread -lrt

	gcc -c client_app.c
	gcc -o clientapp client_app.o -lrt -Wshadow -lpthread -lmenu -lpanel -lform -lncurses

clean:
	rm *.o
	rm client
	rm server
	rm clientapp
	sudo rm /dev/shm/server_send
	sudo rm /dev/shm/server_storage 
	sudo rm /dev/shm/server_recieve 
	
	sudo rm /dev/shm/storage_sem
	sudo rm /dev/shm/recieve_sem
	sudo rm /dev/shm/send_sem


