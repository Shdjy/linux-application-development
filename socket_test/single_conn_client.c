#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

//192.168.93.129
#define INADDR_LOCAL "192.168.93.129"

#define handle_error(cmd, result) \
	if(result < 0) \
	{ \
		perror(cmd); \
		return -1; \
	}

void *read_thread(void *argv)
{
	int sockfd = *(int*)argv;
	char *read_buf = NULL;
	ssize_t count = 0;

	read_buf = malloc(sizeof(char) * 1024);
	if(!read_buf)
	{
		perror("malloc client read_buf");
		return NULL;
	}

	while(count = recv(sockfd, read_buf, 1024, 0))
	{
		if(count < 0)
		{
			perror("recv");
		}
		fputs(read_buf, stdout);
	}

	printf("收到终止信号！\n");
	free(read_buf);

	return NULL;
}

void *write_thread(void *argv)
{
	int sockfd = *(int*)argv;
	char *write_buf = NULL;
	ssize_t count = 0;

	write_buf = malloc(sizeof(char) * 1024);
	if(!write_buf)
	{
		shutdown(sockfd, SHUT_WR);
		perror("malloc client write_buf");
		return NULL;
	}

	while(fgets(write_buf, 1024, stdin) != NULL)
	{
		count = send(sockfd, write_buf, 1024, 0);
		if(count < 0)
		{
			perror("send");
		}
	}
	printf("关闭连接！\n");
	shutdown(sockfd, SHUT_WR);
	free(write_buf);

	return NULL;
}
int main(int argc, char *argv[])
{
	int sockfd, temp_result;
	pthread_t pid_read, pid_write;
	struct sockaddr_in server_addr, client_addr;

	memset(&server_addr, 0, sizeof(server_addr));
	memset(&client_addr, 0, sizeof(client_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	server_addr.sin_port = htons(6666);

	client_addr.sin_family = AF_INET;
	inet_pton(AF_INET, INADDR_LOCAL, &client_addr.sin_addr);
	client_addr.sin_port = htons(8888);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	handle_error("socket", sockfd);

	temp_result = bind(sockfd, (struct sockaddr *)&client_addr, sizeof(client_addr));
	handle_error("bind", temp_result);

	temp_result = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	handle_error("connect", temp_result);
	
	pthread_create(&pid_read, NULL, read_thread, (void *)&sockfd);
	pthread_create(&pid_write, NULL, write_thread, (void*)&sockfd);

	pthread_join(pid_read, NULL);
	pthread_join(pid_write, NULL);

	printf("关闭资源！\n");
	close(sockfd);


	return 0;
}


