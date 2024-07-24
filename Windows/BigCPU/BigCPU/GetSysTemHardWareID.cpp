#include "GetSysTemHardWareID.h"
#include <vector>
#include <iostream> 
#include <list>
using namespace std;
GetSysTemHardWareID::GetSysTemHardWareID(const wchar_t* WMI_Namespace)
{
    pSvc = NULL;
    pLoc = NULL;
    hres = NULL;

    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres))
    {
        printf("Failed to initialize COM library. Error code = 0x");
    }
    hres = CoInitializeSecurity(
        NULL,
        -1,
        NULL,
        NULL,
        RPC_C_AUTHN_LEVEL_DEFAULT,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE,
        NULL
    );

    if (FAILED(hres))
    {
        printf("Failed to initialize security. Error code = 0x");
        CoUninitialize();
    }
    hres = CoCreateInstance(
        CLSID_WbemLocator,
        0,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator, (LPVOID*)&pLoc);

    if (FAILED(hres))
    {
        printf("Failed to create IWbemLocator object.");
        CoUninitialize();

    }
    hres = pLoc->ConnectServer(
        _bstr_t(WMI_Namespace),
        NULL,
        NULL,
        0,
        NULL,
        0,
        0,
        &pSvc
    );

    if (FAILED(hres))
    {
        printf("Could not connect. Error code = 0x");
        pLoc->Release();
        CoUninitialize();
    }
    printf("Connected to ROOT\\CIMV2 WMI namespace");

    hres = CoSetProxyBlanket(
        pSvc,
        RPC_C_AUTHN_WINNT,
        RPC_C_AUTHZ_NONE,
        NULL,
        RPC_C_AUTHN_LEVEL_CALL,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE
    );

    if (FAILED(hres))
    {
        printf("Could not set proxy blanket. Error code = 0x");
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
    }
}
GetSysTemHardWareID::~GetSysTemHardWareID()
{
    pSvc->Release();
    pLoc->Release();
    CoUninitialize();
    hres = NULL;
}

void GetSysTemHardWareID::Get_CpuId(CString& dest)
{
    IEnumWbemClassObject* pEnumerator = NULL;
    ExecQueryValue((char*)"WQL", (char*)"SELECT * FROM Win32_Processor", &pEnumerator);
    IWbemClassObject* pclsObj = NULL;
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
        vtProp.vt = VT_BSTR;
        hr = pclsObj->Get(L"ProcessorId", 0, &vtProp, 0, 0);
        printf("ProcessorId:\n%ws", vtProp.bstrVal);
        printf("\n");
        dest = vtProp.bstrVal;
        VariantClear(&vtProp);
        pclsObj->Release();
    }
    pEnumerator->Release();
}


void GetSysTemHardWareID::Get_CpuInfo(CString& dest)
{
    IEnumWbemClassObject* pEnumerator = NULL;
    ExecQueryValue((char*)"WQL", (char*)"SELECT CurrentClockSpeed  FROM Win32_Processor", &pEnumerator);
    IWbemClassObject* pclsObj = NULL;
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
        vtProp.vt = VT_BSTR;
        hr = pclsObj->Get(L"CurrentClockSpeed", 0, &vtProp, 0, 0);
        printf("CurrentClockSpeed:\n%d", vtProp.ulVal);
        printf("\n");
      //  dest = vtProp.bstrVal;
        VariantClear(&vtProp);
        pclsObj->Release();
    }
    pEnumerator->Release();
}



void GetSysTemHardWareID::Get_DiskDriveId(CString& dest)
{
    IEnumWbemClassObject* pEnumerator = NULL;
    ExecQueryValue((char*)"WQL", (char*)"SELECT * FROM Win32_DiskDrive WHERE (SerialNumber IS NOT NULL) AND (MediaType LIKE 'Fixed hard disk%')", &pEnumerator);
    IWbemClassObject* pclsObj = NULL;
    ULONG uReturn = 0;
    int i = 0;
    CString strN;
    while (pEnumerator)
    {
        i++;
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1,
            &pclsObj, &uReturn);
        if (0 == uReturn)
        {
            break;
        }
        VARIANT vtProp;
        vtProp.vt = VT_BSTR;
        hr = pclsObj->Get(L"SerialNumber", 0, &vtProp, 0, 0);
        strN = strN + vtProp.bstrVal + _T(";");
        dest = vtProp.bstrVal;
        dest.Replace(_T(" "), _T(""));
        strN.Replace(_T(" "), _T(""));
        printf("SerialNumber[%d]:\n%ws", i, dest.GetString());
        printf("\n");
        VariantClear(&vtProp);
        pclsObj->Release();
    }
    pEnumerator->Release();
    i = 0;
    dest = strN;
}

void GetSysTemHardWareID::Get_MacAddress(CString& dest)
{
    IEnumWbemClassObject* pEnumerator = NULL;
    ExecQueryValue((char*)"WQL", (char*)"SELECT * FROM Win32_NetworkAdapter WHERE (MACAddress IS NOT NULL) AND (NOT (PNPDeviceID LIKE 'ROOT%'))", &pEnumerator);
    IWbemClassObject* pclsObj = NULL;
    ULONG uReturn = 0;
    vector<CString> MACAddress;
    while (pEnumerator)
    {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1,
            &pclsObj, &uReturn);
        if (0 == uReturn)
        {
            break;
        }
        VARIANT vtProp;
        VARIANT vtProp2;
        vtProp.vt = VT_BSTR;
        vtProp2.vt = VT_BSTR;
        hr = pclsObj->Get(L"Description", 0, &vtProp2, 0, 0);
        printf("Description:\n%ws", vtProp2.bstrVal);
        printf("\n");
        dest = vtProp2.bstrVal;
        dest += _T(" ; ");
        pclsObj->Get(L"MACAddress", 0, &vtProp, 0, 0);
        MACAddress.push_back((CString)(vtProp.bstrVal));
        printf("MACAddress:\n%ws", vtProp.bstrVal);
        printf("\n");
        VariantClear(&vtProp);
        pclsObj->Release();
        break;
    }
    if (MACAddress.size() >= 1)
    {
        dest = dest + MACAddress[0];
    }
    /*  else if (MACAddress.size()>1)
      {
          for (size_t i = 0; i < MACAddress.size(); i++)
          {
              dest = dest + MACAddress[i];
          }
      }*/
    MACAddress.clear();
    pEnumerator->Release();
}

void GetSysTemHardWareID::Get_OsName(CString& dest)
{
    IEnumWbemClassObject* pEnumerator = NULL;
    ExecQueryValue((char*)"WQL", (char*)"SELECT * FROM Win32_OperatingSystem", &pEnumerator);
    IWbemClassObject* pclsObj = NULL;
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
        vtProp.vt = VT_BSTR;
        hr = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
        printf("OS NAME:\n%ws", vtProp.bstrVal);
        printf("\n");
        dest = vtProp.bstrVal;
        VariantClear(&vtProp);
        pclsObj->Release();
        break;
    }
    pEnumerator->Release();
}

void GetSysTemHardWareID::Get_BaseBoardId(CString& dest)
{
    IEnumWbemClassObject* pEnumerator = NULL;
    ExecQueryValue((char*)"WQL", (char*)"SELECT * FROM Win32_BaseBoard WHERE (SerialNumber IS NOT NULL)", &pEnumerator);
    IWbemClassObject* pclsObj = NULL;
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
        vtProp.vt = VT_BSTR;
        hr = pclsObj->Get(L"SerialNumber", 0, &vtProp, 0, 0);
        printf("Board SerialNumber:\n%ws", vtProp.bstrVal);
        printf("\n");
        dest += _T("BaseBoardId:");
        dest += vtProp.bstrVal;
        VariantClear(&vtProp);
        pclsObj->Release();
        break;
    }
    pEnumerator->Release();
}

void GetSysTemHardWareID::Get_BiosSerialId(CString& dest)
{
    IEnumWbemClassObject* pEnumerator = NULL;
    ExecQueryValue((char*)"WQL", (char*)"SELECT * FROM Win32_BIOS WHERE SerialNumber IS NOT NULL", &pEnumerator);
    IWbemClassObject* pclsObj = NULL;
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
        vtProp.vt = VT_BSTR;
        hr = pclsObj->Get(L"SerialNumber", 0, &vtProp, 0, 0);
        printf("BIOS SerialNumber:\n%ws", vtProp.bstrVal);
        printf("\n");
        dest += _T("BIOSSerialId:");
        dest += vtProp.bstrVal;
        VariantClear(&vtProp);
        pclsObj->Release();
    }
    pEnumerator->Release();
}

bool GetSysTemHardWareID::ExecQueryValue(char* strQueryLanguage, char* strQuery, IEnumWbemClassObject** pEnumerator)
{
    HRESULT hre = pSvc->ExecQuery(
        bstr_t(strQueryLanguage),
        bstr_t(strQuery),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        pEnumerator);
    if (FAILED(hres))
    {
        printf("Query for operating system name failed.");
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return FALSE;
    }
    return true;
}