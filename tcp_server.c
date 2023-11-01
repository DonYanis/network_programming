#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("NO PORT ENTERED");
        return 1;
    }

    const int PORT = atoi(argv[1]);

    //tcp socket : type == sock_stream
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        printf("ERROR WHILE CREATING SOCKET");
        return 1;
    }
    //server address
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("ERROR WHIILE BINDING");
        return 1;
    }

    //listen to upcimming connection
    // max cnx demandes that the server can handle simultanously == 5
    if (listen(server_socket, 5) < 0) {
        printf("ERROR WHILE LISTENING");
        return 1;
    }

    //starting the server
    while (1) {
        printf("WAITING FOR A CLIENT...\n");

        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        //accept communication with new client 
        int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket < 0) {
            printf("ERROR WHILE ACCEPTING CLIENT");
            return 1;
        }

        printf("CONNECTION SECCUESFUL\n");

        char message[1024];
        while (1) {
            memset(message, 0, sizeof(message));

            int bytes_received = recv(client_socket, message, sizeof(message), 0);
            if (bytes_received < 0) {
                printf("ERROR WHILE RECEIVING MSG");
                break;
            }

            if (bytes_received == 0) {
                //if ctrl+d
                printf("CLIENT DISCONNECTED\n");
                break;
            }

            printf("Message : %s", message);

            if (strcmp(message, "quit\n") == 0) {
                printf("CLIENT HAS LEFT\n");
                break;
            }
        }

        close(client_socket);
    }

    close(server_socket);
    return 0;
}
