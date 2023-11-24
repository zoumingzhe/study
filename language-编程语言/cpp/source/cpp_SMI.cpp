#include <iostream>
#include <comdef.h>
#include <Wbemidl.h>

#pragma comment(lib, "wbemuuid.lib")

using namespace std;

/************************************************************************/
/* ��ȡӲ����ʹ��ʱ����¶�                        */
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
			SafeArrayGetVartype(pIn, &vt);    //�����������
			dim = SafeArrayGetDim(pIn);    //���ά��
			long LBound;                           //�½�
			long UBound;                           //�Ͻ�
			SafeArrayGetLBound(pIn, 1, &LBound);    //����½�
			SafeArrayGetUBound(pIn, 1, &UBound);   //����Ͻ�
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
	cout << "Ӳ���Ѿ�ʹ����" << nTotalTime << "��Сʱ\t" << endl;
	cout << "Ӳ�����ڵ��¶���" << nTemperature << "��" << endl;


	double used_space;
	double free_space;
	double capacity;
	//GetDiskSpaceData(L"D:/", used_space, free_space, capacity);
	//cout << "��ʹ��=" << used_space << " GB" << endl;
	//cout << "ʣ��=" << free_space << " GB" << endl;
	//cout << "���ڴ�=" << capacity << " GB" << endl;

	return 0;
}
