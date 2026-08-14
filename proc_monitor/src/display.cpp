#include "display.hpp"
#include <iostream>
#include <iomanip>

// Entire format inspired from Assignment
// used setw to align columns and setprecision to format floating point numbers
void display_all(const CpuInfo& cpu,
                 const MemoryInfo& mem,
                 const StatsInfo& stats,
                 const std::vector<ProcessInfo>& processes,
                 const std::vector<ProcessInfo>& top_mem,
                 const std::vector<ProcessInfo>& top_cpu) {

    std::cout << "==============================================================" << std::endl;
    std::cout << " PROC MONITOR - SYSTEM INFORMATION" << std::endl;
    std::cout << "==============================================================" << std::endl;

    // Task 1: CPU & Memory
    std::cout << "[1] CPU & MEMORY INFORMATION" << std::endl;
    std::cout << "-------------------------------------------------------------" << std::endl;
    std::cout << "CPU Model        : " << cpu.model << std::endl;
    std::cout << "Logical CPUs     : " << cpu.logical_cpus << std::endl;
    std::cout << std::fixed;

    std::cout << "Total Memory     : " << std::setprecision(2)
              << mem.total_kb / (1024.0 * 1024.0) << " GB" << std::endl;

    std::cout << "Available Memory : " << std::setprecision(2)
              << mem.available_kb / (1024.0 * 1024.0) << " GB" << std::endl;

    std::cout << "Free Memory      : " << std::setprecision(2)
              << mem.free_kb / (1024.0 * 1024.0) << " GB" << std::endl;

    std::cout << "Memory Usage     : " << std::setprecision(1)
              << mem.usage_percent << "%" << std::endl;

    std::cout << "-------------------------------------------------------------" << std::endl;

    // Task 2: CPU Utilization & System Stats
    std::cout << "[2] CPU UTILIZATION & SYSTEM STATISTICS" << std::endl;
    std::cout << "-------------------------------------------------------------" << std::endl;

    std::cout << "CPU Usage        : "
              << std::setprecision(1)
              << stats.cpu_usage << "%" << std::endl;

    int total_seconds = (int)stats.uptime_seconds;

    int days    = total_seconds / 86400;
    int hours   = (total_seconds % 86400) / 3600;
    int minutes = (total_seconds % 3600) / 60;

    std::cout << "System Uptime    : "
              << days << " Days "
              << hours << " Hours "
              << minutes << " Minutes" << std::endl;

    // Matches sample output
    std::cout << "Load Average" << std::endl;
    std::cout << "  1 min : " << std::setprecision(2)
              << stats.load_1 << std::endl;
    std::cout << "  5 min : " << std::setprecision(2)
              << stats.load_5 << std::endl;
    std::cout << " 15 min : " << std::setprecision(2)
              << stats.load_15 << std::endl;

    std::cout << "-------------------------------------------------------------" << std::endl;

    // Task 3: Running Processes
    std::cout << "[3] RUNNING PROCESSES" << std::endl;
    std::cout << "-------------------------------------------------------------" << std::endl;

    std::cout << std::left
              << std::setw(8)  << "PID"
              << std::setw(8)  << "PPID"
              << std::setw(8)  << "STATE"
              << std::setw(10) << "THREADS"
              << std::setw(14) << "VmSize(MB)"
              << "NAME" << std::endl;

    for (const auto& p : processes) {
        std::cout << std::left
                  << std::setw(8)  << p.pid
                  << std::setw(8)  << p.ppid
                  << std::setw(8)  << p.state
                  << std::setw(10) << p.threads
                  << std::setw(14) << p.vm_size_kb / 1024
                  << p.name << std::endl;
    }

    std::cout << "-------------------------------------------------------------" << std::endl;

    // Task 4: Top 5 Memory Consumers and CPU Consumers
    std::cout << "[4] TOP 5 MEMORY CONSUMERS" << std::endl;
    std::cout << "-------------------------------------------------------------" << std::endl;

    std::cout << std::left
              << std::setw(8)  << "Rank"
              << std::setw(8)  << "PID"
              << std::setw(14) << "Memory(MB)"
              << "Process" << std::endl;

    for (size_t i = 0; i < top_mem.size(); i++) {
        std::cout << std::left
                  << std::setw(8)  << i + 1
                  << std::setw(8)  << top_mem[i].pid
                  << std::setw(14) << top_mem[i].vm_rss_kb / 1024
                  << top_mem[i].name << std::endl;
    }

    std::cout << "-------------------------------------------------------------" << std::endl;

    std::cout << "TOP 5 CPU CONSUMERS" << std::endl;
    std::cout << "-------------------------------------------------------------" << std::endl;

    std::cout << std::left
              << std::setw(8)  << "Rank"
              << std::setw(8)  << "PID"
              << std::setw(10) << "CPU(%)"
              << "Process" << std::endl;

    for (size_t i = 0; i < top_cpu.size(); i++) {
        std::cout << std::left
                  << std::setw(8)  << i + 1
                  << std::setw(8)  << top_cpu[i].pid
                  << std::setw(10) << std::setprecision(1)
                  << top_cpu[i].cpu_usage
                  << top_cpu[i].name << std::endl;
    }

    std::cout << "==============================================================" << std::endl;
}