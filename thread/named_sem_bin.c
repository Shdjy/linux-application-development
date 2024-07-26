//有名信号量用作二进制信号量
#include <stdio.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
	char *sem_name = "/named_sem";
	char *shm_name = "named_sem_shm";

	sem_t *sem = sem_open(sem_name, O_CREAT, 0666, 1);
	int fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);

	ftruncate(fd, sizeof(int));

	int *value = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	*value = 0; 
	pid_t pid = fork();

	if(pid < 0)
	{
		perror("fork");
	}
	sem_wait(sem);
	int tmp = *value + 1;
	sleep(1);
	*value = tmp;
	sem_post(sem);

	sem_close(sem);

	if(pid > 0)
	{
		waitpid(pid, NULL, 0);
		printf("子进程结束，value = %d\n", *value);
		sem_unlink(sem_name); 
	}
	munmap(value, sizeof(int));
	close(fd);

	if(pid > 0)
	{
		if(shm_unlink(shm_name) == -1)
		{
			perror("shm_unlink");
		}
	}
	return 0;
}
