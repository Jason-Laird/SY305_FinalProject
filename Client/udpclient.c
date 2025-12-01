#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080


int main(void) {

    char *ip = "127.0.0.1";
    int port = PORT;

    int sockfd;
    struct sockaddr_in addr;
    char buffer[1024];
    socklen_t addr_size;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);

    char drop_payload[]    = "The dropped payload";
    char execute_payload[] = "The executed payload";

    while (1) {

        char choice;
        printf("Enter D or E for payload: ");
        scanf(" %c", &choice);

        if (choice != 'D' && choice != 'E') {
            printf("Invalid input. Use D or E.\n");
            continue;
        }

        if (choice == 'D') {
            strcpy(buffer, drop_payload);
        } else {
            strcpy(buffer, execute_payload);
        }

        // Send correct length
        sendto(sockfd, buffer, strlen(buffer) + 1, 0, 
        (struct sockaddr*)&addr, sizeof(addr));

        printf("[+] Sent: %s\n", buffer);

        // Receive
        memset(buffer, 0, sizeof(buffer));
        addr_size = sizeof(addr);

        recvfrom(sockfd, buffer, sizeof(buffer), 0,
        (struct sockaddr*)&addr, &addr_size);

        printf("[+] Received: %s\n", buffer);
    }

    return 0;
}
