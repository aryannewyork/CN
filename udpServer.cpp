#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <bits/stdc++.h>
using namespace std;

int main(void)
{
    int socket_desc;
    struct sockaddr_in server_addr, client_addr;
    char server_message[100], client_message[100];
    socklen_t client_struct_length = sizeof(client_addr);

    // Clean buffers:
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));

    // Create UDP socket:
    socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (socket_desc < 0)
    {
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket created successfully\n");

    // Set port and IP:
    server_addr.sin_family = AF_INET;
    // htons -- Host to network short
    // ntohs -- Network to host short
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind to the set port and IP:
    if (bind(socket_desc, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("Couldn't bind to the port\n");
        return -1;
    }
    printf("Done with binding\n");

    printf("Listening for incoming messages...\n\n");
    while (1)
    {
        // Receive client's message:
        if (recvfrom(socket_desc, client_message, sizeof(client_message), 0,
                     (struct sockaddr *)&client_addr, &client_struct_length) < 0)
        {
            printf("Couldn't receive\n");
            return -1;
        }
        printf("Received message from IP: %s and port: %i\n",
               // ntoa -- network to ASCII
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        printf("Msg from client: %s\n", client_message);

        printf("Enter message: ");
        ifstream file;
        file.open("dir.dat");
        int numRecords;
        file >> numRecords;
        cout << "Total number of records in the directory: " << numRecords << "\n";
        char res[26];
        for(int i = 0; i < numRecords; i++){
            string name, email;
            file >> name >> email;
            if(name == client_message) {
                i = 0;
                for(auto ch : email){
                    res[i] = ch;
                    i++;
                }
                break;
            }
        }
        // Respond to client:
        strcpy(server_message, res);

        if (sendto(socket_desc, server_message, strlen(server_message), 0,
                   (struct sockaddr *)&client_addr, client_struct_length) < 0)
        {
            printf("Can't send\n");
            return -1;
        }
        else
        {
            printf("Sent Successfully\n");
        }
    }

    // Close the socket:
    close(socket_desc);
}
