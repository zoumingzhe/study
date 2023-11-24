#include "stdtime.h"

#define GMT_E8_MS 28800000

char* timestamp2str(i64 gmt_ms, char* str, size_t maxsize, cstr format)
{
	/* 获取 GMT 时间 */
	gmt_ms += GMT_E8_MS;
	i64 second = gmt_ms / 1000;
	struct tm* tm = gmtime(&second);
	if (tm != NULL)
	{
		strftime(str, maxsize, format, tm);
	}
	return str;
}

i64 diffclock(clock_t t1, clock_t t2)
{
	i64 diff = t2 - t1;
	diff *= (i64)1000000;
	return (diff / (i64)CLOCKS_PER_SEC);
}
