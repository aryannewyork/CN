#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5566
#define SIZE 1024

int main() {
    // Client is creating the 'server' socket instance to communicate with it.
    // The client must know the information 
    char *ip = "127.0.0.1";
    int port = PORT;

    int sock;
    struct sockaddr_in addr;
    socklen_t addr_size;
    char buffer[1024];
    int n;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) {
        perror("[-] Cannot create socket.\n");
        exit(1);
    } else {
        printf("[+] TCP server socket created successfully.\n");
    }

    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);

    if(connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("[-] Cannot connect to server.\n");
        exit(1);
    } else {
        printf("[+] Connected to the server.\n");
    }

    memset(buffer, '\0', sizeof(buffer));
    // gets(buffer);
	fgets(buffer, sizeof(buffer), stdin);
    printf("Client: %s\n", buffer);
    send(sock, buffer, sizeof(buffer), 0);

    memset(buffer, '\0', sizeof(buffer));
    recv(sock, buffer, sizeof(buffer), 0);
    printf("Server: %s\n", buffer);
    close(sock);
    printf("Disconnected from the server.\n");

    return 0;
}