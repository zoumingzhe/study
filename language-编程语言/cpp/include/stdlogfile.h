#ifndef __STD_LOGFILE_H__
#define __STD_LOGFILE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stdtime.h"

#define min(a,b) (a)<(b)?(a):(b)
#define max_log_len 1024

class slog {
private:
	FILE* stream;
	char* buffer;
	long int msize;
	//long int csize;
public:
	slog(long int max_size)
	{
		stream = NULL;
		msize = max_size;
		////csize = 0x0L;
		buffer = (char*)malloc(BUFSIZ);
	}

	~slog()
	{
		if (stream)
		{
			fflush(stream);
			fclose(stream);
		}
		if (buffer)
		{
			free(buffer);
		}
	}

	FILE* open(const char* filename)
	{
		if (stream = fopen(filename, "a"))
		{
			setbuf(stream, buffer);
			//csize = ftell(stream);
		}
		return stream;
	}

	void reg(const char* title, const char* str)
	{
		if (!stream)
			return;

		//if (ftell(stream) > msize)
		//{
		//	if (stream)
		//	{
		//		fflush(stream);
		//		fclose(stream);
		//	}
		//	rename(oldname, newname);
		//	if (stream = fopen(filename, "a"))
		//	{
		//		setbuf(stream, buffer);
		//		csize = ftell(stream);
		//	}
		//}

		// todo : 
		// 1.生成时间戳并记录
		char timestamp[64];
		timestamp2str(get_gmt_ms(), timestamp, 64, "%Y-%m-%d %H:%M:%S");
		// 2.自动更换文件

		size_t wlen = 0;
		wlen = strlen(timestamp);
		fwrite(timestamp, wlen, 1, stream);
		fwrite("\t", 1, 1, stream);
		wlen = strlen(title);
		fwrite(title, wlen, 1, stream);
		fwrite("\t", 1, 1, stream);
		wlen = min(strlen(str), max_log_len);
		fwrite(str, wlen, 1, stream);
		fwrite("\n", 1, 1, stream);
		return;
	}
};

#endif
