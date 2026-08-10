#include "display.hpp"
#include <iostream>
#include <iomanip>

// Entire format inspired from Assignment
// Note (Yesheeth): Please review this part as well
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
    std::cout << "CPU Usage        : " << std::setprecision(1)
              << stats.cpu_usage << "%" << std::endl;

    int total_seconds = (int)stats.uptime_seconds;
    int days    = total_seconds / 86400;
    int hours   = (total_seconds % 86400) / 3600;
    int minutes = (total_seconds % 3600) / 60;
    std::cout << "System Uptime    : " << days << " Days "
              << hours << " Hours " << minutes << " Minutes" << std::endl;

    std::cout << "Load Average     : " << std::setprecision(2)
              << stats.load_1 << " (1 min)  "
              << stats.load_5 << " (5 min)  "
              << stats.load_15 << " (15 min)" << std::endl;
    std::cout << "-------------------------------------------------------------" << std::endl;

    // Task 3: Running Processes
    std::cout << "[3] RUNNING PROCESSES" << std::endl;
    std::cout << "-------------------------------------------------------------" << std::endl;
    std::cout << "PID\tPPID\tSTATE\tTHREADS\tVmSize(MB)\tNAME" << std::endl;
    for (const auto& p : processes) {
        std::cout << p.pid << "\t"
                  << p.ppid << "\t"
                  << p.state << "\t"
                  << p.threads << "\t"
                  << p.vm_size_kb / 1024 << "\t\t"
                  << p.name << std::endl;
    }
    std::cout << "-------------------------------------------------------------" << std::endl;

    // Task 4: Top 5 Memory Consumers
    std::cout << "[4] TOP 5 MEMORY CONSUMERS" << std::endl;
    std::cout << "-------------------------------------------------------------" << std::endl;
    std::cout << "Rank\tPID\tMemory(MB)\tProcess" << std::endl;
    for (int i = 0; i < (int)top_mem.size(); i++) {
        std::cout << i + 1 << "\t"
                  << top_mem[i].pid << "\t"
                  << top_mem[i].vm_rss_kb / 1024 << "\t\t"
                  << top_mem[i].name << std::endl;
    }
    std::cout << "-------------------------------------------------------------" << std::endl;

    // Task 4: Top 5 CPU Consumers
    std::cout << "TOP 5 CPU CONSUMERS" << std::endl;
    std::cout << "-------------------------------------------------------------" << std::endl;
    std::cout << "Rank\tPID\tCPU(%)\t\tProcess" << std::endl;
    for (int i = 0; i < (int)top_cpu.size(); i++) {
        std::cout << i + 1 << "\t"
                  << top_cpu[i].pid << "\t"
                  << std::setprecision(1)
                  << top_cpu[i].cpu_usage << "\t\t"
                  << top_cpu[i].name << std::endl;
    }
    std::cout << "==============================================================" << std::endl;
}