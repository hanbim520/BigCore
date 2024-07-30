//By Navy Zhang 710605420@qq.com 
#include "WindowsCpuInfo.h"
#include "InstructionSet.h"

std::vector<int64_t> WindowsCpuInfo::GetCurrentClockSpeed_MHz(CpuInfoData& CpuInfo)
{
    std::string QueryLanguage = "WQL";
    //std::string QueryString(
    //    "SELECT Name, Manufacturer, NumberOfCores, NumberOfLogicalProcessors, MaxClockSpeed, L2CacheSize, L3CacheSize "
    //    "FROM Win32_Processor");
    std::string QueryString("SELECT * FROM Win32_PerfFormattedData_Counters_ProcessorInformation");

    IEnumWbemClassObject* pEnumerator = NULL;
    bool Success = ExecQueryValue(QueryLanguage.c_str(), QueryString.c_str(), &pEnumerator);
    if (!Success)
    {
        return {};
    }
    std::vector<int64_t> result;

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

        CpuInfoData Cpu;

        VARIANT vtProp;
        vtProp.vt = VT_BSTR;

        hr = pclsObj->Get(L"PercentProcessorPerformance", 0, &vtProp, nullptr, nullptr);
        if (SUCCEEDED(hr)) {

            double performance = std::stod(wstring_to_std_string(vtProp.bstrVal)) / 100;
            result.push_back(static_cast<int64_t>(static_cast<double>(CpuInfo._maxClockSpeed_MHz) * performance));
        }

        VariantClear(&vtProp);
        pclsObj->Release();
    }
    pEnumerator->Release();

    return result;
}

std::vector<double> WindowsCpuInfo::GetThreadsUtilisation(CpuInfoData& CpuInfo)
{
    std::string QueryLanguage = "WQL";
    //std::string QueryString(
    //    "SELECT Name, Manufacturer, NumberOfCores, NumberOfLogicalProcessors, MaxClockSpeed, L2CacheSize, L3CacheSize "
    //    "FROM Win32_Processor");
    std::string QueryString("SELECT * FROM Win32_PerfFormattedData_Counters_ProcessorInformation");

    IEnumWbemClassObject* pEnumerator = NULL;
    bool Success = ExecQueryValue(QueryLanguage.c_str(), QueryString.c_str(), &pEnumerator);
    if (!Success)
    {
        return {};
    }
    std::vector<double> result;

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

        CpuInfoData Cpu;

        VARIANT vtProp;
        vtProp.vt = VT_BSTR;

        hr = pclsObj->Get(L"PercentProcessorUtility", 0, &vtProp, nullptr, nullptr);
        if (SUCCEEDED(hr)) {

            std::string data = wstring_to_std_string(vtProp.bstrVal);
            if (data.empty()) {
                result.push_back(-1.f);
            }
            else {
                result.push_back(std::stod(data) / 100.f);
            }
        }

        VariantClear(&vtProp);
        pclsObj->Release();
    }
    pEnumerator->Release();

    return result;
}

WindowsCpuInfo::WindowsCpuInfo(const wchar_t* WMI_Namespace)
{
    pSvc = NULL;
    pLoc = NULL;
    Hres = NULL;

    Hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(Hres))
    {
        printf("Failed to initialize COM library. Error code = 0x");
    }
    Hres = CoInitializeSecurity(
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

    if (FAILED(Hres))
    {
        printf("Failed to initialize security. Error code = 0x");
        CoUninitialize();
    }
    Hres = CoCreateInstance(
        CLSID_WbemLocator,
        0,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator, (LPVOID*)&pLoc);

    if (FAILED(Hres))
    {
        printf("Failed to create IWbemLocator object.");
        CoUninitialize();

    }
    Hres = pLoc->ConnectServer(
        _bstr_t(WMI_Namespace),
        NULL,
        NULL,
        0,
        NULL,
        0,
        0,
        &pSvc
    );

    if (FAILED(Hres))
    {
        printf("Could not connect. Error code = 0x");
        pLoc->Release();
        CoUninitialize();
    }
    //printf("Connected to ROOT\\CIMV2 WMI namespace");

    Hres = CoSetProxyBlanket(
        pSvc,
        RPC_C_AUTHN_WINNT,
        RPC_C_AUTHZ_NONE,
        NULL,
        RPC_C_AUTHN_LEVEL_CALL,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE
    );

    if (FAILED(Hres))
    {
        printf("Could not set proxy blanket. Error code = 0x");
       if(pSvc) pSvc->Release();
       if(pLoc) pLoc->Release();
        CoUninitialize();

        Hres = NULL;
    }
}

WindowsCpuInfo::~WindowsCpuInfo()
{
   if(pSvc) pSvc->Release();
   if (pLoc) pLoc->Release();
    CoUninitialize();
    Hres = NULL;
}

bool WindowsCpuInfo::ExecQueryValue(const char* strQueryLanguage, const char* strQuery, IEnumWbemClassObject** pEnumerator)
{
    HRESULT hre = pSvc->ExecQuery(
        bstr_t(strQueryLanguage),
        bstr_t(strQuery),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        pEnumerator);
    if (FAILED(hre))
    {
        printf("Query for operating system name failed.");
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return FALSE;
    }
    return true;
}

std::vector<CpuInfoData> WindowsCpuInfo::GetAllCpus()
{
    std::string QueryLanguage = "WQL";
    //std::string QueryString(
    //    "SELECT Name, Manufacturer, NumberOfCores, NumberOfLogicalProcessors, MaxClockSpeed, L2CacheSize, L3CacheSize "
    //    "FROM Win32_Processor");
    std::string QueryString("SELECT * FROM Win32_Processor");

    IEnumWbemClassObject* pEnumerator = NULL;
    bool Success = ExecQueryValue(QueryLanguage.c_str(), QueryString.c_str(), &pEnumerator);
    if (!Success)
    {
        return {};
    }
    std::vector<CpuInfoData> CpuInfoDatas;


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

        CpuInfoData Cpu;

        VARIANT vtProp;
        vtProp.vt = VT_BSTR;
        hr = pclsObj->Get(L"Name", 0, &vtProp, nullptr, nullptr);
        if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_BSTR)) {
            Cpu._modelName = wstring_to_std_string(vtProp.bstrVal);
        }
        hr = pclsObj->Get(L"Manufacturer", 0, &vtProp, nullptr, nullptr);
        if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_BSTR)) {
            Cpu._Manufacturer = wstring_to_std_string(vtProp.bstrVal);
        }
        hr = pclsObj->Get(L"NumberOfCores", 0, &vtProp, nullptr, nullptr);
        if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_I4)) {
            Cpu._numPhysicalCores = vtProp.intVal;
        }
        hr = pclsObj->Get(L"NumberOfLogicalProcessors", 0, &vtProp, nullptr, nullptr);
        if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_I4)) {
            Cpu._numLogicalCores = vtProp.intVal;
        }
        hr = pclsObj->Get(L"MaxClockSpeed", 0, &vtProp, nullptr, nullptr);
        if (SUCCEEDED(hr) && (V_VT(&vtProp) == VT_I4)) {
            Cpu._maxClockSpeed_MHz = vtProp.uintVal;
            Cpu._regularClockSpeed_MHz = vtProp.uintVal;
        }
       // const InstructionSet::InstructionSet_Internal InstructionSet::CPU_Rep;

        InstructionSet Instructions;
        //std::vector<std::string> SupportInstructions = Instructions.GetSupportInstructions();
        //Cpu._Features = std::vector<std::string>(SupportInstructions.begin(), SupportInstructions.end());
        Cpu._Features = Instructions.GetSupportInstructions();
        CpuInfoDatas.push_back(Cpu);

        VariantClear(&vtProp);
        pclsObj->Release();
    }
    pEnumerator->Release();

    return CpuInfoDatas;
}
