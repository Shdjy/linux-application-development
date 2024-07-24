#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
	char *shm_sem_name = "unnamed_sem_shm_sem";
	char *shm_value_name = "unnamed_sem_shm_value";
	int sem_fd = shm_open(shm_sem_name, O_CREAT | O_RDWR, 0666);
	int value_fd = shm_open(shm_value_name, O_CREAT | O_RDWR, 0666);
	ftruncate(value_fd, sizeof(int));//调整共享内存大小
	ftruncate(sem_fd, sizeof(sem_t));
	
	sem_t *sem = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED, sem_fd, 0);
	int *value = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, value_fd, 0);//将内存对象映射到共享内存，返回映射区域指针
																						  //
	sem_init(sem, 1, 1);
	*value = 0;
	int pid = fork();

	if(pid > 0)
	{
		sem_wait(sem);
		int tmp = *value + 1;
		sleep(1);
		*value = tmp;
		sem_post(sem);
		waitpid(pid, NULL, 0);
		printf("this is father, child finished\n");
		printf("the final value is %d\n", *value);
	}
	else if(pid == 0)
	{
		sem_wait(sem);
		int tmp = *value + 1;
		sleep(1);
		*value = tmp;
		sem_post(sem);
	}
	else
	{
		perror("fork");
	}

	if(pid > 0)
	{
		if(sem_destroy(sem) == -1)
		{
			perror("sem_destroy");
		}
	}

	if(munmap(sem, sizeof(sem)) == -1)
	{
		perror("munmap sem");
	}

	if(munmap(value, sizeof(int)) == -1)
	{
		perror("munmap value");
	}
	if(close(sem_fd) == -1)
	{
		perror("close sem");
	}
	if(close(value_fd) == -1)
	{
		perror("close value");
	}

	if(pid > 0)
	{
		if(shm_unlink(shm_sem_name) == -1)
		{
			perror("fathre shem_unlink shm_sem_name");
		}
		if(shm_unlink(shm_value_name) == -1)
		{
			perror("father shem_unlink shm_value_name");
		}
	}
	return 0;
}
