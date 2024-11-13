run:
	gcc -c net_tcp_client.c
	gcc -o nettcpclient net_tcp_client.o
	gcc -c net_tcp_server.c
	gcc -o nettcpserver net_tcp_server.o

	gcc -c local_tcp_client.c
	gcc -o localtcpclient local_tcp_client.o
	gcc -c local_tcp_server.c
	gcc -o localtcpserver local_tcp_server.o

	gcc -c net_udp_client.c
	gcc -o netudpclient net_udp_client.o
	gcc -c net_udp_server.c
	gcc -o netudpserver net_udp_server.o

	gcc -c local_udp_client.c
	gcc -o localudpclient local_udp_client.o
	gcc -c local_udp_server.c
	gcc -o localudpserver local_udp_server.o

clean:
	rm *.o
	rm nettcpclient
	rm nettcpserver
	rm netudpclient
	rm netudpserver
	rm localtcpclient
	rm localtcpserver
	rm localudpclient
	rm localudpserver