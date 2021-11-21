#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

typedef struct
{
	char name[16];
	char msg[64];
} thread_args;

static void *printHello(void* args)
{
	thread_args *thread_arg = (thread_args*) args;
	printf ("Hello World! I am: %s!\n", thread_arg->name);
	printf ("%s!\n", thread_arg->msg);

	pthread_exit(NULL);
}

int main(void)
{
	pthread_t threadID;
	int ret;
	thread_args thr;

	memset(&thr, 0x0, sizeof(thread_args));
	strncpy(thr.name, "vimentor", sizeof(thr.name));
	strncpy(thr.msg, "I am a new thread", sizeof(thr.msg));

	ret = pthread_create(&threadID, NULL, printHello, (&thr));
	if(ret)
	{
		printf("pthread_create() error number is %d\n", ret);
		return -1;
	}

	pthread_exit(NULL);
	return 0;
}
