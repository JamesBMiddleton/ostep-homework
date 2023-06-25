// https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/


/* This was becoming a monkey-typewriter deal, I'd probably get it working
 * eventually, but I don't have the fundamental networking theory to
 * really understand what I'm doing - not an efficient learning process. 
 * Leaving it for now... I understand the event-based concurrency concepts. */

#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // read(), write(), close()
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

// Function designed for chat between client and server.
void func(int connfd)
{
	char buff[MAX];
	int n;
	// infinite loop for chat
	while (1) {
		bzero(buff, MAX);

		// read the message from client and copy it in buffer
		read(connfd, buff, sizeof(buff));
		// print buffer which contains the client contents
		printf("From client: %s\t To client : ", buff);
		bzero(buff, MAX);
		n = 0;
		// copy server message in the buffer
		while ((buff[n++] = getchar()) != '\n') // ! buff overflow incoming
			;

		// and send that buffer to client
		write(connfd, buff, sizeof(buff));

		// if msg contains "Exit" then server exit and chat ended.
		if (strncmp("exit", buff, 4) == 0) {
			printf("Server Exit...\n");
			break;
		}
	}
}

void server_loop(int min_connfd, int max_connfd)
{
    char buff[MAX];
    while (1)
    {
        for (int connfd = min_connfd; connfd >= max_connfd; connfd++)
        {
		    bzero(buff, sizeof(buff));
            read(connfd, buff, sizeof(buff));
		    printf("Message from client socket %d: %s", connfd, buff);
        }
        for (int connfd = min_connfd; connfd >= max_connfd; connfd++)
        {
		    bzero(buff, sizeof(buff));
            sprintf(buff, "Message from server socker %d: the time", connfd);
            write(connfd, buff, sizeof(buff));
        }
    }
}


void setup_server(int* sockfd, int* connfd, int port)
{
    printf("port %d\n", port);
	int len;
	struct sockaddr_in servaddr, cli;

	// socket create and verification
	*sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (*sockfd == -1) 
    {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);

	// Binding newly created socket to given IP and verification
	if ((bind(*sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf("socket bind failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully binded..\n");

	// Now server is ready to listen and verification
	if ((listen(*sockfd, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}
	else
		printf("Server listening..\n");
	len = sizeof(cli);

	// Accept the data packet from client and verification
	*connfd = accept(*sockfd, (SA*)&cli, (socklen_t*)&len);
	if (connfd < 0) {
		printf("server accept failed...\n");
		exit(0);
	}
	else
		printf("server accept the client...\n");
}

// Driver function
int main()
{
    /* messy */
    int base_port = 8080;
    int num_ports = 10;
    int min_sockfd; 
    int max_sockfd;
    int min_connfd;
    int max_connfd;
    setup_server(&min_sockfd, &min_connfd, base_port);
    setup_server(&max_connfd, &min_connfd, base_port+1);
    // for (int i=1; i<num_ports; ++i)
    //     setup_server(&max_sockfd, &max_connfd, base_port+i);

    server_loop(min_connfd, max_connfd);

	// After chatting close the socket
	// close(sockfd);
}
