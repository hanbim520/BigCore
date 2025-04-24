//
// Created by xd on 4/24/2025.
//

#ifndef CPUINFOTEST_GETANDROIDINFO_H
#define CPUINFOTEST_GETANDROIDINFO_H
#include <jni.h>

class GetAndroidInfo {
public:
    static int GetBatteryLevel(JNIEnv* Env, jobject Context);
    static float GetBatteryTemperature(JNIEnv* Env, jobject Context);
};


#endif //CPUINFOTEST_GETANDROIDINFO_H
