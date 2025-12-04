#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 5000

int main(void) {

    char *ip = "10.60.117.216";
    int port = PORT;

    int sockfd;
    struct sockaddr_in addr;
    char buffer[2];
    socklen_t addr_size; 

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);


    while (1) {

        char choice;
        printf("Enter D or E for payload: ");
        scanf(" %c", buffer);        // Send correct length
        sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&addr, sizeof(addr));

        bzero(buffer,sizeof(buffer));
    }

    return 0;
}
