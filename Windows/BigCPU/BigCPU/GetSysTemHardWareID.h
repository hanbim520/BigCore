#ifndef _H_GETSYSTEMHARDWAREID
#define _H_GETSYSTEMHARDWAREID
#include<wbemcli.h>
#include<string.h>
#include <iostream>
#include <Windows.h>
#include <comdef.h>
#include <Wbemidl.h>
#include "wincred.h"
#include <atlstr.h>
#pragma comment(lib, "credui.lib")
#pragma comment(lib,"wbemuuid.lib")
using namespace std;
#pragma once
class GetSysTemHardWareID
{
public:
	virtual ~GetSysTemHardWareID();
	GetSysTemHardWareID(const wchar_t* WMI_Namespace = L"ROOT\\CIMV2");
public:
	void Get_CpuId(CString& dest);
	void Get_CpuInfo(CString& dest);
	void Get_DiskDriveId(CString& dest);
	void Get_MacAddress(CString& dest);
	void Get_OsName(CString& dest);
	void Get_BaseBoardId(CString& dest);
	void Get_BiosSerialId(CString& dest);
	HRESULT hres;
private:
	IWbemServices* pSvc;
	IWbemLocator* pLoc;
	bool ExecQueryValue(char* strQueryLanguage, char* strQuery, IEnumWbemClassObject** pEnumerator);
private:
protected:
};
#endif // !_H_GETSYSTEMHARDWAREID