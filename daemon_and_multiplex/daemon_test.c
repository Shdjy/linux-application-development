#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syslog.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <syslog.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

pid_t pid;
int is_shutdown = 0;


void signal_handler(int sig)
{
	switch(sig)
	{
		case SIGHUP:
			syslog(LOG_WARNING, "收到SIGHUP信号．．．");
			break;
		case SIGTERM:
			syslog(LOG_NOTICE, "接受中止信号，准备推出守护进程...");
			syslog(LOG_NOTICE, "向子进程发送SIGTERＭ信号...");
			is_shutdown = 1;
			kill(pid, SIGTERM);
			break;
		default:
			syslog(LOG_INFO, "Received unhandled signal");
	}
}

void my_daemon()
{
	pid_t pid;

	pid = fork();
	if(pid < 0)
	{
		exit(1);
	}
	if(pid > 0)
	{
		exit(0);
	}
	if(setsid() < 0)
	{
		exit(1);
	}
	signal(SIGHUP, signal_handler);
	signal(SIGTERM, signal_handler);
	
	pid = fork();
	if(pid < 0)
	{
		exit(1);
	}
	if(pid > 0)
	{
		exit(0);
	}
	//重置umask
	umask(0);
	//将文件目录切换为根目录
	chdir("/");
	//关闭所有文件描述符
	for(int x = 0; x <= sysconf(_SC_OPEN_MAX); x++)
	{
		close(x);
	}

	openlog("this is our daemonize process:", LOG_PID, LOG_DAEMON);
}

int main()
{
	my_daemon();

	while(1)
	{
		pid = fork();

		if(pid > 0)
		{
			syslog(LOG_INFO, "守护进程正在监听服务端进程...");
			waitpid(-1, NULL, 0);
			if(is_shutdown)
			{
				syslog(LOG_NOTICE, "子进程以被回收，即将关闭syslog连接，收回进程退出");
				closelog();
				exit(EXIT_SUCCESS);
			}
			syslog(LOG_ERR, "服务端进程终止，３后重启．．．");
			sleep(3);
		}
		else if(pid == 0)
		{
			syslog(LOG_INFO, "子进程fork成功");
			syslog(LOG_INFO, "启动服务端进程");
			char *path = "/home/sunhd/code/linux-application-development/daemon_and_multiplex/tcp_server";
			char *argv[] = {"tcp_server",NULL};
			errno = 0;
			execve(path, argv, NULL);
			char buf[1024];
			sprintf(buf, "errno: %d", errno);
			syslog(LOG_ERR, "服务端进程启动失败");
			exit(EXIT_FAILURE);
		}
		else
		{
			syslog(LOG_ERR, "子进程fork失败");
		}
	}
	return 0;
}
