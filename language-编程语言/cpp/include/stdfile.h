#ifndef __STD_FILE_H__
#define __STD_FILE_H__

#include <stdio.h>
#include <stdlib.h>

/* sfile 文件操作类
* 1.此接口提供文件io操作
* 2.此接口与直接文件io接口相比，提供了一层内存缓存
* 3.此接口的内存缓存可以提高块设备的读写效率，但会导致写操作的滞后，若需立即落盘需调用flush操作
* 4.除了此接口中的内存缓存，操作系统层还可能维护了文件缓冲
* 5.此接口提供的是同步io接口，而非异步io接口
*/

class sfile {
private:
	FILE* stream;
	char* buffer;
public:
	sfile()
	{
		stream = NULL;
		buffer = (char*)malloc(BUFSIZ);
	}

	~sfile()
	{
		if (!stream)
		{
			fflush(stream);
			fclose(stream);
		}
		if (buffer)
		{
			free(buffer);
		}
	}

	/*
	"r"		打开一个用于读取的文件。该文件必须存在。
	"w"		创建一个用于写入的空文件。如果文件名称与已存在的文件相同，则会删除已有文件的内容，文件被视为一个新的空文件。
	"a"		追加到一个文件。写操作向文件末尾追加数据。如果文件不存在，则创建文件。
	"r+"	打开一个用于更新的文件，可读取也可写入。该文件必须存在。
	"w+"	创建一个用于读写的空文件。
	"a+"	打开一个用于读取和追加的文件。
	*/
	FILE* open(const char* filename, const char* mode)
	{
		if (stream = fopen(filename, mode))
		{
			setbuf(stream, buffer);
		}
		return stream;
	}

	FILE* reopen(const char* filename, const char* mode)
	{
		if (!stream)
		{
			stream = fopen(filename, mode);
		}
		else
		{
			stream = freopen(filename, mode, stream);
		}

		if (stream = fopen(filename, mode))
		{
			setbuf(stream, buffer);
		}
		return stream;
	}

	int flush()
	{
		if (!stream) return EOF;
		return fflush(stream);
	}

	int close()
	{
		if (!stream) return EOF;
		else fflush(stream);
		return fclose(stream);
	}

	long int tell()
	{
		if (!stream) return EOF;
		return ftell(stream);
	}

	int seek(long int offset, int whence)
	{
		if (!stream) return EOF;
		return fseek(stream, offset, whence);
	}

	size_t read(void* ptr, size_t size)
	{
		if (!stream) return EOF;
		return fread(ptr, size, 1, stream);
	}

	size_t read(void* ptr, size_t size, size_t nmemb)
	{
		if (!stream) return EOF;
		return fread(ptr, size, nmemb, stream);
	}

	size_t write(const void* ptr, size_t size)
	{
		if (!stream) return EOF;
		return fwrite(ptr, size, 1, stream);
	}

	size_t write(const void* ptr, size_t size, size_t nmemb)
	{
		if (!stream) return EOF;
		return fwrite(ptr, size, nmemb, stream);
	}
};

#endif
