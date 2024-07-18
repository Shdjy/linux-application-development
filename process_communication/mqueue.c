#include <bits/types/clockid_t.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <mqueue.h>
#include <string.h>

int main()
{
	struct mq_attr attr;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = 100;

	attr.mq_flags = 0;
	attr.mq_curmsgs = 0;//当前队列消息数量
	
	char *mq_name = "/father_son_mq";
	mqd_t mqdes = mq_open(mq_name, O_RDWR | O_CREAT, 0664, &attr);//返回消息队列的描述符，mqd_t识int的重命名，失败返回－１
	if(mqdes == (mqd_t)-1)
	{
		perror("mq_open");
		exit(EXIT_FAILURE);
	}
	pid_t pid = fork();
	if (pid < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if(pid == 0)
	{
		char read_buf[100];
		struct timespec time_info;
		for(int i = 0; i < 10; i++)
		{
			bzero(read_buf, 100);
			clock_gettime(0, &time_info);//获取当前真实时间的时钟
			time_info.tv_sec += 15;
			//接受消息打印到控制台
			if(mq_timedreceive(mqdes, read_buf, 100, NULL, &time_info) == -1)
			{
				perror("mq_timedreceive");
			}
			printf("子进程接受到数据：%s\n", read_buf);
		}
	}
	else
	{
		char send_buf[100];
		struct timespec time_info;

		for(int i = 0; i < 10; i++)
		{
			bzero(send_buf, 100);
			sprintf(send_buf, "父进程第%d次发送消息\n", (int)(i+1));
			clock_gettime(0, &time_info);
			time_info.tv_sec += 5;
			if(mq_timedsend(mqdes, send_buf, strlen(send_buf), 0, &time_info) == -1)
			{
				perror("mq_timedsend");
			}
			printf("父进程发送一条消息休眠一秒\n");
			sleep(1);
		}
	}
	//释放消息队列的引用
	close(mqdes);
	if(pid > 0)
	{
		mq_unlink(mq_name);
	}


	return 0;
}
