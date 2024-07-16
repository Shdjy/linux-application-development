#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int pipefd[2];
    pid_t cpid;
    char buf;
    if(argc != 2)
    {
        fprintf(stderr, "%s:need information\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if (pipe(pipefd) == -1)
    {
        /* code */
        perror("pipe create failed!\n");
        exit(EXIT_FAILURE);
    }
    cpid = fork();
    if (cpid == -1)
    {
        /* code */
        perror("process create failed!\n");
        exit(EXIT_FAILURE);
    }
   if (cpid == 0)
   {
    /* code */
        close(pipefd[1]);//1-write
        char str[100] = {0};
        sprintf(str, "child process%d\n", getpid());
        write(STDOUT_FILENO, str, sizeof(str));
        while (read(pipefd[0], &buf, 1) > 0)
        {
            write(STDOUT_FILENO, &buf, 1);
        }
        write(STDOUT_FILENO, "\n", 1);
        close(pipefd[0]);
        _exit(EXIT_SUCCESS);
   }
   else
   {
        close(pipefd[0]);
        printf("main process%d\n", getpid());
        write(pipefd[1], argv[1], strlen(argv[1]));
        close(pipefd[1]);
        waitpid(cpid, NULL, 0);
        exit(EXIT_SUCCESS);
   }
    
    
}