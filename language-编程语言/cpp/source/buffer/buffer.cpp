#include "buffer.h"
#include "stdlogfile.h"

void write_buffer(const char* buff, const int size)
{
	char    temp[1024];
	slog    runlog(1024);
	runlog.open(".\\write.log");
	sprintf(temp, "%p:%d", buff, size);
	runlog.reg("write", temp);
}

void write_buffer_slice(const char* newbuf, const char* oldbuf, const int size)
{
	char    temp[1024];
	slog    runlog(1024);
	runlog.open(".\\write.log");
	if (size <= 128)
	{
		sprintf(temp, "%p:%d", newbuf, size);
		runlog.reg("write", temp);
		return;
	}

	char*   end = (char*)newbuf + size;
	char*   pa = NULL;
	char*   pb = NULL;
	char*   p0 = NULL;
	char*   p1 = NULL;// newbuf指针
	char*   p2 = NULL;// oldbuf指针
	int     alen = 0; // 变动字节数
	int     blen = 0; // 变动字节数
	int     dlen = 0; // 差异字节数
	int     wlen = 0; // 写盘字节数

	int     xsiz1 = (size / 10, 64);
	int     xsiz2 = (size / 20, 16);

	for (p1 = (char*)newbuf, p2 = (char*)oldbuf; p1 < end; p1++, p2++)
	{
		if (*p1 == *p2)
		{
			continue;
		}
		// 读取一段buffer变动
		p0 = p1;
		alen = 0;
		while (*p1 != *p2)
		{
			p1++;
			p2++;
			alen++;
		}
		// 是否写盘
		dlen += p1 - p0;
		if (alen == dlen)
		{
			pa = p0;
			wlen = p1 - pa;
		}
		else
		{
			// 落盘多少数据？
			wlen = p0 - pa;
			blen = p1 - pa;
			// 落盘前一次数据
			if (blen - wlen > xsiz2 || wlen - dlen > xsiz1)
			{
				sprintf(temp, "%p:%d(%d)", pa, wlen, dlen);
				runlog.reg("write", temp);
				dlen = 0;
			}
			pb = p1;
		}
	}
	// 尚有未落盘数据？
	if (dlen > 0)
	{
		sprintf(temp, "%p:%d(%d)", pa, wlen, dlen);
		runlog.reg("write", temp);
	}
}
