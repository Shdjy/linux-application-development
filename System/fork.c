#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char* argv[])
{
    printf("parene process %d runing\n", getpid());

    pid_t pid = fork();

    if (pid < 0)
    {
        printf("child process create failed!\n");
    }
    else if (pid == 0)
    {
        printf("child create success id:%d,parent process is %d\n", getpid(), getppid());
    }
    else
    {
        printf("parent process %d create child process %d\n", getpid(), pid);
    }
    return 0;
}