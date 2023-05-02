#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5555
#define SIZE 1024

int main() {
    // This is where socket will be binded, ip and port.
    char *ip = "127.0.0.1";
    int port = PORT;

    // Creating structures to contain client and server socket information
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    addr_size = sizeof(client_addr);
    // This buffer will be used to exchange information between client and server.
    char buffer[SIZE], client_message[SIZE];
    int n;

    // Creating an actual socket, of STREAM type (for TCP).
    server_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(server_sock < 0) {
        perror("[-] Error in creating socket.\n");
        exit(1);
    } else {
        printf("[+] UDP Server socket created successfully,\n");
    }

    // Preparing the server socket to be binded, by populating its information struct with
    // relevant information.
    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    // Binding the socket created in line 24 with socket description in server_addr struct
    n = bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(n < 0) {
        perror("[-] Error in binding.\n");
        exit(1);
    } else {
        printf("[+] Socket binded successfully,\n");
    }
    printf("Listening...\n");

    while(1) {
        memset(&buffer, '\0', sizeof(buffer));
        memset(&client_message, '\0', sizeof(client_message));
        if(recvfrom(server_sock, client_message, sizeof(client_message), 0, (struct sockaddr*)&client_addr, &addr_size) < 0) {
            perror("[-] Couldn't receive.\n");
            exit(1);
        }
        printf("Received Message from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        printf("Message from client: %s\n", client_message);
	    fgets(buffer, sizeof(buffer), stdin);
        if(sendto(server_sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, addr_size) < 0){
            perror("[-] Cannot Send.\n");
            exit(1);
        }
        printf("Sent Successfully.\n"); 
    }
    close(server_sock);
}

