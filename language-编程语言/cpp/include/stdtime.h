#ifndef __STD_TIME_H__
#define __STD_TIME_H__

#include <time.h>
#include "stdtypedef.h"

extern char* timestamp2str(i64 timestamp, char* str, size_t maxsize, cstr format = "%Y-%m-%d %H:%M:%S");
extern i64 diffclock(clock_t t1, clock_t t2);

#include <sys/timeb.h>

inline static i64 get_gmt()
{
	timeb t;
	ftime(&t);
	return ((i64)t.time);
}

inline static i64 get_gmt_ms()
{
	timeb t;
	ftime(&t);
	return ((i64)t.time * (i64)1000 + (i64)t.millitm);
}

#endif
