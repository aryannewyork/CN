#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5555
#define SIZE 1024

int main() {
    // Client is creating the 'server' socket instance to communicate with it.
    // The client must know the information 
    char *ip = "127.0.0.1";
    int port = PORT;

    int sock;
    struct sockaddr_in addr;
    socklen_t addr_size = sizeof(addr);
    char buffer[SIZE], server_message[SIZE];
    int n;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0) {
        perror("[-] Cannot create socket.\n");
        exit(1);
    } else {
        printf("[+] UDP server socket created successfully.\n");
    }

    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);
    memset(&buffer, '\0', sizeof(buffer));
    printf("Enter Message: ");

	fgets(buffer, sizeof(buffer), stdin);
    printf("buffer %s\n", buffer);
    if(sendto(sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&addr, addr_size) < 0) {
        perror("[-] Unable to send message.\n");
        exit(1);
    }
    printf("[+] Sent Successfully.\n");
    if(recvfrom(sock, server_message, sizeof(server_message), 0, (struct sockaddr*)&addr, &addr_size) < 0) {
        perror("[-] Error while receiving server's message.\n");
        exit(1);
    }
    printf("Server's Message: %s\n", server_message);
    close(sock);
    return 0;
}