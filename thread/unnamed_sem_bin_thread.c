/**
 * @brief 在sem指向的地址初始化一个无名信号量。
 * 
 * @param sem 信号量地址
 * @param pshared 指明信号量是线程间共享还是进程间共享的
 * 0: 信号量是线程间共享的，应该被置于所有线程均可见的地址（如，全局变量或在堆中动态分配的变量）
 * 非0: 信号量是进程间共享的，应该被置于共享内存区域，任何进程只要能访问共享内存区域，即可操作进程间共享的信号量
 * @param value 信号量的初始值
 * @return int 成功返回0，失败返回-1，同时errno被设置以记录错误信息
 */
//int sem_init(sem_t *sem, int pshared, unsigned int value);
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

sem_t unnamed_sem;
int shard_num = 0;

void *plusOne(void *argv)
{
	sem_wait(&unnamed_sem);
	int tmp = shard_num + 1;
	shard_num = tmp;
	sem_post(&unnamed_sem);
	return NULL;
}

int main()
{
	sem_init(&unnamed_sem, 0, 1);
	pthread_t tid[10000];
	for(int i = 0; i < 10000; i++)
	{
		pthread_create(tid + i, NULL, plusOne, NULL);
	}

	for(int i = 0; i < 10000; i++)
	{
		pthread_join(tid[i], NULL);
	}

	printf("shard_num is %d\n", shard_num);
	return 0;
}
