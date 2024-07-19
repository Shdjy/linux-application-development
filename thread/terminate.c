#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

typedef struct Result
{
	char* p;
	int len;
}Result;

void *red_thread(void *argv)
{
	Result *result = malloc(sizeof(Result));
	char code = *((char*)argv);
	char *ans = malloc(101);
	while (1)
	{
		fgets(ans, 100, stdin);
		if(ans[0] == code)
		{
			free(ans);
			printf("红玫瑰离开了！\n");
			char *redAns = strdup("红玫瑰独自去了伦敦\n");
			result->p = redAns;
			result->len = strlen(redAns);
			pthread_exit((void*)result);
		}
		else
		{
			printf("红玫瑰还在等你!\n");
		}
	}

	//return NULL;
}

void *white_thread(void*argv)
{
	Result *result = malloc(sizeof(Result));
	char code = *((char*)argv);
	char *ans = malloc(101);
	while (1)
	{
		fgets(ans, 100, stdin);
		if(ans[0] == code)
		{
			free(ans);
			printf("白玫瑰离开了！\n");
			char *redAns = strdup("白玫瑰独自去了纽约\n");
			result->p = redAns;
			result->len = strlen(redAns);
			pthread_exit((void*)result);
		}
		else
		{
			printf("白玫瑰还在等你!\n");
		}
	}

	//return NULL;
}


int main()
{
	pthread_t pid_red;
	pthread_t pid_white;
	char red_code = 'r';
	char white_code = 'w';
	Result *red_result = NULL;
	Result *white_result = NULL;

	pthread_create(&pid_red, NULL, red_thread, &red_code);
	pthread_create(&pid_white, NULL, white_thread, &white_code);

	pthread_join(pid_red, (void**)&red_result);
	printf("红玫瑰故事结束：%s\n", red_result->p);
	free(red_result->p);
	free(red_result);

	pthread_join(pid_white, (void**)&white_result);
	printf("白玫瑰故事结束:%s\n", white_result->p);
	free(white_result->p);
	free(white_result);

	return 0;
}
