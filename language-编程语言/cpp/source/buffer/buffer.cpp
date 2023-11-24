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
	char*   p1 = NULL;// newbufָ��
	char*   p2 = NULL;// oldbufָ��
	int     alen = 0; // �䶯�ֽ���
	int     blen = 0; // �䶯�ֽ���
	int     dlen = 0; // �����ֽ���
	int     wlen = 0; // д���ֽ���

	int     xsiz1 = (size / 10, 64);
	int     xsiz2 = (size / 20, 16);

	for (p1 = (char*)newbuf, p2 = (char*)oldbuf; p1 < end; p1++, p2++)
	{
		if (*p1 == *p2)
		{
			continue;
		}
		// ��ȡһ��buffer�䶯
		p0 = p1;
		alen = 0;
		while (*p1 != *p2)
		{
			p1++;
			p2++;
			alen++;
		}
		// �Ƿ�д��
		dlen += p1 - p0;
		if (alen == dlen)
		{
			pa = p0;
			wlen = p1 - pa;
		}
		else
		{
			// ���̶������ݣ�
			wlen = p0 - pa;
			blen = p1 - pa;
			// ����ǰһ������
			if (blen - wlen > xsiz2 || wlen - dlen > xsiz1)
			{
				sprintf(temp, "%p:%d(%d)", pa, wlen, dlen);
				runlog.reg("write", temp);
				dlen = 0;
			}
			pb = p1;
		}
	}
	// ����δ�������ݣ�
	if (dlen > 0)
	{
		sprintf(temp, "%p:%d(%d)", pa, wlen, dlen);
		runlog.reg("write", temp);
	}
}
