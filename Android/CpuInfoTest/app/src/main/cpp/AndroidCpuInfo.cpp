//By Navy Zhang 710605420@qq.com 
//
// Created by xd on 7/23/2024.
//

#include "AndroidCpuInfo.h"
#include <unistd.h>

#include <chrono>
#include <cmath>
#include <fstream>
#include <map>
#include <string>
#include <thread>
#include <vector>
#include <sstream>

// _____________________________________________________________________________________________________________________
int CpuInfoData::Processor() const { return _processor; }

// _____________________________________________________________________________________________________________________
const std::string& CpuInfoData::BogoMIPS() const { return _BogoMIPS; }

// _____________________________________________________________________________________________________________________
const std::string& CpuInfoData::Features() const { return _Features; }
// _____________________________________________________________________________________________________________________
int64_t CpuInfoData::GetCurrentClockSpeed() const
{
    int64_t frequency_Hz = GetSpecsByFilePath(
            "/sys/devices/system/cpu/cpu" + std::to_string(_processor) +
            "/cpufreq/scaling_cur_freq");
    return frequency_Hz / 1000;
}
// _____________________________________________________________________________________________________________________
int64_t CpuInfoData::GetMaxClockSpeed_MHz() const
{
    int64_t Hz = GetSpecsByFilePath("/sys/devices/system/cpu/cpu" + std::to_string(_processor) +
                                                    "/cpufreq/scaling_max_freq");
    if (Hz > -1) {
        return Hz / 1000;
    }

    return -1;
}

// _____________________________________________________________________________________________________________________
int64_t CpuInfoData::GetRegularClockSpeed_MHz()const
{
    int64_t Hz = GetSpecsByFilePath("/sys/devices/system/cpu/cpu" + std::to_string(_processor) +
                                                    "/cpufreq/base_frequency");
    if (Hz > -1) {
        return Hz / 1000;
    }

    return -1;
}

// _____________________________________________________________________________________________________________________
int64_t CpuInfoData::GetMinClockSpeed_MHz()const
{
    int64_t Hz = GetSpecsByFilePath("/sys/devices/system/cpu/cpu" + std::to_string(_processor) +
                                                    "/cpufreq/scaling_min_freq");
    if (Hz > -1) {
        return Hz / 1000;
    }

    return -1;
}



std::vector<CpuInfoData> AndroidCpuInfo::GetAllCpus()
{

    std::vector<CpuInfoData> cpus;

    std::ifstream cpuinfo("/proc/cpuinfo");
    if (!cpuinfo.is_open()) {
        return {};
    }
    std::string file((std::istreambuf_iterator<char>(cpuinfo)), (std::istreambuf_iterator<char>()));
    cpuinfo.close();
    auto cpu_blocks_string = split(file, "\n\n");
    std::map<const std::string, const std::string> cpu_block;
    bool next_add = false;
    for ( auto& block : cpu_blocks_string)
    {
        strip(block);
        if (block == "")
        {
            continue;
        }
        CpuInfoData cpu;
        auto lines = split(block, "\n");
        for (auto& line : lines) {
            strip(line);
            if (line == "")
            {
                continue;
            }
            auto line_pairs = split(line, ":");
            if (line_pairs.size() < 2) {
                continue;
            }
            auto name = line_pairs[0];
            auto value = line_pairs[1];
            strip(name);
            strip(value);
            if (name == "processor") {
                cpu._processor = std::stoi(value);
            }
            else if (name == "BogoMIPS") {
                cpu._BogoMIPS = value;
            } else if (name == "Features") {
                cpu._Features = value;
            } else if (name == "CPU implementer") {
                cpu._CPU_Implementer = value;
            } else if (name == "CPU architecture") {
                cpu.CPU_Architecture = value;
            }
            else if (name == "CPU variant") {
                cpu.CPU_Variant = value;
            }
            else if (name == "CPU part") {
                cpu.CPU_Part = value;
            }
            else if (name == "CPU revision") {
                cpu.CPU_Revision = value;
            }
        }
        cpus.push_back(std::move(cpu));
    }

    return  cpus;
}