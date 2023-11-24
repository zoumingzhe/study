#include <stdio.h>
#include <process.h>
#include "winpthreads.h"
#include "stdlogfile.h"

#include "stdtime.h"


#include <sys/timeb.h>
i64 get_sys_time_ms_by_ftime()
{
	timeb t;
	ftime(&t);
	return ((i64)t.time * (i64)1000 + (i64)t.millitm);
}

#define __OS_WINDOWS__ 1
//#define __OS_LINUX__   2

#ifdef __OS_WINDOWS__
#include <windows.h>

inline static i64 get_sys_time_ms_by_GetLocalTime()
{
	SYSTEMTIME s;
	GetLocalTime(&s);
	//printf("It's %d/%d/%d %02d:%02d:%02d now.", s.wYear, s.wMonth, s.wDay, s.wHour, s.wMinute, s.wMinute);
	//printf("\nAnd it's the %d day of a week.", s.wDayOfWeek);
	return 0;
}
#else
define_error
#endif




/*
* ¹¤¾ßÁ´½Ó£º
* https://tool.lu/timestamp/
*/

#define test_ms(function) do {\
	char temp[64];\
	clock_t t1, t2;\
	pthread_yield();\
	t1 = clock();\
	for (i64 i = 0; i < loop; i++)\
	{\
		function();\
	}\
	t2 = clock();\
	i64 ms = function();\
	i64 use = diffclock(t1, t2);\
	printf("%lld - %s - %s - %lldms / %lld = %lldns\n",\
		ms,\
		timestamp2str(ms, temp, 64),\
		#function,\
		(i64)(use / (i64)1000),\
		(i64)loop,\
		(i64)(use * (i64)1000) / loop);\
} while(0);

int main(void)
{
	i64 loop = 10000000;
	for (size_t i = 0; i < 10; i++)
	{
		pthread_yield();
		test_ms(get_sys_time_ms_by_ftime);
		test_ms(get_sys_time_ms_by_GetLocalTime);
	}
}
