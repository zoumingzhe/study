#include <stdio.h>
#include <process.h>
#include "slicestream.h"
#include "winpthreads.h"

#include "buffer.h"

#include "stdfile.h"
#include "stdlogfile.h"

#include "stdtime.h"

int main(void)
{
	int     size = 8192;
	char    oldbuff[8192];
	char    newbuff[8192];

	memset(oldbuff, 0x0, size);
	memset(newbuff, 0x0, size);
	newbuff[0] = 1;
	//newbuff[1] = 0;
	newbuff[2] = 2;
	newbuff[3] = 3;
	//newbuff[4] = 0;
	newbuff[5] = 4;
	newbuff[6] = 5;
	newbuff[7] = 6;
	//newbuff[8] = 0;
	newbuff[9] = 7;
	//newbuff[10] = 0;
	newbuff[11] = 10;

	write_buffer(newbuff, size);
	write_buffer_slice(newbuff, oldbuff, size);
}
