#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BUFFER_SIZE 5
int buffer[BUFFER_SIZE];
int count = 0;

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *producer(void *arg)
{
	int item = 1;
	while(1)
	{
		pthread_mutex_lock(&mutex);
		while(count == BUFFER_SIZE)
		{
			pthread_cond_wait(&cond, &mutex);
		}
		buffer[count++] = item++;
		printf("发出一个幸运数字%d\n", buffer[count-1]);
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

void *consumer(void *arg)
{
	while(1)
	{
		pthread_mutex_lock(&mutex);
		while(count == 0)
		{
			pthread_cond_wait(&cond, &mutex);
		}
		printf("收到了幸运数字%d\n", buffer[--count]);
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

int main()
{
	pthread_t thread_producer, thread_consumer;
	pthread_create(&thread_producer, NULL, producer, NULL);
	pthread_create(&thread_consumer, NULL, consumer, NULL);
	pthread_join(thread_producer, NULL);
	pthread_join(thread_consumer, NULL);

	return 0;
}
