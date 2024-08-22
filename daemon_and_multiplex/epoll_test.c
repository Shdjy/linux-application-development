#include <asm-generic/errno-base.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>

#define SERVER_PROT 6666
#define BUFFER_SIZE 1024
#define MAX_EVENTS 10

#define handle_error(cmd, result) \
	if(result < 0) \
	{ \
		perror(cmd); \
		return -1; \
	}

char *read_buf = NULL;
char *write_buf = NULL;

void init_buf()
{
	read_buf = malloc(sizeof(char) * BUFFER_SIZE);
	if(!read_buf)
	{
		printf("服务端创建读缓冲异常\n");
		perror("malloc server read_buf");
		exit(1);
	}

	write_buf = malloc(sizeof(char) * BUFFER_SIZE);
	if(!write_buf)
	{
		printf("服务端创建写缓冲异常\n");
		perror("malloc server write_buf");
		exit(1);
	}
	memset(read_buf, 0, BUFFER_SIZE);
	memset(write_buf, 0, BUFFER_SIZE);
}

void clear_buf(char *buf)
{
	memset(buf, 0, BUFFER_SIZE);
}

//设置为非阻塞模式
void set_nonblocking(int sockfd)
{
	//获取状态
	int opts = fcntl(sockfd, F_GETFL);
	if(opts < 0)
	{
		perror("fcntl(F_GETFL)");
		exit(1);
	}

	opts |= O_NONBLOCK;
	int res = fcntl(sockfd, F_SETFL, opts);
	if(res < 0)
	{
		perror("fcntl(F_SETFL)");
		exit(1);
	}
}

int main(int argc, char const *argv[])
{
	init_buf();

	int sockfd, client_fd, temp_result;

	struct sockaddr_in server_addr, client_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	memset(&client_addr, 0, sizeof(client_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(SERVER_PROT);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	handle_error("socket", sockfd);

	temp_result = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	handle_error("bind", temp_result);

	temp_result = listen(sockfd, 128);

	//设置非阻塞模式
	set_nonblocking(sockfd);

	int epollfd, nfds;
	struct epoll_event ev, events[MAX_EVENTS];

	//创建epoll
	epollfd = epoll_create1(0);
	handle_error("epoll_create1", epollfd);

	ev.data.fd = sockfd;
	ev.events = EPOLLIN;
	temp_result = epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev);
	handle_error("epoll_ctl", temp_result);

	socklen_t cliaddr_len = sizeof(client_addr);

	while(1)
	{
		nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
		handle_error("epoll_wait", nfds);

		for(int i = 0; i < nfds; i++)
		{
			if(events[i].data.fd == sockfd)
			{
				client_fd = accept(sockfd, (struct sockaddr *)&client_addr, &cliaddr_len);
				handle_error("accept", client_fd);
				set_nonblocking(client_fd);
				printf("客户端from %s at port %d 文件描述符%d建立连接\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), client_fd);
				ev.data.fd = client_fd;
				ev.events = EPOLLIN | EPOLLET;
				epoll_ctl(epollfd, EPOLL_CTL_ADD, client_fd, &ev);
			}
			else if(events[i].events & EPOLLIN)
			{
				int count = 0, send_count = 0;
				client_fd = events[i].data.fd;
				while((count = recv(client_fd, read_buf, BUFFER_SIZE, 0)) > 0)
				{
					printf("recive message from client_fd: %d: %s\n", client_fd, read_buf);
					clear_buf(read_buf);
					strcpy(write_buf, "reveived~\n");
					send_count = send(client_fd, write_buf, strlen(write_buf), 0);
					handle_error("send", send_count);
					clear_buf(write_buf);
				}
				if(count == -1 && errno == EAGAIN)
				{
					printf("来自客户端client_fd: %d当前批次的数据已经读完，继续监听\n", client_fd);
				}
				else if(count == 0)
				{
					printf("客户端client_fd:%d请求关闭连接...\n", client_fd);
					strcpy(write_buf, "receive your shutdown signal\n");
					send_count = send(client_fd, write_buf, strlen(write_buf), 0);
					handle_error("send", send_count);
					clear_buf(write_buf);
					printf("从epoll文件描述符中集体移除client_fd: %d\n", client_fd);
					epoll_ctl(epollfd, EPOLL_CTL_DEL, client_fd, NULL);
					printf("释放client_fd:%d资源\n", client_fd);
					shutdown(client_fd, SHUT_WR);
					close(client_fd);
				}
			}
		}
	}
	printf("释放资源\n");
	close(epollfd);
	close(sockfd);
	free(read_buf);
	free(write_buf);

	return 0;
}
