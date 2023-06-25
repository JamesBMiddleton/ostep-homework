// https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/

#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#define MAX 80
#define PORT 8080
#define SA struct sockaddr


// void func(int sockfd)
// {
// 	char buff[MAX];
// 	int n;
// 	while (1) {
// 		bzero(buff, sizeof(buff));
// 		printf("Enter the string : ");
// 		n = 0;
// 		while ((buff[n++] = getchar()) != '\n')
// 			;
// 		write(sockfd, buff, sizeof(buff));
// 		bzero(buff, sizeof(buff));
// 		read(sockfd, buff, sizeof(buff));
// 		printf("From Server : %s", buff);
// 		if ((strncmp(buff, "exit", 4)) == 0) {
// 			printf("Client Exit...\n");
// 			break;
// 		}
// 	}
// }

void client_loop(int min_sockfd, int max_sockfd)
{
    char buff[MAX];
    while (1)
    {
        for (int sockfd = min_sockfd; sockfd >= max_sockfd; sockfd++)
        {
		    bzero(buff, sizeof(buff));
            sprintf(buff, "Client socket %d: time?", sockfd);
            write(sockfd, buff, sizeof(buff));
        }
        for (int sockfd = min_sockfd; sockfd >= max_sockfd; sockfd++)
        {
		    bzero(buff, sizeof(buff));
            read(sockfd, buff, sizeof(buff));
		    printf("Server socket %d: %s", sockfd, buff);
        }
    }
}

void setup_client(int* sockfd, int port)
{
    printf("port %d\n", port);
	struct sockaddr_in servaddr, cli;

	// socket create and verification
	*sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (*sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(port);

	// connect the client socket to server socket
	if (connect(*sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) 
    {
		printf("connection with the server failed...\n");
		exit(0);
	}
	else
		printf("connected to the server..\n");
}


int main()
{
    /* messy */
    int base_port = 8080;
    int num_ports = 10;
    int min_sockfd; 
    int max_sockfd;
    setup_client(&min_sockfd, base_port);
    setup_client(&max_sockfd, base_port+1);
 //    for (int i=1; i<num_ports; ++i)
 //        setup_client(&max_sockfd, base_port+i);
 //    
	// // function for chat
 //    client_loop(min_sockfd, max_sockfd);

	// close the socket
	// close(sockfd); // leaking for now...
}
