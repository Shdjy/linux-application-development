//计数信号量用于进程通信
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

int main()
{
	char *shm_name = "unnamed_sem_shm";
	int fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
	ftruncate(fd, sizeof(sem_t));

	sem_t *sem = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	sem_init(sem, 1, 0);

	int pid = fork();
	if(pid > 0)
	{
		sem_wait(sem);
		printf("this is father\n");
		waitpid(pid, NULL, 0);
	}
	else if(pid == 0)
	{
		sleep(1);
		printf("this is son\n");
		sem_post(sem);
	}
	else
	{
		perror("fork");
	}

	if(pid > 0)
	{
		if(sem_destroy(sem) ==  -1)
		{
			perror("father sem_destroy");
		}
	}

	if(munmap(sem, sizeof(sem_t)) == -1)
	{
		perror("munmap");
	}
	if(close(fd) == -1)
	{
		perror("close");
	}

	if(pid > 0)
	{
		if(shm_unlink(shm_name) == -1)
		{
			perror("father shm_unlink");
		}
	}

	return 0;
}
