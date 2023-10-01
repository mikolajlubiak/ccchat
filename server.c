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

void func(int connfd)
{
	char buff[MAX];
	char nick[] = "server";
	int n;

	for (;;)
	{
		bzero(buff, sizeof(buff));

		read(connfd, buff, sizeof(buff));
		printf("%s\n", buff);

		bzero(buff, sizeof(buff));
		n = 0;

		while ((buff[n++] = getchar()) != '\n')
			;

		char msg[sizeof(buff)+sizeof(nick)+3];

		sprintf(msg, "%s: %s", nick, buff);

		write(connfd, msg, sizeof(msg));

		if (strncmp("exit", buff, 4) == 0)
		{
			printf("Server exit...\n");
			break;
		}
	}
}

int main()
{
	int sockfd, connfd, len;

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
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0)
	{
		printf("Socket bind failed...\n");
		exit(0);
	}
	else
		printf("Socket binded successfully...\n");

	if ((listen(sockfd, 5)) != 0)
	{
		printf("Listen failed...\n");
		exit(0);
	}
	else
		printf("Server listening...\n");

	len = sizeof(cli);

	connfd = accept(sockfd, (SA*)&cli, &len);
	if (connfd < 0)
	{
		printf("Server accept failed...\n");
		exit(0);
	}
	else
		printf("Server accept the client...\n");

	func(connfd);

	close(sockfd);
}
