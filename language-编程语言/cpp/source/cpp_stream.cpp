#include <stdio.h>
#include <process.h>
#include "slicestream.h"
#include "winpthreads.h"

void add_stack(int no, int i)
{
	if (i % 100 == 0)
		printf("%d-%d\r\n", no, i);
	return add_stack(no, i + 1);
}

void* task_add(void* para)
{
	printf("new task %d\r\n", *((int*)para));
	printf("rest add stack\r\n");
	add_stack(*((int*)para), 1);
	printf("task %d exit.\r\n", *((int*)para));
	return NULL;
}

static int yield_count = 0;

void* task_yeild1(void* para)
{
	printf("new task %d\r\n", *((int*)para));
	printf("test yeild\r\n");
	while (--yield_count) Sleep(1);
	printf("task %d exit.\r\n", *((int*)para));
	return NULL;
}

void* task_yeild2(void* para)
{
	printf("new task %d\r\n", *((int*)para));
	printf("test yeild\r\n");
	//while (yield_count) ;
	//while (yield_count) Sleep(1);
	while (yield_count) pthread_yield();
	printf("task %d exit.\r\n", *((int*)para));
	return NULL;
}

void* task_join(void* para)
{
	printf("new task %d\r\n", *((int*)para));
	printf("test join\r\n");
	while (--yield_count) Sleep(1);
	printf("task %d exit.\r\n", *((int*)para));
	return NULL;
}


#include "stdfile.h"
#include "stdlogfile.h"

#include "stdtime.h"

int main(void)
{
	//int     cnt = 10000;
	//slog    runlog(1024);
	//remove(".\\run.txt");
	//runlog.open(".\\run.log");
	//while (cnt--)
	//{
	//	char buff[1024];
	//	sprintf(buff, "%d", cnt);
	//	runlog.reg("test", buff);
	//	//runlog.close();
	//	clock_t t1 = clock();
	//	_sleep(100);
	//	clock_t t2 = clock();
	//	printf("%dus\n", diffclock(t1, t2));
	//}

	////remove(".\\test.txt");
	////while (1) pthread_yield();
	//sfile   file;
	//file.open(".\\test.txt", "w");
	//file.write("test", 4);
	//file.close();
	////while (1) pthread_yield();


	for (int i = 0; i < 5; i++)
	{
		printf("%d ", i);
	}
	printf("\n");

	for (int j = 0; j < 5; ++j)
	{
		printf("%d ", j);
	}
	printf("\n");
}
