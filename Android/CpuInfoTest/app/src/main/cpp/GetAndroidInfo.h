//
// Created by xd on 4/24/2025.
//

#ifndef CPUINFOTEST_GETANDROIDINFO_H
#define CPUINFOTEST_GETANDROIDINFO_H
#include <jni.h>
#include <sched.h>  // for sched_getcpu
#include <stdio.h>

class GetAndroidInfo {
public:
    static int GetBatteryLevel(JNIEnv* Env, jobject Context);
    static float GetBatteryTemperature(JNIEnv* Env, jobject Context);

    static int GetCurrentCPU()
    {
        return sched_getcpu();
    }
};


#endif //CPUINFOTEST_GETANDROIDINFO_H
