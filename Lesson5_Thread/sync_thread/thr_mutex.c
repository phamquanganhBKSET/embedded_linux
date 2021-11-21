#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_THREAD 10

pthread_t tid[MAX_THREAD];

int count;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void *threadFunc(void* argv)
{
	pthread_mutex_lock(&mutex);
	count += 1;
	printf("Thread %d has started\n", count);

	//sleep(1);
	printf("Thread %d has finished\n", count);
	pthread_mutex_unlock(&mutex);

	pthread_exit(NULL);
}

int main(void)
{
	int i = 0;
	int ret = 0;

	for (i = 0; i < MAX_THREAD; i++)
	{
		ret = pthread_create(&(tid[i]), NULL, threadFunc, NULL);
		if (ret != 0)
		{
			printf("Thread [%d] created error\n", i);
		}
	}

	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);

	return 0;
}
