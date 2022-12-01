// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/wait.h>
#define PORT 2003
#define MAX_LENGTH 100

int main(int argc, char const *argv[])
{
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	
	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	
	// Forcefully attaching socket to the port 8080
#ifdef __APPLE__
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt)))
#else
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt)))
#endif
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );
	
	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr *)&address,
			sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	while(1) {
		if (listen(server_fd, 3) < 0)
		{
			perror("listen");
			exit(EXIT_FAILURE);
		}
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
						(socklen_t*)&addrlen))<0)
		{
			perror("accept");
			exit(EXIT_FAILURE);
		}

		// create proccess that will handle client interaction
		int pid = fork();
		if(pid == 0) {
			while(1) {
				char buffer[MAX_LENGTH] = {0};
				valread = read(new_socket , buffer, 1024);

				if(strcmp(buffer, "quit") == 0 || valread == 0) {
					printf("socket %d closed", new_socket);
					exit(0);
				}

				printf("recieved command: %s\n", buffer);
				
				// create proccess that will exit once command is executed
				pid = fork();
				if(pid == 0) {
					char* argument_list[MAX_LENGTH / 2];
					argument_list[0] = buffer;
					int i = 0;
					int argumentCount = 1;
					int openQuotes = 0;
					while(buffer[i] != '\n' && i < MAX_LENGTH - 1) {
						if(buffer[i] == ' ' && openQuotes == 0) {
							buffer[i] = '\0';
							argument_list[argumentCount] = &buffer[i + 1];
						} else if(buffer[i] == '"') {
							openQuotes = openQuotes == 1 ? 0 : 1;
						}

						i++;
					}
					buffer[i] = '\0';

					// redirect stdout and stderr to socket
					dup2(new_socket, STDOUT_FILENO);
					dup2(new_socket, STDERR_FILENO);
					close(new_socket);
					
					execvp(argument_list[0], argument_list);

					perror("Couldn't exec");
					exit(1);
				}
				waitpid(pid, NULL, 0);
			}

			return 0;
		}
	}

	return 0;		
}