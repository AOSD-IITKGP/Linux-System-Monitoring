#ifndef PROCESS_INFO_HPP
#define PROCESS_INFO_HPP

#include <string>
#include <vector>

struct ProcessInfo {
    int pid;
    std::string name;
    int ppid;
    char state;
    int threads;
    long vm_size_kb;    // VmSize, 0 if not present (kernel threads)
    long vm_rss_kb;     // VmRSS, needed for task 4
};

// get info for all processes
std::vector<ProcessInfo> get_all_processes();

#endif