#include <arpa/inet.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX 8192
#define PORT 1235
#define SA struct sockaddr

void func(int sockfd)
{
	char buff[MAX];
	int n;

	for (;;)
	{
		bzero(buff, sizeof(buff));

		printf("Enter the string: ");

		n = 0;

		while ((buff[n++] = getchar()) != '\n')
			;

		write(sockfd, buff, sizeof(buff));

		bzero(buff, sizeof(buff));

		read(sockfd, buff, sizeof(buff));
		printf("From Server: %s\n", buff);

		if (strncmp("exit", buff, 4) == 0)
		{
			printf("Client exit...\n");
			break;
		}

	}
}

int main()
{
	int sockfd, connfd;

	struct sockaddr_in servaddr, cli;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd == -1)
	{
		printf("Socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket created successfully...\n");

	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0)
	{
		printf("Conntection with the server failed...\n");
		exit(0);
	}
	else
		printf("Connected to the server...\n");

	func(sockfd);

	close(sockfd);
}
