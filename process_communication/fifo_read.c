#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main()
{
    int fd;
    char *pipe_path = "/tmp/myfifo";
    fd = open(pipe_path, O_RDONLY);
    if (fd == -1/* condition */)
    {
        perror("open failed");
        exit(EXIT_FAILURE);
    }

    char read_buf[100];
    ssize_t read_num;
    while ((read_num = read(fd, read_buf, 100)) > 0)
    {
        write(STDOUT_FILENO, read_buf, read_num);
    }
    if (read_num < 0)
    {
        perror("read");
        printf("read failed!\n");
        exit(EXIT_FAILURE);
    }
    printf("pipe over, process over!\n");
    close(fd);
    
    
    
    return 0;
}