#include <jni.h>
#include <string>
#include <iostream>

#include "AndroidCpuInfo.h"
#include "AndroidBuf.h"


extern "C" JNIEXPORT jstring JNICALL
Java_com_example_cpuinfotest_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    std::cout.rdbuf(new AndroidBuf());

    AndroidCpuInfo androidCpuInfo;
    std::vector<CpuInfoData>  CPUInfos = androidCpuInfo.GetAllCpus();
    for (const auto& cpu : CPUInfos)
    {
        auto threads_speed = cpu.GetMaxClockSpeed_MHz();
       // assert((threads_utility.size() == threads_speed.size()));

        std::cout << "cpu id: " << cpu._processor << " Max ClockSpeed: " <<  threads_speed  << "MHz"<<std::endl;
    }
    return env->NewStringUTF(hello.c_str());
}