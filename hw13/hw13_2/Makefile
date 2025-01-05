run:
	gcc -c server.c
	gcc -o server server.o -lrt
	gcc -c client.c
	gcc -o client client.o -lrt -lpthread
	gcc -c client_app.c
	gcc -o clientapp client_app.o -lrt -Wshadow -lpthread -lmenu -lpanel -lform -lncurses

clean:
	rm *.o
	rm client
	rm clientapp
	rm server