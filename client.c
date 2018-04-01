#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8000
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 4096
int main(int argc, char *argv[]) {

    if(argc != 3) {
        printf("Usage: %s <ip_address> <port_number>\n", argv[0]);
        exit(1);
    }

    int server_soc;
    struct sockaddr_in server_address;

    char *ip_addr = argv[1];
    unsigned int port_num = atoi(argv[2]);

    // Create socket
    server_soc = socket(AF_INET, SOCK_STREAM, 0);
    if(server_soc == -1) {
        printf("Could not create socket.\n");
        exit(1);
    }
    // Initialize the socket addresses
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(ip_addr);
    server_address.sin_port = htons(port_num);

    // Client  connect to server on port
    if (connect(server_soc, (struct sockaddr *) &server_address, sizeof(server_address)) != 0) {
        printf("Could not connect to %s on port number %d\n", ip_addr, port_num);
        exit(1);
    }

    // Send to sever and receive from server
    while (1) {
        char server_msg[BUFFER_SIZE] = "";
        char client_msg[BUFFER_SIZE] = "";
        printf("\nEnter message to send to server:\n");
        fgets(client_msg, BUFFER_SIZE, stdin);
        if(client_msg[0]=='#')
            break;
        //n = strlen(client_msg) + 1;
        send(server_soc, client_msg, strlen(client_msg), 0);
        recv(server_soc, server_msg, BUFFER_SIZE, 0);
        printf("Receive message from  server::%s\n", server_msg);
    }
    close(server_soc);
    return 0;
}
