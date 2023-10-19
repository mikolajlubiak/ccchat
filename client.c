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
#include <pthread.h>

#define MAX 8192
#define PORT 1235
#define SA struct sockaddr

void *read_msg(void *ptr_sockfd)
{
	int sockfd = *(int *)ptr_sockfd;
	char buff[MAX];

	for (;;)
	{
		read(sockfd, buff, sizeof(buff));
		if (buff[0] != '\0')
		{
			printf("%s\n", buff);

			if (strncmp("server: exit", buff, 12) == 0)
			{
				printf("Client exit...\n");
				break;
			}

			bzero(buff, sizeof(buff));
		}
	}
}

void *write_msg(void *ptr_sockfd)
{
	int sockfd = *(int *)ptr_sockfd;
	char buff[MAX];
	char nick[] = "client";
	char msg[sizeof(buff)+sizeof(nick)+2];
	int n;

	for (;;)
	{
		while ((buff[n++] = getchar()) != '\n');
		sprintf(msg, "%s: %s", nick, buff);
		write(sockfd, msg, sizeof(msg));

		n = 0;
		bzero(msg, sizeof(msg));
		bzero(buff, sizeof(buff));
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

	pthread_t read_id;
	pthread_create(&read_id, NULL, read_msg, (void *)&sockfd);
	pthread_join(read_id, NULL);

	pthread_t write_id;
	pthread_create(&write_id, NULL, write_msg, (void *)&sockfd);
	pthread_join(write_id, NULL);

	close(sockfd);
}
