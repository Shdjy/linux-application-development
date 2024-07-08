#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

int main(int argc, char *argv)
{
    char *name = "oldStu";
    printf("%s%d in old room sutdy\n", name, getpid());
    pid_t pid = fork();
    if(pid == -1)
    {
        printf("new student intvite failed!\n");
    }
    else if (pid == 0)
    {
        char *newName = "execve0";
        char *argv[] = {"/home/sunhd/code/linux/System/execve0", newName, NULL};
        char *envp[] = {NULL};
        int re = execve(argv[0], argv, envp);
        if (re == -1)
        {
            printf("failed");
            return 1;
        }
    }
    else
    {
        printf("old student:%d invite new student:%d failed\n", getpid(), pid);
        char byte = getchar();
    }
    
    return 0;
}