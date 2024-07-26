//有名信号量用作计数信号量
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
	char *sem_name = "/named_sem";
	sem_t *sem = sem_open(sem_name, O_CREAT, 0666, 0);
	pid_t pid = fork();
	if(pid > 0)
	{
		sem_wait(sem);
		printf("this is father\n");
		waitpid(pid, NULL, 0);

		sem_close(sem);
		if(sem_unlink(sem_name) == -1)
		{
			perror("sem_unlink");
		}
	}
	else if(pid == 0)
	{
		sleep(1);
		printf("this is son\n");
		sem_post(sem);

		sem_close(sem);
	}
	else
	{
		perror("fork");
	}
	return 0;
}
