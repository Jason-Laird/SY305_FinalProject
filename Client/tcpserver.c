#include <stdio.h>          // For printf, perror
#include <stdlib.h>         // For exit, EXIT_FAILURE
#include <string.h>         // For memset, strlen
#include <unistd.h>         // For read, write, close
#include <arpa/inet.h>      // For sockaddr_in, inet functions, htons

#define PORT 5001          // Port number the server will listen on
#define BUFFER_SIZE 512    // Size of buffer for incoming messages


int main() {
    int server_fd, client_fd;                  // File descriptors for server and client sockets
    struct sockaddr_in server_addr, client_addr;  // Structures to hold address information
    socklen_t client_len = sizeof(client_addr);   // Length of client address struct
    char buffer[BUFFER_SIZE];                  // Buffer to store data received from client

    pid_t cliFork;
    cliFork= fork();
    if (0==cliFork){
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
    }else{
        // ----------------------
        // Create a TCP socket
        // ----------------------
        server_fd = socket(AF_INET, SOCK_STREAM, 0);  // AF_INET = IPv4, SOCK_STREAM = TCP
        if (server_fd < 0) {
            perror("socket failed");                 // Print error message
            exit(EXIT_FAILURE);                      // Exit program
        }
    }
        // ----------------------
        // Configure server address
        // ----------------------
        memset(&server_addr, 0, sizeof(server_addr)); // Clear structure (optional but good practice)
        server_addr.sin_family = AF_INET;             // Set address family to IPv4
        server_addr.sin_addr.s_addr = INADDR_ANY;     // Bind to all available network interfaces
        server_addr.sin_port = htons(PORT);           // Convert port number to network byte order

        // ----------------------
        // Bind socket to the address and port
        // ----------------------
        if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            perror("bind failed");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        // ----------------------
        // Begin listening for incoming connections
        // ----------------------
        if (listen(server_fd, 5) < 0) {               // '5' = max number of queued connections
            perror("listen failed");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        printf("Server listening on port %d...\n", PORT);

        // ----------------------
        // Accept a client connection
        // ----------------------
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            perror("accept failed");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        printf("Client connected.\n");

        // ----------------------
        // Read data from the client
        // ----------------------
        int bytes = read(client_fd, buffer, BUFFER_SIZE - 1);  // Read up to 1023 bytes
        if (bytes < 0) {
        perror("read failed");
        } else{
            (client_fd);   // Close connection with client
            close(server_fd);   // Close the listening socket
            buffer[bytes] = '\0';              // Null-terminate received data
            printf("Received: %s\n", buffer);  // Print the message sent by the client

        }

        // ----------------------
        // Close sockets
        // ----------------------
        close(client_fd);   // Close connection with client
        close(server_fd);   // Close the listening socket

        
        return 0; 
    }        // Exit program successfully
