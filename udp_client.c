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

    const char *server_address = argv[1];
    const int PORT = atoi(argv[2]);

    //create client socket
    int client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket < 0) {
        perror("RROR WHILE CREATING SOCKET");
        return 1;
    }

    //socket addr : Port and server addre
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    //convert decimal pointed string IP to binary
    inet_aton(server_address, &server_addr.sin_addr);

    char message[1024];
    printf("MESSAGE : \n");
    while (fgets(message, sizeof(message), stdin) != NULL) {
        sendto(client_socket, message, strlen(message), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
        printf("MESSAGE SENT: %s", message);
        printf("Again ? ('quit' to shut server) : ");
    }

    close(client_socket);

    return 0;
}
