#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main()
{
	struct sockaddr_in server_addr;
	struct in_addr server_in_addr;
	in_addr_t server_in_addr_t;

	memset(&server_addr, 0, sizeof(server_addr));
	memset(&server_in_addr, 0, sizeof(server_in_addr));
	memset(&server_in_addr_t, 0, sizeof(server_in_addr_t));

	printf("192.168.6.101的十六进制：0x%X 0x%X ox%X 0x%X\n", 192, 168, 6, 101);
	server_in_addr_t = inet_addr("192.168.6.101");//主机地址转为二进制
	printf("inet_addr convert: 0x%X\n", server_in_addr.s_addr);

	inet_aton("192.168.6.101", &server_in_addr);
	printf("inet_aton convert: 0x%X\n", server_in_addr.s_addr);

	inet_pton(AF_INET, "192.168.6.101", &server_addr.sin_addr);//字符串转sockin_addr结构体
	printf("inet_pton 后server_addr.sin_addr的十六进制为0x%X\n", server_addr.sin_addr.s_addr);

	//结构体转字符串
	printf("通过inet_ntoa打印inet_pton转化后的地址: %s\n", inet_ntoa(server_addr.sin_addr));

	printf("打印本地网络部分：0x%X\n", inet_lnaof(server_addr.sin_addr));
	printf("打印网络号：0x%X\n", inet_netof(server_addr.sin_addr));

	//将本地地址和网络号拼接成网络字节序的互联网主机地址
	server_addr.sin_addr = inet_makeaddr(inet_netof(server_addr.sin_addr), 102);

	printf("inet_makeaddr: 0x%X\n", server_addr.sin_addr.s_addr);
	printf("通过inet_ntoa打印iner_makeaddr拼接后的地址%s\n", inet_ntoa(server_addr.sin_addr));

	return 0;
}
