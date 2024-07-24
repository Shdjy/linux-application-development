#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
	char* shm_value_name = "unnamed_sem_shm_value";
	int value_fd = shm_open(shm_value_name, O_CREAT | O_RDWR, 0666);
	ftruncate(value_fd, sizeof(int));//调整共享内存大小

	int *value = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, value_fd, 0);//将内存对象映射到共享内存，返回映射区域指针
	*value = 0;
	int pid = fork();

	if(pid > 0)
	{
		int tmp = *value + 1;
		sleep(1);
		*value = tmp;
		waitpid(pid, NULL, 0);
		printf("this is father, child finished\n");
		printf("the final value is %d\n", *value);
	}
	else if(pid == 0)
	{
		int tmp = *value + 1;
		sleep(1);
		*value = tmp;
	}
	else
	{
		perror("fork");
	}

	if(munmap(value, sizeof(int)) == -1)
	{
		perror("munmap value");
	}
	if(close(value_fd) == -1)
	{
		perror("close value");
	}

	if(pid > 0)
	{
		if(shm_unlink(shm_value_name) == -1)
		{
			perror("father shem_unlink shm_value_name");
		}
	}
	return 0;
}
