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

    /* Sample output to parse
        processor       : 0   (In my PC there are 0 to 15 processor ids, i.e 16 processors)
        vendor_id       : AuthenticAMD
        cpu family      : 25
        model           : 80
        model name      : AMD Ryzen 7 5800HS with Radeon Graphics
        stepping        : 0
        microcode       : 0xffffffff
        cpu MHz         : 3193.917
        cache size      : 512 KB
        physical id     : 0
        siblings        : 16
        core id         : 0
        cpu cores       : 8
        ....
    */

    std::string line;
    while (std::getline(file, line)) {
        // count every "processor" line
        if (line.find("processor") == 0) {
            info.logical_cpus++;
        }

        // grab model name from first occurrence only
        if (line.find("model name") == 0 && info.model.empty()) {
            std::size_t pos = line.find(':');
            if (pos != std::string::npos) {
                info.model = line.substr(pos + 2); // skip ": "
            }
        }
    }

    file.close();
    return info;
}