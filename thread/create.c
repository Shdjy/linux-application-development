#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUF_LEN 1024

char *buf;

void *input_thread(void *argv)
{
	int i = 0;
	while(1)
	{
		char c = fgetc(stdin);
		if(c && c != '\n')
		{
			buf[i++] = c;
			if(i >= BUF_LEN)
			{
				i = 0;
			}
		}
	}
	return NULL;
}

void *output_thread(void *argv)
{
	int i = 0; 
	while(1)
	{
		if(buf[i])
		{
			fputc(buf[i], stdout);
			fputc('\n', stdout);

			buf[i++] = 0;
			if(i >= BUF_LEN)
			{
				i = 0;
			}
		}
		else {
		sleep(1);
		}
	}
	return NULL;
}

int main()
{
	pthread_t pid_input;
	pthread_t pid_output;

	buf = malloc(BUF_LEN);

	memset(buf, 0, BUF_LEN);
	/* for(int i = 0; i < BUF_LEN; i++) */
	/* { */
	/* 	buf[i] = 0; */
	/* } */

	pthread_create(&pid_input, NULL, input_thread, NULL);
	pthread_create(&pid_output, NULL, output_thread, NULL);
	
	pthread_join(pid_input, NULL);
	pthread_join(pid_output, NULL);

	free(buf);

	return 0;
}
