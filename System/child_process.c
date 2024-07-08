#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("param < 2\n");
        return 1;   
    }
    printf("i am %s %d \n", argv[1], getpid());

    sleep(100);
    return 0;
}