//
// Created by xd on 4/24/2025.
//

#include "GetAndroidInfo.h"

#include <fstream>
#include <string>


// 获取电池电量百分比（0 - 100）
int GetAndroidInfo::GetBatteryLevel(JNIEnv* Env, jobject Context)
{
    jclass ContextClass = Env->GetObjectClass(Context);

    jclass IntentFilterClass = Env->FindClass("android/content/IntentFilter");
    jmethodID IntentFilterCtor = Env->GetMethodID(IntentFilterClass, "<init>", "(Ljava/lang/String;)V");
    jstring Action = Env->NewStringUTF("android.intent.action.BATTERY_CHANGED");
    jobject IntentFilter = Env->NewObject(IntentFilterClass, IntentFilterCtor, Action);

    jmethodID RegisterReceiver = Env->GetMethodID(ContextClass, "registerReceiver",
                                                  "(Landroid/content/BroadcastReceiver;Landroid/content/IntentFilter;)Landroid/content/Intent;");
    jobject BatteryIntent = Env->CallObjectMethod(Context, RegisterReceiver, nullptr, IntentFilter);

    jclass IntentClass = Env->GetObjectClass(BatteryIntent);
    jmethodID GetIntExtra = Env->GetMethodID(IntentClass, "getIntExtra", "(Ljava/lang/String;I)I");
    jstring LevelKey = Env->NewStringUTF("level");
    jint Level = Env->CallIntMethod(BatteryIntent, GetIntExtra, LevelKey, -1);

    return Level;  // 百分比值
}

// 获取电池温度（单位：摄氏度）
float GetAndroidInfo::GetBatteryTemperature(JNIEnv* Env, jobject Context)
{
    jclass ContextClass = Env->GetObjectClass(Context);

    jclass IntentFilterClass = Env->FindClass("android/content/IntentFilter");
    jmethodID IntentFilterCtor = Env->GetMethodID(IntentFilterClass, "<init>", "(Ljava/lang/String;)V");
    jstring Action = Env->NewStringUTF("android.intent.action.BATTERY_CHANGED");
    jobject IntentFilter = Env->NewObject(IntentFilterClass, IntentFilterCtor, Action);

    jmethodID RegisterReceiver = Env->GetMethodID(ContextClass, "registerReceiver",
                                                  "(Landroid/content/BroadcastReceiver;Landroid/content/IntentFilter;)Landroid/content/Intent;");
    jobject BatteryIntent = Env->CallObjectMethod(Context, RegisterReceiver, nullptr, IntentFilter);

    jclass IntentClass = Env->GetObjectClass(BatteryIntent);
    jmethodID GetIntExtra = Env->GetMethodID(IntentClass, "getIntExtra", "(Ljava/lang/String;I)I");
    jstring TempKey = Env->NewStringUTF("temperature");
    jint TempRaw = Env->CallIntMethod(BatteryIntent, GetIntExtra, TempKey, -1);

    return TempRaw / 10.0f;  // 转换为摄氏度
}