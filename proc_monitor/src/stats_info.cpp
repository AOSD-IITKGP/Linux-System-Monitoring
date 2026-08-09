#include "stats_info.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include <thread>
#include <chrono>

static void read_cpu_times(long times[10]) {
    std::ifstream file("/proc/stat");
    std::string line;

    /*
        cpu  1712 0 3901 17193629 859 0 305 0 0 0 (cpu is aggregate of all cpu{x})
        cpu0 31 0 586 1073992 18 0 187 0 0 0
        cpu1 68 0 57 1074913 52 0 37 0 0 0
        cpu2 219 0 153 1074486 149 0 34 0 0 0
        cpu3 112 0 117 1074716 89 0 32 0 0 0
        cpu4 108 0 203 1074650 24 0 5 0 0 0
        cpu5 82 0 101 1074925 27 0 0 0 0 0

        The fields: (there are a total of ten fields, all are counters in clock ticks)
        cpu{x}
        │
        ├── user
        ├── nice
        ├── system
        ├── idle
        ├── iowait
        ├── irq
        ├── softirq
        ├── steal
        ├── guest
        └── guest_nice
    */

    if (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string cpu_label;
        iss >> cpu_label; // skip "cpu"

        for (int i = 0; i < 10; i++) {
            iss >> times[i];
        }
    }
    file.close();
}

static double compute_cpu_usage(long before[10], long after[10]) {
    // idle = idle + iowait (indices 3 and 4)
    long idle_before = before[3] + before[4];
    long idle_after  = after[3] + after[4];

    long total_before = 0, total_after = 0;
    for (int i = 0; i < 10; i++) {
        total_before += before[i];
        total_after  += after[i];
    }

    long total_delta = total_after - total_before;
    long idle_delta  = idle_after - idle_before;

    if (total_delta == 0) return 0.0;

    return (1.0 - (double)idle_delta / total_delta) * 100.0;
}

StatsInfo get_stats_info() {
    StatsInfo info = {0.0, 0.0, 0.0, 0.0, 0.0};

    // --- CPU usage: two samples, 1 second apart ---
    long times_before[10] = {0};
    long times_after[10]  = {0};

    read_cpu_times(times_before);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    read_cpu_times(times_after);

    info.cpu_usage = compute_cpu_usage(times_before, times_after);

    // --- Uptime ---
    /*
        13192.17 (system uptime) 210976.15 (cpu idle time)
    */
    std::ifstream uptime_file("/proc/uptime");
    if (uptime_file.is_open()) {
        uptime_file >> info.uptime_seconds;
        uptime_file.close();
    }

    // --- Load averages ---
    /*
         0.00   0.00   0.00   1/260 2273
        1 min  5 min  15 min
    */
    std::ifstream loadavg_file("/proc/loadavg");
    if (loadavg_file.is_open()) {
        loadavg_file >> info.load_1 >> info.load_5 >> info.load_15;
        loadavg_file.close();
    }

    return info;
}