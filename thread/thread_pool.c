#include <stdio.h>
#include <glib.h>
#include <unistd.h>
#include <stdlib.h>

void task_func(gpointer data, gpointer user_data)
{
	int task_num = *(int*)data;
	free(data);
	printf("executing task is %d...\n", task_num);
	sleep(1);
	printf("task %d completed\n", task_num);
}

int main()
{
	GThreadPool *thread_pool = g_thread_pool_new(task_func, NULL, 5, TRUE, NULL);

	for(int i = 0; i < 10; i++)
	{
		int *tmp = malloc(sizeof(int));
		*tmp = i + 1;
		g_thread_pool_push(thread_pool, tmp, NULL);
	}

	g_thread_pool_free(thread_pool, FALSE, TRUE);
	printf("all task completed\n");

	return 0;
}
