#include <sys/types.h>
#include <time.h>
#include <mqueue.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char *mq_name = "/p_c_mq";
	struct mq_attr attr;
	attr.mq_curmsgs = 0;
	attr.mq_flags = 0;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = 100;

	mqd_t mqdes = mq_open(mq_name, O_CREAT | O_WRONLY, 0666, &attr);
	if(mqdes == (mqd_t)-1)
	{
		perror("mq_open");
	}

	char writeBuf[100];
	struct timespec time_info;

	while (1)
	{
		bzero(writeBuf, 100);
		ssize_t read_count = read(0, writeBuf, 100);
		if(read_count == -1)
		{
			perror("read");
			continue;
		}
		
		clock_gettime(0, &time_info);
		time_info.tv_sec += 5;

		if(read_count == 0)
		{
			printf("Received EOF, exit...\n");
			char eof = EOF;
			if(mq_timedsend(mqdes, &eof, 1, 0, &time_info) == -1)
			{
				perror("mq_timedsend");
			}
			break;
		}
		if(mq_timedsend(mqdes, writeBuf, strlen(writeBuf),0, &time_info) == -1)
		{
			perror("mq_timedsend");
		}
		printf("从命令行接受到的数据，已经发送给消费者\n");
	}

	close(mqdes);

	return 0;
}
