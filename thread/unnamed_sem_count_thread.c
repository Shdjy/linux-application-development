//计数信号量控制线程执行顺序
#include <stdio.h>
#include <sys/select.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

sem_t *full;
sem_t *empty;
int shard_num = 0;

int rand_num()
{
	srand(time(NULL));
	return rand();
}

void *producer(void *argv)
{
	for(int i = 0; i < 5; i++)
	{
		sem_wait(empty);
		printf("\n========第%d轮数据传输==========\n",i+1);
		sleep(1);
		shard_num = rand_num();
		printf("生产者发送数据\n");
		sem_post(full);
	}
	return NULL;
}

void *consumer(void *argb)
{
	for(int i = 0; i < 5; i++)
	{
		sem_wait(full);
		printf("消费者接受数据\n");
		printf("shard num is %d\n", shard_num);
		sleep(1);
		sem_post(empty);
	}
	return NULL;
}


int main()
{
	full = malloc(sizeof(sem_t));
	empty = malloc(sizeof(sem_t));

	sem_init(empty, 0, 1);
	sem_init(full, 0, 0);
	pthread_t producer_id, consumer_id;

	pthread_create(&producer_id, NULL, producer, NULL);
	pthread_create(&consumer_id, NULL, consumer, NULL);

	pthread_join(producer_id, NULL);
	pthread_join(consumer_id, NULL);

	sem_destroy(full);
	sem_destroy(empty);
	return 0;
}
