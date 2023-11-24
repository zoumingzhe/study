#include <stdio.h>
#include <process.h>
#include "thread.h"
#include "winpthreads.h"

#include <windows.h>

#define CacheLine 64
#define MaxThdNum 8
#define RepeatNum 10

struct ThdPara
{
	char thd_no;
	char repeat;
	char *str;
	long *num;
};

int get_diff_time(SYSTEMTIME sys0, SYSTEMTIME sys1)
{
	int t0 = sys0.wMilliseconds + sys0.wSecond*1000 + sys0.wMinute*60000 + sys0.wHour*3600000;
	int t1 = sys1.wMilliseconds + sys1.wSecond*1000 + sys1.wMinute*60000 + sys1.wHour*3600000;
	return t1-t0;
}

void* task_todo(void* para)
{
	SYSTEMTIME sys0, sys1;
	ThdPara* tp = (ThdPara*)para;
	int *timer = (int*)malloc(sizeof(int)*tp->repeat);

	for (size_t i = 0; i < tp->repeat; i++)
	{
		*(tp->num) = 100000000;
		Sleep(100);
		GetLocalTime(&sys0);
		while (--(*(tp->num)) > 0);
		GetLocalTime(&sys1);
		Sleep(100);
		timer[i] = get_diff_time(sys0, sys1);
	}

	Sleep(1000);
	int  sum = 0;
	char tmp[128];
	tp->str = (char*)malloc(1024);
	sprintf(tp->str, "%d,", tp->thd_no);
	for (size_t i = 0; i < tp->repeat; i++)
	{
		sprintf(tmp, "%d,", timer[i]);
		strcat(tp->str, tmp);
		sum += timer[i];
	}
	sprintf(tmp, "%d,%d\n", sum, sum / tp->repeat);
	strcat(tp->str, tmp);
	
	free(timer);
	return NULL;
}

int main(void)
{
	ThdPara *tp = (ThdPara*)malloc(sizeof(ThdPara) * MaxThdNum);
	pthread_t *htid = (pthread_t*)malloc(sizeof(pthread_t) * MaxThdNum);

	for (size_t siz = 8; siz <= CacheLine; siz *= 2)
	{
		printf("\r\nsizeof:%d\r\n", siz);
		char* ptr = (char*)malloc(siz * MaxThdNum);
		memset(tp, 0x0, sizeof(ThdPara) * MaxThdNum);
		memset(htid, 0x0, sizeof(pthread_t) * MaxThdNum);

		for (size_t n = 1; n <= MaxThdNum; n++)
		{
			printf("\r\n**********thd:%03d**********\r\n", n);

			for (size_t j = 0; j < n; j++)
			{
				tp[j].thd_no = j + 1;
				tp[j].repeat = RepeatNum;
				tp[j].num = (long*)(ptr + siz * j);
				pthread_create(htid + j, NULL, task_todo, tp + j);
			}

			for (size_t j = 0; j < n; j++)
			{
				pthread_join(htid[j], NULL);
			}

			char tmp[128];
			sprintf(tmp, ".\\test\\%d-thd%d.csv", siz, n);
			FILE* fp = fopen(tmp, "a");
			for (size_t j = 0; j < n; j++)
			{
				fwrite(tp[j].str, strlen(tp[j].str), 1, fp);
				printf("%s", tp[j].str);
				free(tp[j].str);
			}
			fclose(fp);
		}
		free(ptr);
	}

	free(tp);
	free(htid);
	printf("all task ok.\r\n");
	while (1) pthread_yield();
}
