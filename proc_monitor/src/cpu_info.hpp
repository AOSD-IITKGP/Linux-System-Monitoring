#ifndef CPU_INFO_HPP
#define CPU_INFO_HPP

#include <string>

struct CpuInfo {
    std::string model;
    int logical_cpus;
};

// reads /proc/cpuinfo and returns populated struct
CpuInfo get_cpu_info();

#endif