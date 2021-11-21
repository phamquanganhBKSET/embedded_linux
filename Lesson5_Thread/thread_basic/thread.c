/*=======Create new thread=======

#include <pthread.h>

int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start)(void *), void *arg);

=> Return 0 if success or a positive error number if fail
*/



/*===========Thread ID===========

#include <pthread.h>

pthread_t pthread_self(void);

=> Return thread ID of current thread
*/



/*=========Exit thread===========

#include <pthread.h>

void pthread_exit(void *retval);

*/

#include <pthread.h>
#include <stdio.h>

static void *printHello(void* argv)
{
	printf("Hello World! This is entry point of thread!\n");
	pthread_exit(NULL);
}

int main(void)
{
	pthread_t threadID;
	int ret;

	ret = pthread_create(&threadID, NULL, printHello, NULL);
	if(ret)
        {
                printf("pthread_create() error number=%d\n", ret);
                return -1;
        }
        pthread_exit(NULL);
	return 0;
}
