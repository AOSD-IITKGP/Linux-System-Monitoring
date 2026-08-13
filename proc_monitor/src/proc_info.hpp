#ifndef PROCESS_INFO_HPP
#define PROCESS_INFO_HPP

#include <string>
#include <vector>

struct ProcessInfo {
    int pid;
    std::string name;
    int ppid;
    char state;         // can be R, S, D, Z, T
    int threads;
    long vm_size_kb;    // VmSize: total virtual memory in KB (0 for kernel threads)
    long vm_rss_kb;     // VmRSS : resident set size in KB
    double cpu_usage;   // CPU usage percentage
};

// get info for all processes
std::vector<ProcessInfo> get_all_processes();

// top 5 by memory 
std::vector<ProcessInfo> get_top_memory(const std::vector<ProcessInfo>& processes, int count = 5);

// top 5 by CPU
std::vector<ProcessInfo> get_top_cpu(const std::vector<ProcessInfo>& processes, int count = 5);

#endif