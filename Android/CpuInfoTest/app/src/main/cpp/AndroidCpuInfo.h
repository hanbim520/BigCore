//
// Created by xd on 7/23/2024.
//

#ifndef CPUINFOTEST_ANDROIDCPUINFO_H
#define CPUINFOTEST_ANDROIDCPUINFO_H
#include <string>
#include <vector>
#include <fstream>

static int64_t GetSpecsByFilePath(const std::string& path) {
    std::string line;
    std::ifstream stream(path);

    if (!stream) {
        return -1;
    }

    getline(stream, line);
    stream.close();

    try {
        return static_cast<int64_t>(std::stoll(line));
    } catch (std::invalid_argument& e) {
        return -1;
    }
}

static inline std::vector<std::string> split(const std::string& input, const std::string& delimiter) {
    std::vector<std::string> result;
    size_t shift = 0;
    while (true) {
        size_t match = input.find(delimiter, shift);
        result.emplace_back(input.substr(shift, match - shift));
        if (match == std::string::npos) {
            break;
        }
        shift = match + delimiter.size();
    }
    return result;
}
static inline void strip(std::string& input) {
    if (input.empty()) {
        return;
    }
    // optimization for input size == 1
    if (input.size() == 1) {
        if (input[0] == ' ' || input[0] == '\t' || input[0] == '\n') {
            input = "";
            return;
        } else {
            return;
        }
    }
    size_t start_index = 0;
    while (true) {
        char c = input[start_index];
        if (c != ' ' && c != '\t' && c != '\n') {
            break;
        }
        start_index++;
    }
    size_t end_index = input.size() - 1;
    while (true) {
        char c = input[end_index];
        if (c != ' ' && c != '\t' && c != '\n') {
            break;
        }
        end_index--;
    }
    if (end_index < start_index) {
        input.assign("");
        return;
    }
    input.assign(input.begin() + start_index, input.begin() + end_index + 1);
}

class CpuInfoData
{
    friend std::vector<CpuInfoData> GetAllCpus();
public:
    int _processor{-1};
    std::string _BogoMIPS;
    std::string _Features;
    std::string _CPU_Implementer;
    std::string CPU_Architecture;
    std::string CPU_Variant;
    std::string CPU_Part;
    std::string CPU_Revision;


public:
    int Processor() const;
    const std::string& BogoMIPS() const;
    const std::string& Features() const;
    int64_t GetCurrentClockSpeed() const;
    int64_t GetMaxClockSpeed_MHz()const;
    int64_t GetRegularClockSpeed_MHz()const;
    int64_t GetMinClockSpeed_MHz()const;
};

class AndroidCpuInfo {
public:
     std::vector<CpuInfoData> GetAllCpus();


};


#endif //CPUINFOTEST_ANDROIDCPUINFO_H
