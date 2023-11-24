#include <iostream>
#include <comdef.h>
#include <Wbemidl.h>

#pragma comment(lib, "wbemuuid.lib")

using namespace std;

/************************************************************************/
/* 获取硬盘总使用时间和温度                        */
/************************************************************************/
void GetTotalTimeAndTemperature(int& nTemperature, int &nTotalTime)
{
	HRESULT hres;
	hres = CoInitializeEx(0, COINIT_MULTITHREADED);

	hres = CoInitializeSecurity(
		NULL,
		-1,                          // COM authentication
		NULL,                        // Authentication services
		NULL,                        // Reserved
		RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication 
		RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation  
		NULL,                        // Authentication info
		EOAC_NONE,                   // Additional capabilities 
		NULL                         // Reserved
	);


	IWbemLocator *pLoc = NULL;

	hres = CoCreateInstance(
		CLSID_WbemLocator,
		0,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator, (LPVOID *)&pLoc);

	IWbemServices *pSvc = NULL;

	hres = pLoc->ConnectServer(
		_bstr_t(L"ROOT\\WMI"), // Object path of WMI namespace
		NULL,                    // User name. NULL = current user
		NULL,                    // User password. NULL = current
		0,                       // Locale. NULL indicates current
		NULL,                    // Security flags.
		0,                       // Authority (e.g. Kerberos)
		0,                       // Context object 
		&pSvc                    // pointer to IWbemServices proxy
	);

	hres = CoSetProxyBlanket(
		pSvc,                        // Indicates the proxy to set
		RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
		RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
		NULL,                        // Server principal name 
		RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx 
		RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
		NULL,                        // client identity
		EOAC_NONE                    // proxy capabilities 
	);

	IEnumWbemClassObject* pEnumerator = NULL;
	hres = pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t("SELECT * FROM MSStorageDriver_ATAPISmartData"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator);

	IWbemClassObject *pclsObj;
	ULONG uReturn = 0;

	while (pEnumerator)
	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1,
			&pclsObj, &uReturn);

		if (0 == uReturn)
		{
			break;
		}

		VARIANT vtProp;
		VariantInit(&vtProp);

		// Get the value of the VendorSpecific property
		hr = pclsObj->Get(L"VendorSpecific", 0, &vtProp, 0, 0);

		if (vtProp.vt == VT_ARRAY | VT_UI1)
		{
			SAFEARRAY *pIn;
			pIn = vtProp.parray;
			VARTYPE vt;
			UINT dim;
			SafeArrayGetVartype(pIn, &vt);    //获得数据类型
			dim = SafeArrayGetDim(pIn);    //获得维数
			long LBound;                           //下界
			long UBound;                           //上界
			SafeArrayGetLBound(pIn, 1, &LBound);    //获得下界
			SafeArrayGetUBound(pIn, 1, &UBound);   //获得上界
			BYTE *pdata = new BYTE[UBound - LBound + 1];
			ZeroMemory(pdata, UBound - LBound + 1);

			BYTE *buf;
			SafeArrayAccessData(pIn, (void **)&buf);
			memcpy(pdata, buf, UBound - LBound + 1);
			SafeArrayUnaccessData(pIn);

			BYTE* pTemp = pdata + 2;
			for (int i = 2; i < UBound - LBound + 1; i += 12)
			{
				pTemp = pdata + i;
				if (*pTemp == 0xc2)
				{
					//Beep(1000,200);
					nTemperature = *(pTemp + 5);
				}

				if (*pTemp == 0x09)
				{
					//Beep(1000,200);
					nTotalTime = (*(pTemp + 5)) + (*(pTemp + 6) << 8);
				}

			}
		}
		VariantClear(&vtProp);
	}
	CoUninitialize();
}

//extern void GetDiskSpaceData(const wstring& path, double& used_space, double& free_space, double& capacity);

int main(int argc, char* argv[])
{
	int nTemperature = 0;
	int nTotalTime = 0;
	GetTotalTimeAndTemperature(nTemperature, nTotalTime);
	cout << "硬盘已经使用了" << nTotalTime << "个小时\t" << endl;
	cout << "硬盘现在的温度是" << nTemperature << "℃" << endl;


	double used_space;
	double free_space;
	double capacity;
	//GetDiskSpaceData(L"D:/", used_space, free_space, capacity);
	//cout << "已使用=" << used_space << " GB" << endl;
	//cout << "剩余=" << free_space << " GB" << endl;
	//cout << "总内存=" << capacity << " GB" << endl;

	return 0;
}
