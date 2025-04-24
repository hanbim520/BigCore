//By Navy Zhang 710605420@qq.com 
#include <jni.h>
#include <string>
#include <iostream>

#include "AndroidCpuInfo.h"
#include "AndroidBuf.h"
#include "GetAndroidInfo.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_cpuinfotest_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject MainActivity     /* this */) {
    std::string hello = "Hello from C++";

    std::cout.rdbuf(new AndroidBuf());

    AndroidCpuInfo androidCpuInfo;
    std::vector<CpuInfoData>  CPUInfos = androidCpuInfo.GetAllCpus();
    for (const auto& cpu : CPUInfos)
    {
        auto max_speed = cpu.GetMaxClockSpeed_MHz();
        auto min_speed = cpu.GetMinClockSpeed_MHz();
        auto reg_speed = cpu.GetRegularClockSpeed_MHz();
        auto cur_speed = cpu.GetCurrentClockSpeed();
       // assert((threads_utility.size() == threads_speed.size()));

        std::cout << "cpu id: " << cpu._processor << " Max ClockSpeed: " <<  max_speed  << "MHz"<< " Min ClockSpeed: " <<  min_speed  << "MHz"<< " Regular ClockSpeed: " <<  reg_speed  << "MHz"<< " Current ClockSpeed: " <<  cur_speed  << "MHz"<< std::endl;
    }


    std::cout << "BatteryLevel: " << GetAndroidInfo::GetBatteryLevel(env,MainActivity) << " BatteryTemperature : " <<  GetAndroidInfo::GetBatteryTemperature(env,MainActivity) << "°C"<<std::endl;
    std::cout << "Work on cpu Core: " << GetAndroidInfo::GetCurrentCPU()<<std::endl;
    return env->NewStringUTF(hello.c_str());
}