#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
	unsigned short local_num = 0x1f, network_num = 0;
	network_num = htons(local_num);
	printf("将主机字节序0x%hX,转化为网络字节序为0x%hX\n", local_num, network_num);
	
	local_num = ntohs(network_num);
	printf("将网络字节序0x%hX，转为主机字节序为0x%hX\n", network_num, local_num);
	return 0;

}
