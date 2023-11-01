#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("NOT ENOUGH PARAMS");
        return 1;
    }

    //server IP@ and PORT
    const char *server_ip = argv[1];
    int server_port = atoi(argv[2]);

    //client socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        printf("ERROR WHILE CREATING SOCKET");
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

    //connect to server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("CANNOT CONNECT TO SERVER");
        return 1;
    }

    char message[1024];
    printf("CONNECTED TO SERVER\nENTER YOUR MESSAGES : \n");

    while (1) {
        memset(message, 0, sizeof(message));
        if (fgets(message, sizeof(message), stdin) == NULL) {
            printf("DESCONNECTED.\n");
            break; // BREAK IF CTRL+D
        }

        //SEND
        send(client_socket, message, strlen(message), 0);

        if (strcmp(message, "quit\n") == 0) {
            printf("DESCONNECTED.\n");
            break;
        }
    }

    close(client_socket);

    return 0;
}
