#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8000
#define BUFFER_SIZE 100
int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Usage: %s <port_number>\n", argv[0]);
        exit(1);
    }

    int server_soc;//to create socket
    int client_soc;//to accept connection

    int port_num = atoi(argv[1]);

    struct sockaddr_in server_address;//server receive on this address
    struct sockaddr_in client_address;//server sends to client on this address

    int msg_len;
    int client_addressLength;
    int pid;

    //create socket
    server_soc = socket(AF_INET, SOCK_STREAM, 0);
    //initialize the socket addresses
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port_num);

    //bind the socket with the server address and port
    bind(server_soc, (struct sockaddr *) &server_address, sizeof(server_address));

    //listen for connection from client
    listen(server_soc, 5);

    while(1)
    {
        //parent process waiting to accept a new connection
        printf("\n*****server waiting for new client connection:*****\n");
        int client_address_len = sizeof(client_address);
        client_soc = accept(server_soc, (struct sockaddr *) &client_address, &client_address_len);
        printf("connected to client: %s\n", inet_ntoa(client_address.sin_addr));

        //child process is created for serving each new clients
        pid = fork();
        if(pid == 0) {
            while (1) {
                char msg[BUFFER_SIZE] = "";
                msg_len = recv(client_soc, msg, BUFFER_SIZE, 0);
                if (msg_len == 0) {
                    close(client_soc);
                    break;
                }
                msg[msg_len] = '\0';
                send(client_soc, msg, strlen(msg), 0);
                printf("Receive and set:%s\n", msg);
            }
            exit(0);
        } else {
            close(client_soc);
        }
    }
    return 0;
}
