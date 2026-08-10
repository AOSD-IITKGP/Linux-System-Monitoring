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
    long vm_size_kb;    // VmSize : Total virtual address space the process has mapped (0 if not present (kernel threads))
    long vm_rss_kb;     // VmRSS  : Portion of VmSize that is in RAM
    double cpu_usage;   
};

// get info for all processes
std::vector<ProcessInfo> get_all_processes();

// top 5 by memory 
std::vector<ProcessInfo> get_top_memory(const std::vector<ProcessInfo>& processes, int count = 5);

// top 5 by CPU
std::vector<ProcessInfo> get_top_cpu(const std::vector<ProcessInfo>& processes, int count = 5);

#endif