#include <bits/time.h>
#include <bits/types/clockid_t.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mqueue.h>
#include <time.h>
#include <string.h>

int main()
{
	char *mq_name = "/p_c_mq";
	struct mq_attr attr;
	attr.mq_curmsgs = 0;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = 100;
	attr.mq_flags = 0;

	mqd_t mqds = mq_open(mq_name, O_CREAT | O_RDONLY, 0666, &attr);

	if(mqds == (mqd_t)-1)
	{
		perror("mq_open");
	}

	char readBuf[100];
	struct timespec time_info;

	while (1)
	{
		bzero(readBuf, 100);
		clock_gettime(CLOCK_REALTIME, &time_info);
		time_info.tv_sec += 86400;

		if(mq_timedreceive(mqds, readBuf, 100, NULL, &time_info) == -1)
		{
			perror("mq_timedreceive");
		}

		if(readBuf[0] == EOF)
		{
			printf("接受到生产者的终止信号，准备推出．．．\n");
			break;
		}
		printf("接收到来自生产者的信息\n%s", readBuf);
	}
	close(mqds);

	mq_unlink(mq_name);
	return 0;
}
