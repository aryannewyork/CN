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
    int listenfd, connectfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    // This buffer will be used to exchange information between client and server.
    char buffer[SIZE];
    int n;

    // Creating an actual socket, of STREAM type (for TCP).
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd < 0) {
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
    n = bind(listenfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(n < 0) {
        perror("[-] Error in binding.\n");
        exit(1);
    } else {
        printf("[+] Socket binded successfully,\n");
    }

    // Put socket in listen state, so it can take in connections from clients.
    listen(listenfd, 2);
    printf("Listening...\n");

    while(1) {
        addr_size = sizeof(client_addr);
        // Accepts connection from client on listenfd (client must make a request to connect to listenfd w/ correct server sock information.)
        // server captures the information of client in client_addr, which will be sued to communicate back to client.
        connectfd = accept(listenfd, (struct sockaddr*)&client_addr, &addr_size);
        if(connectfd < 0) {
            perror("[-] Client cannot connect.\n");
            exit(1);
        } else {
            printf("[+] Client connected. %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
            // printf("Port number of client is %d\n", client_addr.sin_port);
            int rc = fork();
            if(-1 == rc) {
                perror("[-] Fork Failed.\n");
            } else {
                int z;
                if(0 == rc) {
                    close(listenfd);
                    // Clearing buffers to start communication (receive and send data from and to client).
                    memset(&buffer, '\0', sizeof(buffer));
                    while(z = (recv(connectfd, buffer, sizeof(buffer), 0)))
                    printf("Client: %s\n", buffer);
                    memset(&buffer, '\0', sizeof(buffer));
                    fgets(buffer, sizeof(buffer), stdin);
                    printf("Server: %s\n", buffer);
                    send(connectfd, buffer, sizeof(buffer), 0);
                    // close(connectfd);
                    // printf("[+] Client disconnected.\n\n");
                }
                if(z < 0) {
                    perror("[-] Read Error.\n");
                }
            }
        }
    }
    close(connectfd);
    return 0;
}