#include <iostream>
#include <cstdlib>
#include <cstring>

// uncomment these as you implement each module
#include "cpu_info.hpp"
// #include "memory_info.hpp"
// #include "stats_info.hpp"
// #include "process_info.hpp"
// #include "display.hpp"

int main(int argc, char* argv[]) {

    // params for live mode
    bool live_mode = false;
    int interval = 0;  // T value

    // we need to check if "-T" flag is present, if yes then we are in live mode
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-T") == 0) {
            live_mode = true;
        }
    }

    // parse the T value argv and store it in interval
    if (live_mode) {
        if (argc < 3) {
            std::cerr << "Usage: ./proc_monitor -T <seconds>" << std::endl;
            return 1;
        }

        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-T") == 0 && i + 1 < argc) {
                interval = std::atoi(argv[i + 1]);
                break;
            }
        }

        if (interval <= 0) {
            std::cerr << "Error: interval must be a positive integer" << std::endl;
            return 1;
        }
    }

    if (live_mode) {
        std::cout << "Live mode with interval: " << interval << "s" << std::endl;
        // TODO: implement live mode loop
    } else {
        std::cout << "Static mode" << std::endl;

        // Task 1: CPU and Memory Information
        CpuInfo cpu = get_cpu_info();
        std::cout << "CPU Model      : " << cpu.model << std::endl;
        std::cout << "Logical CPUs   : " << cpu.logical_cpus << std::endl;
        // TODO: memory_info

        // Task 2: CPU Utilization and System Statistics
        // TODO: stats_info

        // Task 3: Process Information
        // TODO: process_info (list all)

        // Task 4: Top Resource-Consuming Processes
        // TODO: process_info (top 5)
    }

    return 0;
}