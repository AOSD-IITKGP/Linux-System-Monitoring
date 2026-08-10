#include "proc_info.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include <dirent.h>
#include <algorithm>
#include <unordered_map>
#include <thread>
#include <chrono>

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


// NOTE (HADWIK): Please implement this function, I guess u might need few helper functions as well
/*
std::vector<ProcessInfo> get_top_cpu(int count) {
    There are some edge cases to be handled, what if a process dies in between?, is this an issue>
}
*/