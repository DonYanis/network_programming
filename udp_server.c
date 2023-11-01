#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char **argv) {

    if (argc < 2){
        printf("NO PORT ENTERED");
        return 1;
    }

    //get the PORT
    const int PORT = atoi(argv[1]);

    //create a UDP scoket
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0) {
        printf("ERROR WHILE CREATING SOCKET");
        return 1;
    }

    //int an internet socket address
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;

    /*one : specify an address : */
    //inet_aton("127.0.0.0", &server_addr.sin_addr);

    /*two : listen to all addresses , equiv to 0.0.0.0*/
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); //htonl : convert to big indian

    server_addr.sin_port = htons(PORT); //htons : convert to big indian
    for (int i=0; i<8; i++) {
        server_addr.sin_zero[i] = 0;
    }
    //bind an address to the socket

    if (bind(sock, (struct sockaddr *) &server_addr, sizeof(struct sockaddr_in)) < 0) {
        printf("ERROR WHILE BINDING %d",PORT);
        return 1;
    }

    
    char buffer[1024];
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);

    while (1) {
        memset(buffer, 0, sizeof(buffer)); //all bytes to 0
        int recv_len = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &addr_len);
        if (recv_len < 0) {
            printf("ERROR IN DATA RECEPTION");
            break;
        }

        printf("Message : %s\n", buffer);
        if(strcmp(buffer, "quit\n") == 0){
            break;
        }
    }

    close(sock);  

    return 0;
}