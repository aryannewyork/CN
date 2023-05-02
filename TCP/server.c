#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5566
#define SIZE 1024

int main() {
    // This is where socket will be binded, ip and port.
    char *ip = "127.0.0.1";
    int port = PORT;

    // Creating structures to contain client and server socket information
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    // This buffer will be used to exchange information between client and server.
    char buffer[SIZE];
    int n;

    // Creating an actual socket, of STREAM type (for TCP).
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(server_sock < 0) {
        perror("[-] Error in creating socket.\n");
        exit(1);
    } else {
        printf("[+] TCP Server socket created successfully,\n");
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

    // Put socket in listen state, so it can take in connections from clients.
    listen(server_sock, 5);
    printf("Listening...\n");

    while(1) {
        addr_size = sizeof(client_addr);
        // Accepts connection from client on server_sock (client must make a request to connect to server_sock w/ correct server sock information.)
        // server captures the information of client in client_addr, which will be sued to communicate back to client.
        client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
        if(client_sock < 0) {
            perror("[-] Client cannot connect.\n");
            exit(1);
        } else {
            printf("[+] Client connected.\n");
            // printf("Port number of client is %d\n", client_addr.sin_port);
        }
        
        // Clearing buffers to start communication (receive and send data from and to client).
        memset(&buffer, '\0', sizeof(buffer));
        recv(client_sock, buffer, sizeof(buffer), 0);
        printf("Client: %s\n", buffer);
        memset(&buffer, '\0', sizeof(buffer));
        // gets(buffer);
	    fgets(buffer, sizeof(buffer), stdin);
        printf("Server: %s\n", buffer);
        send(client_sock, buffer, sizeof(buffer), 0);
        close(client_sock);
        printf("[+] Client disconnected.\n\n");
        
    }
    return 0;
}

