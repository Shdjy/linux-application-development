#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    char* name = "SunHD";
    printf("i am %s, in parent process :%d\n", name, getpid());
    
    char* argv[] = {"/home/sunhd/code/linux/System/execve0", name, NULL};
    char* envs[] = {"/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:/snap/bin:/bin:/bin",NULL};

    int re = execve(argv[0], argv, envs);//在同一个进程中跳转执行另外一个程序
    if (re == -1)
    {
        printf("Jump failure!\n");
        return -1;
    }
    
    return 0;
}


/*exec系列函数  父进程跳转进入一个新进程
    推荐使用execve
    char *__path: 需要执行程序的完整路径名
    char *const __argv[]: 指向字符串数组的指针 需要传入多个参数
        (1) 需要执行的程序命令(同*__path)
        (2) 执行程序需要传入的参数
        (3) 最后一个参数必须是NULL
    char *const __envp[]: 指向字符串数组的指针 需要传入多个环境变量参数
        (1) 环境变量参数 固定格式 key=value
        (2) 最后一个参数必须是NULL
    return: 成功就回不来了 下面的代码都没有意义
            失败返回-1
    int execve (const char *__path, char *const __argv[], char *const __envp[]) 
    */