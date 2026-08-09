#include "mem_info.hpp"
#include <fstream>
#include <string>
#include <sstream>

MemoryInfo get_memory_info() {
    MemoryInfo info = {0, 0, 0, 0.0};

    std::ifstream file("/proc/meminfo");
    if (!file.is_open()) {
        return info;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key;
        long value;

        /*  Sample output to parse:
            MemTotal:        7825744 kB
            MemFree:         7143208 kB
            MemAvailable:    7150484 kB
            Buffers:           24956 kB
            Cached:           193484 kB
            ...
        */

        iss >> key >> value;   

        if (key == "MemTotal:") {
            info.total_kb = value;
        } else if (key == "MemFree:") {
            info.free_kb = value;
        } else if (key == "MemAvailable:") {
            info.available_kb = value;
        }
    }

    file.close();

    // compute usage percentage
    if (info.total_kb > 0) {
        info.usage_percent = (double)(info.total_kb - info.available_kb) / info.total_kb * 100.0;
    }

    return info;
}