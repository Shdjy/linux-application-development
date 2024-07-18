#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void my_signal_handler(int signum)
{
	printf("\n接收到%d信号，停止程序！\n", signum);
	exit(signum);
}
int main()
{
	//注册SIGINT消息处理函数(ctrl+c)
	if (signal(SIGINT, my_signal_handler) == SIG_ERR)
	{
		perror("注册自定义信号失败\n");
		return 1;
	}

	while(1)
	{
		sleep(2);
		printf("wait\n");
	}
	return 0;
}
