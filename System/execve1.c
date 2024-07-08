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