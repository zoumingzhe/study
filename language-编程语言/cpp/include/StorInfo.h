#ifndef STOR_INFO
#define STOR_INFO


struct StorSpaceInfo
{
	char*   directory;
	__int64 capacity;
	__int64 used_bytes;
	__int64 free_bytes;
};


#endif /* STOR_INFO */
