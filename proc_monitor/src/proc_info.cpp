#include "proc_info.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include <dirent.h>
#include <algorithm>
#include <unordered_map>
#include <thread>
#include <chrono>
#include <unistd.h>

// parses /proc/[pid]/status and fills it into a ProcessInfo struct
static bool read_process_status(int pid, ProcessInfo& proc) {
    std::string path = "/proc/" + std::to_string(pid) + "/status";
    std::ifstream file(path);

    if (!file.is_open()) {
        return false;
    }

    // NOTE (To Yesheeth): These are default values, see if there any issues.
    proc.pid = pid;
    proc.ppid = 0;
    proc.state = '?';
    proc.threads = 0;
    proc.vm_size_kb = 0;
    proc.vm_rss_kb = 0;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key;
        iss >> key;

        if (key == "Name:") {
            iss >> proc.name;
        } else if (key == "Pid:") {
            iss >> proc.pid;
        } else if (key == "PPid:") {
            iss >> proc.ppid;
        } else if (key == "State:") {
            iss >> proc.state;
        } else if (key == "Threads:") {
            iss >> proc.threads;
        } else if (key == "VmSize:") {
            iss >> proc.vm_size_kb;
        } else if (key == "VmRSS:") {
            iss >> proc.vm_rss_kb;
        }
    }

    file.close();
    return true;
}

std::vector<ProcessInfo> get_all_processes() {
    std::vector<ProcessInfo> processes;

    DIR* dir = opendir("/proc");
    if (dir == nullptr) {
        return processes;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::string name = entry->d_name;

        // PIDs are the directories that are numbers like 1462/
        if (!std::all_of(name.begin(), name.end(), ::isdigit)) {
            continue;
        }

        int pid = std::stoi(name);
        ProcessInfo proc;

        if (read_process_status(pid, proc)) {
            processes.push_back(proc);
        }
        // NOTE (YESHEETH): Handle the case when a process dies during this function run.
    }

    closedir(dir);

    // sorting by PID
    std::sort(processes.begin(), processes.end(), 
            [](const ProcessInfo& a, const ProcessInfo& b) {
                return a.pid < b.pid;
            });

    return processes;
}


std::vector<ProcessInfo> get_top_memory(const std::vector<ProcessInfo>& processes, int count) {
    // copy and sort by VmRSS descending
    std::vector<ProcessInfo> sorted = processes;

    std::sort(sorted.begin(), sorted.end(),
                [](const ProcessInfo& a, const ProcessInfo& b) {
                    return a.vm_rss_kb > b.vm_rss_kb;
                });

    // return top 'count' (default = 5)
    if ((int)sorted.size() > count) {
        sorted.resize(count);
    }

    return sorted;
}

// Reads utime+stime (in clock ticks) for one process from /proc/[pid]/stat.
// Returns false if the process no longer exists (died between samples) or
// the file couldn't be parsed - caller is responsible for skipping it.
static bool read_proc_cpu_ticks(int pid, long& total_ticks) {
    std::string path = "/proc/" + std::to_string(pid) + "/stat";
    std::ifstream file(path);
    if (!file.is_open()) {
        return false;
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // comm (field 2) is in parentheses and can contain spaces/parens itself,
    std::size_t close_paren = content.rfind(')');
    if (close_paren == std::string::npos) {
        return false;
    }

    std::istringstream iss(content.substr(close_paren + 1));

    // 3 state, 4 ppid, 5 pgrp, 6 session, 7 tty_nr, 8 tpgid, 9 flags, 10 minflt, 11 cminflt, 12 majflt, 13 cmajflt, 14 utime, 15 stime
    std::string state;
    iss >> state; // field 3

    long field, utime = 0, stime = 0;
    for (int field_num = 4; field_num <= 15; field_num++) {
        if (!(iss >> field)) {
            return false; // malformed/short line
        }
        if (field_num == 14) utime = field;
        if (field_num == 15) stime = field;
    }

    total_ticks = utime + stime;
    return true;
}

std::vector<ProcessInfo> get_top_cpu(const std::vector<ProcessInfo>& processes, int count) {
    long ticks_per_sec = sysconf(_SC_CLK_TCK);
    if (ticks_per_sec <= 0) {
        ticks_per_sec = 100; // sane fallback, 100 Hz is the common default
    }

    // first sample
    std::unordered_map<int, long> before;
    for (const auto& p : processes) {
        long ticks;
        if (read_proc_cpu_ticks(p.pid, ticks)) {
            before[p.pid] = ticks;
        }
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // second sample + delta
    std::vector<ProcessInfo> result;
    for (const auto& p : processes) {
        long after_ticks;
        if (!read_proc_cpu_ticks(p.pid, after_ticks)) {
            continue; // process died between samples - skip it
        }
        auto it = before.find(p.pid);
        if (it == before.end()) {
            continue; // wasn't readable in the first sample either
        }

        long delta_ticks = after_ticks - it->second;
        if (delta_ticks < 0) delta_ticks = 0;

        ProcessInfo entry = p;
        entry.cpu_usage = (double)delta_ticks / ticks_per_sec * 100.0;
        result.push_back(entry);
    }

    std::sort(result.begin(), result.end(), [](const ProcessInfo& a, const ProcessInfo& b) {return a.cpu_usage > b.cpu_usage;});

    if ((int)result.size() > count) {
        result.resize(count);
    }

    return result;
}