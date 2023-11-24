#include <iostream>
#include <comdef.h>
//#include <Wbemidl.h>

using namespace std;

#include <StorInfo.h>

/************************************************************************/
/* 获取硬盘空间函数        单位GB                    */
/************************************************************************/
//void GetDiskSpaceData(const wstring& path, double& used_space, double& free_space, double& capacity)
void GetStorSpaceInfo(const char *path, StorSpaceInfo &p_info)
{
	//std::wstring wide_path = path;;
	//if (wide_path.empty())
	//	return;

	//LPCSTR multi_path = (LPCSTR)wide_path.c_str();

	ULARGE_INTEGER uFreeByteAvailable, uTotalByte, uTotalFreeBytes;
	//if (!GetDiskFreeSpaceEx(path.c_str(), &uFreeByteAvailable, &uTotalByte, &uTotalFreeBytes))
	//{
	//	return;
	//}
	//else
	//{
	//	ULONGLONG free_space_data = uFreeByteAvailable.QuadPart / 1024 / 1024 / 1024;
	//	ULONGLONG total_free = uTotalFreeBytes.QuadPart / 1024 / 1024 / 1024;
	//	ULONGLONG total_giga_bytes = uTotalByte.QuadPart / 1024 / 1024 / 1024;

	//	p_info.used_bytes = (double)(total_giga_bytes - total_free);
	//	p_info.free_bytes = free_space_data;
	//	p_info.capacity = total_giga_bytes;
	//}

}
