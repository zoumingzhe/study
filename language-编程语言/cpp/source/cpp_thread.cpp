#include <stdio.h>
#include <process.h>
#include "thread.h"
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

int main(void)
{
	ThdCtl* p_thd = NULL;
	//p_thd->thd_no = 0;
	int num[5] = { 1, 2, 3, 4, 5 };
	yield_count = 10000;
	//int hStdOut0 = _beginthread(task_add, 0, num + 0);
	//int hStdOut1 = _beginthread(task_add, 1024, num + 1);
	//int hStdOut2 = _beginthread(task_add, 1024*1024, num + 2);
	//int hStdOut3 = _beginthread(task_add, 1024*1024*1024, num + 3);
	//int hStdOut1 = _beginthread(task_yeild1, 0, num + 1);
	//int hStdOut2 = _beginthread(task_yeild2, 0, num + 2);
	//HANDLE h;
	//pthread_create(&h, task_join, num + 0);
	//pthread_join(h, NULL);
	pthread_t ntid;
	pthread_create(&ntid, NULL, task_join, num + 0);
	pthread_join(ntid, NULL);
	printf("all task exit.\r\n");
	while (1) pthread_yield();
}
