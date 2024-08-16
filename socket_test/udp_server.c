#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define handle_error(cmd, result) \
	if(result < 0) \
	{ \
		perror(cmd); \
		return -1; \
	}

int main(int argc, char const *argv[])
{
	int sockfd, temp_result, clientfd;
	struct sockaddr_in server_addr, client_addr;
	char *buf = malloc(1024);

	memset(&server_addr, 0, sizeof(server_addr));
	memset(&client_addr, 0, sizeof(client_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(6666);

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	handle_error("socket", sockfd);

	temp_result = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	handle_error("bind", temp_result);

	do
	{
		memset(buf, 0, 1024);
		socklen_t clientaddr_len = sizeof(clientfd);

		temp_result = recvfrom(sockfd, buf, 1024, 0, (struct sockaddr *)&client_addr, &clientaddr_len);
		handle_error("recvfrom", temp_result);

		if(strncmp(buf, "EOF", 3) != 0)
		{
			printf("received msg from %s at port %d:%s", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buf);
			strcpy(buf, "ok\n");
		}
		else
		{
			printf("received EOF from client, existing...\n");
		}

		temp_result = sendto(sockfd, buf, 4, 0, (struct sockaddr *)&client_addr, clientaddr_len);
	}while(strncmp(buf, "EOF", 3) != 0);

	free(buf);

	return 0;
}
