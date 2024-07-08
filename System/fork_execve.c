#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    char* name = "SunHD";
    printf("%s,%d in main\n", name, getpid());

    pid_t pid = fork();
    if (pid == -1)
    {
        printf("create failed!\n");
    }
    else if (pid == 0)
    {
       char *newname = "jiaqi";
       char *args[] = {"/home/sunhd/code/linux/System/execve0", newname, NULL};
       char *envs[] = {NULL, NULL};
       int re = execve(args[0], args, envs);
       if (re == -1)
       {
            printf("create failed!\n");
            return 1;
       }
    }
    else
    {
        printf("%s, %d in main,%d in new\n", name, getpid(), pid);
    }
    

    return 0;
}





