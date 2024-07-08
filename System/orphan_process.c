#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    printf("main process%d\n", getpid());
    pid_t pid = fork();
    if (pid == -1)
    {
        printf("create failed!\n");
        /* code */
    }
    else if (pid == 0)
    {
        char *name = "sunhd";
        char *argv[] = {"/home/sunhd/code/linux/System/child_process", name, NULL};
        char *enpv[] = {NULL};
        int re = execve(argv[0], argv, enpv);
        if(re == -1)
        {
            printf("child process create failed!\n");
            return 1;
        }
        /* code */
    }
   else
   {
        printf("this is parent process %d,child process %d\n", getpid(), pid);
   } 
   
   return 0; 
}