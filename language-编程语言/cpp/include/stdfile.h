#ifndef __STD_FILE_H__
#define __STD_FILE_H__

#include <stdio.h>
#include <stdlib.h>

/* sfile �ļ�������
* 1.�˽ӿ��ṩ�ļ�io����
* 2.�˽ӿ���ֱ���ļ�io�ӿ���ȣ��ṩ��һ���ڴ滺��
* 3.�˽ӿڵ��ڴ滺�������߿��豸�Ķ�дЧ�ʣ����ᵼ��д�������ͺ������������������flush����
* 4.���˴˽ӿ��е��ڴ滺�棬����ϵͳ�㻹����ά�����ļ�����
* 5.�˽ӿ��ṩ����ͬ��io�ӿڣ������첽io�ӿ�
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
	"r"		��һ�����ڶ�ȡ���ļ������ļ�������ڡ�
	"w"		����һ������д��Ŀ��ļ�������ļ��������Ѵ��ڵ��ļ���ͬ�����ɾ�������ļ������ݣ��ļ�����Ϊһ���µĿ��ļ���
	"a"		׷�ӵ�һ���ļ���д�������ļ�ĩβ׷�����ݡ�����ļ������ڣ��򴴽��ļ���
	"r+"	��һ�����ڸ��µ��ļ����ɶ�ȡҲ��д�롣���ļ�������ڡ�
	"w+"	����һ�����ڶ�д�Ŀ��ļ���
	"a+"	��һ�����ڶ�ȡ��׷�ӵ��ļ���
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
