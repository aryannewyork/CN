// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 8090 


int main(int argc, char const *argv[]) 
{ 


	pid_t childpid;     
	int server_fd, new_socket, valread; 
	struct sockaddr_in address; 
	int opt = 1; 
	int addrlen = sizeof(address); 
	char buffer[1024] = {0}; 
	char *hello = "Hello from server"; 
	char string[35];
	
	// Creating socket file descriptor 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	// Forcefully attaching socket to the port 8080 
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 
	
	// Forcefully attaching socket to the port 8080 
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	if (listen(server_fd, 3) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
	while(1)
	{
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
					(socklen_t*)&addrlen))<0) 
	{ 
		perror("accept"); 
		exit(EXIT_FAILURE); 
	} 
	
	if((childpid = fork()) == 0){
			close(server_fd);
			
	
	printf("Socket Descriptor %d\n", new_socket); 
	while(1)
	{
	printf("%ld%ld \n", (long)getpid(), (long)getppid());
	valread = read( new_socket , buffer, 1024); 
	printf("%s\n",buffer ); 
	fgets(string, 25, stdin);
	send(new_socket , string , strlen(string) , 0 ); 
	printf("---------Sent (Server)----------\n"); 
	}
	
	return 0; 
	close(new_socket);
	}
	else{
	close(new_socket);
	printf("CHILD PROCESS CREATED PID is %d", childpid);
	}
	}
} 
