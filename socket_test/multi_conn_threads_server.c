#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define handle_error(cmd, result) \
	if(result < 0) \
	{ \
		perror(cmd); \
		return -1; \
	}

void *read_thread(void* argv)
{
	int client_fd = *(int*)argv;
	char *read_buf = NULL;
	char *write_buf = NULL;
	ssize_t send_count = 0;
	ssize_t count = 0; 

	read_buf = malloc(sizeof(char) * 1024);
	if(!read_buf)
	{
		printf("读缓存创建失败，断开连接\n");
		shutdown(client_fd, SHUT_WR);
		close(client_fd);
		perror("malloc server read_buf");
		return NULL;
	}
	write_buf = malloc(sizeof(char) * 1024);
	if(!write_buf)
	{
		printf("写缓存创建失败，断开连接\n");
		free(read_buf);
		shutdown(client_fd, SHUT_WR);
		close(client_fd);
		perror("malloc server write_buf");
		return NULL;
	}

	while((count = recv(client_fd, read_buf, 1024, 0)))
	{
		if(count < 0)
		{
			perror("recv");
		}
		printf("reveive message from client_fd: %d: %s\n", client_fd, read_buf);
		strcpy(write_buf, "reveived~\n");
		send_count = send(client_fd, write_buf, 1024, 0);
		if(send_count < 0)
		{
			perror("send");
		}
	}

	printf("客户端:%d,请求关闭连接\n", client_fd);
	strcpy(write_buf, "receive your shutdown signal\n");

	send_count = send(client_fd, write_buf, 1024, 0);
	if(send_count < 0)
	{
		perror("send");
	}
	printf("释放客户端%d资源\n", client_fd);
	shutdown(client_fd, SHUT_WR);
	close(client_fd);
	free(write_buf);
	free(read_buf);

	return NULL;
}

int main(int argc, char const* argv[])
{
	int sockfd, temp_result, client_fd;
	pthread_t pid_read, pid_write;
	struct sockaddr_in server_addr, client_addr;

	memset(&server_addr, 0, sizeof(server_addr));
	memset(&client_addr, 0, sizeof(client_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(6666);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	handle_error("sockfd", sockfd);

	temp_result = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	handle_error("bind", temp_result);

	temp_result = listen(sockfd, 128);
	handle_error("listen", temp_result);

	socklen_t cliaddr_len = sizeof(client_addr);
	
	while(1)
	{
		client_fd = accept(sockfd, (struct sockaddr *)&client_addr, &cliaddr_len);
		handle_error("accept", client_fd);

		printf("客户端from %s, port %d 文件描述符%d 建立连接\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), client_fd);

		pthread_t pid_read_write;
		if(pthread_create(&pid_read_write, NULL,read_thread, (void*)&client_fd))
		{
			perror("pthread_create");
		}
		pthread_detach(pid_read_write);
		printf("创建子线程，并处理为detached状态\n");
	}


	printf("释放资源\n");
	close(sockfd);

	return 0;
}
