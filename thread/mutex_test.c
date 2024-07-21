#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define THREAD_COUNT 20000

static pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;

void *add_thread(void *argv)
{
	int *p = (int*)argv;
	pthread_mutex_lock(&counter_mutex);
	(*p)++;
	pthread_mutex_unlock(&counter_mutex);
	return NULL;
}

int main()
{
	pthread_t pid[THREAD_COUNT];

	int num = 0;
	for(int i = 0; i < THREAD_COUNT; i++)
	{
		pthread_create(&pid[i], NULL,add_thread, &num);
	}

	for (int i = 0; i < THREAD_COUNT; i++)
	{
		pthread_join(pid[i], NULL);

	}

	printf("累加结果：%d\n", num);
	return 0;
}
