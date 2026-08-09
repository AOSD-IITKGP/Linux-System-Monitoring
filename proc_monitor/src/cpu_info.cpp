#include "cpu_info.hpp"
#include <fstream>
#include <string>

CpuInfo get_cpu_info() {
    CpuInfo info;
    info.logical_cpus = 0;

    std::ifstream file("/proc/cpuinfo");
    if (!file.is_open()) {
        info.model = "Unknown";
        return info;
    }

    std::string line;
    while (std::getline(file, line)) {
        // count every "processor" line
        if (line.find("processor") == 0) {
            info.logical_cpus++;
        }

        // grab model name from first occurrence only
        if (line.find("model name") == 0 && info.model.empty()) {
            // format is "model name\t: AMD Ryzen 7 ..."
            // find the ": " and take everything after it
            std::size_t pos = line.find(':');
            if (pos != std::string::npos) {
                info.model = line.substr(pos + 2); // skip ": "
            }
        }
    }

    file.close();
    return info;
}