#include <iostream>
#include <Windows.h>
#include <wbemidl.h>
#include <comdef.h>  // 包含 _bstr_t 的定义
#include "WindowsCpuInfo.h"
#include<assert.h>
#pragma comment(lib, "wbemuuid.lib")

int main() {


    WindowsCpuInfo cpuInfo;

    std::vector<CpuInfoData> CpuInfoDatas = cpuInfo.GetAllCpus();
    std::vector<int64_t> ClockSpeed = cpuInfo.GetCurrentClockSpeed_MHz(CpuInfoDatas[0]);
    std::vector<double>  UserPercent =  cpuInfo.GetThreadsUtilisation(CpuInfoDatas[0]);

    std::cout << "PhysicalCores: " << CpuInfoDatas[0]._numPhysicalCores << std::endl;
    std::cout << "LogicalCores: " << CpuInfoDatas[0]._numLogicalCores << std::endl;
    std::cout << "CPU Max Clock Speed: " << CpuInfoDatas[0]._maxClockSpeed_MHz << "MHz" << std::endl;
    std::cout << "CPU Regular Clock Speed: " << CpuInfoDatas[0]._regularClockSpeed_MHz << "MHz" << std::endl;
   

    assert((ClockSpeed.size() == UserPercent.size()));
    for (size_t thread_id = 0; thread_id != ClockSpeed.size(); ++thread_id) {
        std::cout<<"thread_id: "<<thread_id<<" ClockSpeed: "<<ClockSpeed[thread_id]<<" UserPercent:"<<UserPercent[thread_id]<<std::endl;
    }

    std::string flags;
    for (auto& Feature : CpuInfoDatas[0]._Features)
    {
        flags.append(Feature + " ");
    }

    std::cout << "CPU Flags : " << flags << std::endl;

    getchar();

    return 0;
    HRESULT hres;

    // 初始化 COM 库
    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres)) {
        std::cout << "Failed to initialize COM library. Error code = 0x"
            << std::hex << hres << std::endl;
        return 1;
    }

    // 设置 COM 安全性
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

    if (FAILED(hres)) {
        std::cout << "Failed to initialize security. Error code = 0x"
            << std::hex << hres << std::endl;
        CoUninitialize();
        return 1;
    }

    // 初始化 WMI
    IWbemLocator* pLoc = NULL;

    hres = CoCreateInstance(
        CLSID_WbemLocator,
        0,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator, (LPVOID*)&pLoc);

    if (FAILED(hres)) {
        std::cout << "Failed to create IWbemLocator object. "
            << "Err code = 0x" << std::hex << hres << std::endl;
        CoUninitialize();
        return 1;
    }

    IWbemServices* pSvc = NULL;

    // 连接到 WMI 上的根 \CIMV2
    hres = pLoc->ConnectServer(
        _bstr_t(L"ROOT\\CIMV2"),
        NULL,
        NULL,
        0,
        NULL,
        0,
        0,
        &pSvc);

    if (FAILED(hres)) {
        std::cout << "Could not connect. Error code = 0x"
            << std::hex << hres << std::endl;
        pLoc->Release();
        CoUninitialize();
        return 1;
    }

    // 设置 WMI 连接的安全性
    hres = CoSetProxyBlanket(
        pSvc,
        RPC_C_AUTHN_WINNT,
        RPC_C_AUTHZ_NONE,
        NULL,
        RPC_C_AUTHN_LEVEL_CALL,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE);

    if (FAILED(hres)) {
        std::cout << "Could not set proxy blanket. Error code = 0x"
            << std::hex << hres << std::endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return 1;
    }

    // 使用 WMI 查询计算机系统的处理器数量
    IEnumWbemClassObject* pEnumerator = NULL;
    hres = pSvc->ExecQuery(
        bstr_t("WQL"),
        bstr_t("SELECT * FROM Win32_ComputerSystem"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator);

    if (FAILED(hres)) {
        std::cout << "Query for computer system failed. "
            << "Error code = 0x"
            << std::hex << hres << std::endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return 1;
    }

    IWbemClassObject* pclsObj = NULL;
    ULONG uReturn = 0;

    while (pEnumerator) {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1,
            &pclsObj, &uReturn);

        if (0 == uReturn) {
            break;
        }

        VARIANT vtNumProcessors;
        VariantInit(&vtNumProcessors);

        // 获取 NumberOfProcessors 属性
        hr = pclsObj->Get(L"NumberOfProcessors", 0, &vtNumProcessors, 0, 0);

        if (SUCCEEDED(hr)) {
            std::wcout << "Number of Processors: " << vtNumProcessors.uintVal << std::endl;
        }
        else {
            std::cout << "Failed to get properties. Error code = 0x" << std::hex << hr << std::endl;
        }

        VariantClear(&vtNumProcessors);

        pclsObj->Release();
    }

    // 清理
    pSvc->Release();
    pLoc->Release();
    pEnumerator->Release();
    CoUninitialize();

    return 0;
}
