#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <arpa/inet.h> 
#include <fcntl.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <time.h>

#define BUFSIZE 1460

int main(int argc, char **argv)
{
	int sock;
	int file;

	char buf[BUFSIZE];
	char client_message[] = "Hello Server!";

	clock_t start;
	clock_t end;
	double time_length;

	struct sockaddr_in server_addr;
	int len;

	int total_size;

	FILE *total = fopen("/mnt/c/Users/dlehd/Linux/send.mp4", "r");
	fseek(total, 0, SEEK_END);
	total_size = ftell(total);
	fclose(total);

	if (argc != 3) {
		printf("[Error] Please follow the form: %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	file = open("receive.mp4", O_WRONLY | O_CREAT | O_TRUNC);
	if (file == -1){
		printf("[Error] open()\n");
		exit(1);
	}


	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1){
		printf("[Error] socket()\n");
		exit(1);
	}

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*) &server_addr, sizeof(server_addr)) == -1){
		printf("[Error] connect()\n");
		exit(1);
	}

	write(sock, client_message, sizeof(client_message));
	
	double percent;
	FILE* current;
	current = fopen("/mnt/c/Users/dlehd/Linux/receive.mp4", "r");
	int current_size;

	start = clock();
	while ((len = read(sock, buf, BUFSIZE)) != 0)
	{
		write(file, buf, len);
		fseek(current, 0, SEEK_END);
		current_size = ftell(current);
		percent = (current_size * 100.0f) / total_size;	
		printf("Downloading...%.2f%%\r", percent);
	}
	end = clock();
	time_length = ((double)(end - start) / CLOCKS_PER_SEC);
	fclose(current);

	printf("Time spent downloading file : %.5f초\n", time_length);

	close(file);
	close(sock);
	return 0;
}