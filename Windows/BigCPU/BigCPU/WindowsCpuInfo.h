//By Navy Zhang 710605420@qq.com 
#pragma once
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
#include <vector>
#include <cstdlib>
#include <clocale>



inline std::string wstring_to_std_string(const std::wstring& ws) {
    std::string str_locale = std::setlocale(LC_ALL, "");
    const wchar_t* wch_src = ws.c_str();

#ifdef _MSC_VER
    size_t n_dest_size;
    wcstombs_s(&n_dest_size, nullptr, 0, wch_src, 0);
    n_dest_size++;  // Increase by one for null terminator

    char* ch_dest = new char[n_dest_size];
    memset(ch_dest, 0, n_dest_size);

    size_t n_convert_size;
    wcstombs_s(&n_convert_size, ch_dest, n_dest_size, wch_src,
        n_dest_size - 1);  // subtract one to ignore null terminator

    std::string result_text = ch_dest;
    delete[] ch_dest;
#else
    size_t n_dest_size = std::wcstombs(NULL, wch_src, 0) + 1;
    char* ch_dest = new char[n_dest_size];
    std::memset(ch_dest, 0, n_dest_size);
    std::wcstombs(ch_dest, wch_src, n_dest_size);
    std::string result_text = ch_dest;
    delete[] ch_dest;
#endif

    std::setlocale(LC_ALL, str_locale.c_str());
    return result_text;
}

class CpuInfoData
{
    friend std::vector<CpuInfoData> GetAllCpus();
public:
    int _processor{ -1 };
    std::string _BogoMIPS;
    std::vector<std::string> _Features;
    std::string _Manufacturer;
    std::string _CPU_Implementer;
    std::string CPU_Architecture;
    std::string CPU_Variant;
    std::string CPU_Part;
    std::string CPU_Revision;

    std::string _modelName;
    int _numPhysicalCores;
    int _numLogicalCores;
    unsigned int _maxClockSpeed_MHz;
    unsigned int _regularClockSpeed_MHz;


public:
    //     int Processor() const;
    //     const std::string& BogoMIPS() const;
    //     const std::string& Features() const;
    //     int64_t GetCurrentClockSpeed() const;
    //     int64_t GetMaxClockSpeed_MHz()const;
    //     int64_t GetRegularClockSpeed_MHz()const;
    //     int64_t GetMinClockSpeed_MHz()const;



};

class WindowsCpuInfo
{

public:
    WindowsCpuInfo(const wchar_t* WMI_Namespace = L"ROOT\\CIMV2");
    ~WindowsCpuInfo();


public:
    std::vector<CpuInfoData> GetAllCpus();
    std::vector<int64_t> GetCurrentClockSpeed_MHz(CpuInfoData& CpuInfo) ;
    std::vector<double> GetThreadsUtilisation(CpuInfoData& CpuInfo);

private:
    HRESULT Hres;

    IWbemServices* pSvc;
    IWbemLocator* pLoc;

    bool ExecQueryValue(const char* strQueryLanguage, const char* strQuery, IEnumWbemClassObject** pEnumerator);



};

