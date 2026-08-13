#include <iostream>
#include <cstdlib>
#include <cstring>
#include <csignal>
#include <thread>
#include <chrono>

#include "cpu_info.hpp"
#include "mem_info.hpp"
#include "stats_info.hpp"
#include "proc_info.hpp"
#include "display.hpp"

volatile sig_atomic_t running = 1;

void signal_handler(int) {
    running = 0;
}

int main(int argc, char* argv[]) {

    bool live_mode = false;
    int interval = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-T") == 0) {
            live_mode = true;
        }
    }

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
        if(interval <= 2) { // as the internal sleeps for 2 seconds, we need to ensure that the interval is at least 2 seconds
            std::cerr << "Warning: Interval less than or equal to 2 seconds may not provide accurate CPU usage data." << std::endl;
            return 1;
        }
    }

    if (live_mode) {
        signal(SIGINT, signal_handler);

        while (running) {
            system("clear");

            CpuInfo cpu = get_cpu_info();
            MemoryInfo mem = get_memory_info();
            StatsInfo stats = get_stats_info();
            std::vector<ProcessInfo> processes = get_all_processes();
            std::vector<ProcessInfo> top_mem = get_top_memory(processes, 5);
            std::vector<ProcessInfo> top_cpu = get_top_cpu(processes, 5);

            display_all(cpu, mem, stats, processes, top_mem, top_cpu);

            // stats_info already sleeps 1 second and proc_info sleeps 1 second, so sleep remaining T-2
        
            if (interval > 2 && running) {
                std::this_thread::sleep_for(std::chrono::seconds(interval - 2));
            }
        }

        std::cout << "\nMonitor stopped." << std::endl;

    } else {
        CpuInfo cpu = get_cpu_info();
        MemoryInfo mem = get_memory_info();
        StatsInfo stats = get_stats_info();
        std::vector<ProcessInfo> processes = get_all_processes();
        std::vector<ProcessInfo> top_mem = get_top_memory(processes);
        std::vector<ProcessInfo> top_cpu = get_top_cpu(processes);

        display_all(cpu, mem, stats, processes, top_mem, top_cpu);
    }

    return 0;
}