//By Navy Zhang 710605420@qq.com 
// #include <iostream>
// #include <vector>
// #include <windows.h>
// #include <powrprof.h>
// 
// #pragma comment(lib, "powrprof.lib")
// 
// #define STATUS_BUFFER_TOO_SMALL ((NTSTATUS)0xC0000023L)
// #define STATUS_SUCCESS ((NTSTATUS)0x00000000L)
// 
// typedef struct _PROCESSOR_POWER_INFORMATION {
//     ULONG Number;
//     ULONG MaxMhz;
//     ULONG CurrentMhz;
//     ULONG MhzLimit;
//     ULONG MaxIdleState;
//     ULONG CurrentIdleState;
// } PROCESSOR_POWER_INFORMATION, * PPROCESSOR_POWER_INFORMATION;
// // 获取处理器频率
// DWORD GetProcessorFrequency(DWORD processorIndex) {
//     DWORD frequency = 0;
//     ULONG bufferSize = 0;
//     NTSTATUS status = CallNtPowerInformation(ProcessorInformation, nullptr, 0, nullptr, bufferSize);
// 
//     if (status == STATUS_BUFFER_TOO_SMALL) {
//         std::vector<UCHAR> buffer(bufferSize);
//         status = CallNtPowerInformation(ProcessorInformation, nullptr, 0, buffer.data(), bufferSize);
// 
//         if (status == STATUS_SUCCESS) {
//             auto info = reinterpret_cast<PPROCESSOR_POWER_INFORMATION>(buffer.data());
//             frequency = info[processorIndex].MaxMhz;
//         }
//     }
// 
//     return frequency;
// }
// 
// // 查找大核
// int FindBigCore() {
//     DWORD length = 0;
//     GetLogicalProcessorInformationEx(RelationProcessorCore, nullptr, &length);
//     std::vector<BYTE> buffer(length);
// 
//     if (!GetLogicalProcessorInformationEx(RelationProcessorCore, reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(buffer.data()), &length)) {
//         std::cerr << "GetLogicalProcessorInformationEx failed with error: " << GetLastError() << std::endl;
//         return -1;
//     }
// 
//     PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX info = reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(buffer.data());
//     int bigCoreIndex = -1;
//     DWORD maxMhz = 0;
// 
//     for (DWORD i = 0; i < length; i += info->Size) {
//         if (info->Relationship == RelationProcessorCore) {
//             DWORD mhz = GetProcessorFrequency(i);
//             if (mhz > maxMhz) {
//                 maxMhz = mhz;
//                 bigCoreIndex = i;
//             }
//         }
//         info = reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(reinterpret_cast<BYTE*>(info) + info->Size);
//     }
// 
//     return bigCoreIndex;
// }
// 
// // 线程函数
// DWORD WINAPI ThreadFunction(LPVOID lpParam) {
//     std::cout << "Thread running on core: " << GetCurrentProcessorNumber() << std::endl;
//     return 0;
// }
// 
// int main() {
//     int bigCoreIndex = FindBigCore();
// 
//     if (bigCoreIndex == -1) {
//         std::cerr << "No big core found!" << std::endl;
//         return -1;
//     }
// 
//     std::cout << "Big core index: " << bigCoreIndex << std::endl;
// 
//     // 创建线程
//     HANDLE hThread = CreateThread(
//         nullptr,                // 默认安全属性
//         0,                      // 默认堆栈大小
//         ThreadFunction,         // 线程函数
//         nullptr,                // 线程函数参数
//         0,                      // 默认创建标志
//         nullptr                 // 接收线程ID
//     );
// 
//     if (hThread == nullptr) {
//         std::cerr << "CreateThread failed with error: " << GetLastError() << std::endl;
//         return -1;
//     }
// 
//     // 设置线程亲和性到大核
//     DWORD_PTR affinityMask = 1 << bigCoreIndex;
//     if (!SetThreadAffinityMask(hThread, affinityMask)) {
//         std::cerr << "SetThreadAffinityMask failed with error: " << GetLastError() << std::endl;
//         CloseHandle(hThread);
//         return -1;
//     }
// 
//     // 等待线程完成
//     WaitForSingleObject(hThread, INFINITE);
// 
//     // 关闭线程句柄
//     CloseHandle(hThread);
// 
//     return 0;
// }
