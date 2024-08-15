#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

#define handle_error(cmd, result) \
	if(result < 0) \
	{ \
		perror(cmd); \
		return -1; \
	}

void dealer(int sig)
{
	pid_t pid;
	int status;
	while(pid = waitpid(-1, &status, WNOHANG))
	{
		if(WIFEXITED(status))
		{
			printf("子进程：%d以%d状态退出，已被回收\n", pid, WEXITSTATUS(status));
		}
		else if(WIFSIGNALED(status))
		{
			printf("子进程：%d被%d杀死，已被回收\n", pid, WTERMSIG(status));
		}
		else
		{
			printf("子进程：%d其他原因退出，已被回收\n", pid);
		}
	}

}

void *read_from_client(void *argv)
{
	int clientfd = *(int*)argv;
	ssize_t count = 0, send_count = 0;
	char *read_buf = NULL;
	char *write_buf = NULL;

	read_buf = malloc(sizeof(char) * 1024);
	if(!read_buf)
	{
		printf("服务端pid:%d创建读缓存失败，断开连接\n", getpid());
		shutdown(clientfd, SHUT_WR);
		close(clientfd);
		perror("malloc server read_buf");
		return NULL;
	}

	write_buf = malloc(sizeof(char) * 1024);
	if(!write_buf)
	{
		printf("服务端pid:%d穿件写缓存失败，断开连接\n", getpid());
		free(read_buf);
		shutdown(clientfd, SHUT_WR);
		close(clientfd);
		perror("malloc server write_buf");
		return NULL;
	}

	while((count = recv(clientfd, read_buf, 1024, 0)))
	{
		if(count < 0)
		{
			perror("recv");
		}
		printf("服务端pid：%d：receive message from client_fd:%d:%s", getpid(), clientfd, read_buf);
		sprintf(write_buf, "服务端pid:%d:received~\n", getpid());
		send_count = send(clientfd, write_buf, 1024, 0);
		if(send_count < 0)
		{
			perror("send");
		}
	}
	printf("服务端pid:%d,:客户端client_fd:%d请求关闭连接\n", getpid(), clientfd);
	sprintf(write_buf,"服务端pid:%d:recrive your shutdown signal\n", getpid());
	send_count = send(clientfd, write_buf, 1024, 0);
	if(send_count < 0)
	{
		perror("send");
	}

	printf("服务端pid:%d:释放client_fd:%d资源\n", getpid(), clientfd);
	shutdown(clientfd, SHUT_WR);
	close(clientfd);
	free(read_buf);
	free(write_buf);
	return NULL;
}

int main(int argc, char *argv[])
{
	int sockfd, temp_result;
	struct sockaddr_in server_addr, client_addr;

	memset(&server_addr, 0, sizeof(server_addr));
	memset(&client_addr, 0, sizeof(client_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(6666);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	handle_error("socket", sockfd);

	temp_result = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	handle_error("bind", temp_result);

	temp_result = listen(sockfd, 5);
	handle_error("listen", temp_result);

	socklen_t cliaddr_len = sizeof(client_addr);
	//注册信号处理函数，处理SIGCHLD，当一个子进程终止时发初这个信号
	signal(SIGCHLD, dealer);

	while(1)
	{
		int client_fd = accept(sockfd, (struct sockaddr *)&server_addr,&cliaddr_len);
		handle_error("accept", client_fd);

		pid_t pid = fork();
		if(pid > 0)
		{
			printf("this is father, pid is %d, continue accepting\n", getpid());
			//父进程中不需要处理client_fd，释放文件描述符，使其引用计数－１，使子进程释放client_fd后，引用计数变为０，从而释放资源．
			close(client_fd);
		}
		else if(pid == 0)
		{
			//子进程中不需要sockfd,释放
			close(sockfd);
			printf("与客户端%s at Port%d, 文件描述符%d建立连接\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), client_fd);
			printf("新客户端pid为：%d\n", getpid());
			read_from_client((void*)&client_fd);
			close(client_fd);
			exit(EXIT_SUCCESS);
		}
	}

	printf("释放资源\n");
	close(sockfd);

	return 0;
}
