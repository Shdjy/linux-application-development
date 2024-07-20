#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *task(void *arg)
{
	printf("子线程开始工作\n");
	sleep(2);
	printf("子线程结束工作\n");
	return NULL;
}
int main()
{
	pthread_t tid;
	pthread_create(&tid,NULL, task, NULL);
	pthread_detach(tid);//pthread_detach不会等待子线程结束，主线程结束子线程会强制结束
						//
	printf("主线程继续工作\n");
	sleep(3);
	printf("主线程结束\n");


	return 0;
}
