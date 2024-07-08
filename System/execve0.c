#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("参数不足无法创建\n");
        return 1;
    }
    printf("i am %s, id:%d\n", argv[1], getpid());

    return 0;
}