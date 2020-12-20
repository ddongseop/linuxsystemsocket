#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <arpa/inet.h> 
#include <fcntl.h> 
#include <sys/types.h> 
#include <sys/socket.h> 

#define BUFSIZE 1460

int main(int argc, char **argv)
{
	int server_socket;
	int client_socket;
	int file;
	
	char buf[BUFSIZE];
	char client_message[30];

	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	int client_addr_size;
	int len;

	if (argc != 2) {
		printf("[Error] Please follow the form: %s <port>\n", argv[0]);
		exit(1);
	}

	server_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (server_socket == -1){
		printf("[Error] socket()\n");
		exit(1);
	}

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(atoi(argv[1]));

	if (bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr)) == -1){
		printf("[Error] bind()\n");
		exit(1);
	}

	if (listen(server_socket, 5) == -1){
		printf("[Error] listen()\n");
		exit(1);
	}

	client_addr_size = sizeof(client_addr);
	client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_size);
	if (client_socket == -1){
		printf("[Error] accept()\n");
		exit(1);
	}

	int str_len = read(client_socket, client_message, sizeof(client_message) - 1);

	if (str_len == -1){
		printf("[Error] read()\n");
		exit(1);
	}

	printf("Message from Client: %s\n", client_message);

	file = open("/mnt/c/Users/dlehd/Linux/send.mp4", O_RDONLY);
	if (file == -1){
		printf("[Error] open() (check yourfile)\n");
		exit(1);
	}

	while ((len = read(file, buf, BUFSIZE)) != 0)
	{
		write(client_socket, buf, len);
	}

	if (shutdown(client_socket, SHUT_WR) == -1){
		printf("[Error] shutdown()\n");
		exit(1);
	}

	close(file);
	close(client_socket);
	close(server_socket);

	return 0;
}


