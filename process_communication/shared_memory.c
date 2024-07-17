/*************************************************************************
	> File Name: shared_memory.c
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<sys/wait.h>
#include<string.h>

int main()
{
	char *share;
	pid_t pid;
	char shmName[100] = {0};
	sprintf(shmName, "/letter%d", getpid());
	
	int fd;
	//const char *name:共享内存对象的名称
	//参数２：int oflag：打开模式
	//		１．O_CREAR:不存在就创建
	//		２．O_EXCL：与O_CREAT一起使用时，共享内存嗯已经存在时返回错误避免覆盖
	//		３．O_RDONLY：只读
	//		４．O_RDWR:读写
	//		５．O_TRUNC：截断现有对象至０长度
	//参数３，权限0644
	fd = shm_open(shmName, O_CREAT | O_RDWR, 0644);
	if(fd < 0)
	{
		perror("共享内存对象创建失败！\n");
		exit(EXIT_FAILURE);
	}
	//将指定文件描述符截取制定大小
	ftruncate(fd, 100);
	//将文件映射到内存区域
	//参数１：期望映射的地址
	//参数２：映射内存区域长度
	//参数３：映射区域保护标志
	//		１．PROT_READ：允许读取映射区域
	//		２．PORT_WRITE:允许写入
	//		3. POT_EXEC:允许执行
	//		4.PROT_NONE：页面不可访问
	//参数４：映射选项标志
	//		１．MAP_SHARED：映射区域共享
	//		２.MAP_PRIVATE:私有，对映射区域的修改不会影响原始文件,修改保存到副本中
	//		３．MAP_ANONYMOUS：匿名映射，不与文件关联
	//		4．MAP_FIXED：映射到制定地址，不允许映射返回错误
	//参数４：fd指定映射的文件或设备
	//参数５:从文件开头的偏移量
	//返回值：成功安徽映射区域起始地址，出错返回（void*)-1
	share = mmap(NULL, 100, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(share == MAP_FAILED)
	{
		perror("共享内存对象映射到内存失败！\n");
		exit(EXIT_FAILURE);
	}

	close(fd);
	pid = fork();
	if(pid == 0)
	{
		strcpy(share, "你是个好人！\n");
		printf("新学员%d完成回信！\n", getpid());
	}
	else
	{
		sleep(1);
		printf("老学员%d收到新学员%d的回信：%s", getpid(), pid, share);
		wait(NULL);
		int ret = munmap(share, 100);
		if (ret == -1)
		{
			perror("munmap");
			exit(EXIT_FAILURE);		
		}
		shm_unlink(shmName);

		return  0;
	}
}

