#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <vector>
#include "cpu_info.hpp"
#include "mem_info.hpp"
#include "stats_info.hpp"
#include "proc_info.hpp"

// This will match the format mentioned in the assignment output
void display_all(const CpuInfo& cpu,
                 const MemoryInfo& mem,
                 const StatsInfo& stats,
                 const std::vector<ProcessInfo>& processes,
                 const std::vector<ProcessInfo>& top_mem,
                 const std::vector<ProcessInfo>& top_cpu);

#endif