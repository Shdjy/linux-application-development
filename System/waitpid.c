#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    int subprocess_status;
    printf("main process\n");

    pid_t pid = fork();
    if (pid == -1)
    {
        printf("create failed!\n");
    }
    else if (pid == 0)
    {
       char *args[] = {"/usr/bin/ping", "-c", "10", "www.atguigu.com", NULL};
       char *envs[] = {NULL, NULL};
       printf("new request 10\n");
       int re = execve(args[0], args, envs);

       if (re == -1)
       {
            printf("create failed!\n");
            return 1;
       }
    }
    else
    {
        printf("main process %d wait new %d request\n", getpid(), pid);
        waitpid(pid, &subprocess_status, 0);
    }
    printf("wait new over!\n");

    return 0;
}





