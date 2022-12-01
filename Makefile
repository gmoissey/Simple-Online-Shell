all:
	$(CC) socket_server.c -g -o socket_server
	gcc socket_client.c -g -o socket_client
clean:
	rm socket_server socket_client