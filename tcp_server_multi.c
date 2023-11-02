#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>

/*
chat server with tcp
*/

#define MAX_CLIENTS 3

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("NO PORT ENTERED");;
        return 1;
    }

    const int PORT = atoi(argv[1]);

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        printf("ERROR WHILE CREATING SOCKET");
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("ERROR WHIILE BINDING");
        return 1;
    }

    if (listen(server_socket, MAX_CLIENTS) < 0) {
        printf("ERROR WHILE LISTENING");
        return 1;
    }

    int client_sockets[MAX_CLIENTS] = {0};
    int num_clients = 0;

    while (num_clients < MAX_CLIENTS) {
        printf("waiting for client %d...\n", num_clients + 1);
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);

        if (client_socket < 0) {
            printf("ERROR WHILE ACCEPTING CLIENT");
            continue;
        }

        printf("Client %d connected.\n", num_clients + 1);

        client_sockets[num_clients] = client_socket;
        num_clients++;

        if (num_clients == MAX_CLIENTS) {
            printf("Everyone is here chat begins :\n");
        }
    }

    // Make sockets non-read blocking with the function fcntl
    for (int i = 0; i < MAX_CLIENTS; i++) {
        fcntl(client_sockets[i], F_SETFL, O_NONBLOCK);
    }

    while (1) {
        for (int i = 0; i < MAX_CLIENTS; i++) {
            char message[1024];
            memset(message, 0, sizeof(message));

            int bytes_received = recv(client_sockets[i], message, sizeof(message), 0);
            if (bytes_received > 0) {
                printf("Client %d : %s", i + 1, message);

                // send message to other clients
                char prefix[1024];
                for (int j = 0; j < MAX_CLIENTS; j++) {
                    if (j != i) {
                        snprintf(prefix, sizeof(prefix), "%d : %s", i, message);
                        send(client_sockets[j], prefix, strlen(prefix), 0);
                    }
                }
            } else if (bytes_received == 0) {
                printf("Client %d disconnected.\n", i + 1);
                close(client_sockets[i]);
                client_sockets[i] = 0;
                num_clients--;

                if (num_clients == 0) {
                    printf("All clients left, server is closed.\n");
                    close(server_socket);
                    return 0;
                }
            }
        }
    }

    return 0;
}
