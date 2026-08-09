#ifndef MEMORY_INFO_HPP
#define MEMORY_INFO_HPP

struct MemoryInfo {
    long total_kb;
    long free_kb;
    long available_kb;
    double usage_percent;
};

MemoryInfo get_memory_info();

#endif