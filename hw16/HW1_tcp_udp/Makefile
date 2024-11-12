run:
	gcc -c net_tcp_client.c
	gcc -o nettcpclient net_tcp_client.o
	gcc -c net_tcp_server.c
	gcc -o nettcpserver net_tcp_server.o

	gcc -c net_udp_client.c
	gcc -o netudpclient net_udp_client.o
	gcc -c net_udp_server.c
	gcc -o netudpserver net_udp_server.o

clean:
	rm *.o
	rm nettcpclient
	rm nettcpserver
	rm netudpclient
	rm netudpserver