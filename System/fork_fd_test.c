#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char* argv[])
{
    int fd = open("io.txt", O_CREAT | O_WRONLY | O_APPEND, 0644);
    if (fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }
    char buffer[1024];
    pid_t pid = fork();

    if(pid < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0)
    {
        strcpy(buffer,"child process write data!\n");
    }
    else
    {
        sleep(1);
        strcpy(buffer, "parent process write data!\n");
    }
    ssize_t bytes_writes = write(fd, buffer, strlen(buffer));

    if (bytes_writes == -1)
    {
        perror("write");
        close(fd);
        exit(EXIT_FAILURE);
    }
    printf("write over!\n");
    close(fd);

    if (pid == 0)
    {
        printf("child process write over!\n");
    }
    else
    {
        printf("parent process write over!\n");
    }

    return 0;
}
