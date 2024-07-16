#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

int main()
{
    int fd;
    char *pipe_path = "/tmp/myfifo";
    if (mkfifo(pipe_path, 0664) != 0)
    {
        perror("mkfifo failed");
        if (errno != 17)
        {
            exit(EXIT_FAILURE);   /* code */
        }
    }

    fd = open(pipe_path, O_WRONLY);
    if (fd == -1)
    {
        perror("open failed");
        exit(EXIT_FAILURE);
    }   
   char write_buf[100];
   ssize_t read_num;

   while ((read_num = read(STDIN_FILENO, write_buf, 100)) > 0)
   {
        //write_buf[read_num] = '\0';
        write(fd, write_buf, read_num);
   }

   if (read_num < 0)
   {
        perror("read");
        printf("read error!\n");
        close(fd);
        exit(EXIT_FAILURE);
   }
   
    printf("pipe over! process over!\n");
    close(fd);
    
    if(unlink(pipe_path) == -1)
    {
        perror("fifo_write unlink");
    }

    return 0;
}