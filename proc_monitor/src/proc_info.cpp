#include "proc_info.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include <dirent.h>
#include <algorithm>

// parse /proc/[pid]/status and fill a ProcessInfo struct
static bool read_process_status(int pid, ProcessInfo& proc) {
    std::string path = "/proc/" + std::to_string(pid) + "/status";
    std::ifstream file(path);

    if (!file.is_open()) {
        return false;
    }

    // set defaults
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

        // only numeric directories are PIDs
        if (!std::all_of(name.begin(), name.end(), ::isdigit)) {
            continue;
        }

        int pid = std::stoi(name);
        ProcessInfo proc;

        if (read_process_status(pid, proc)) {
            processes.push_back(proc);
        }
        // if the file couldn't be read (process died between listing and reading), skip it
    }

    closedir(dir);

    // sort by PID for clean output
    std::sort(processes.begin(), processes.end(),
              [](const ProcessInfo& a, const ProcessInfo& b) {
                  return a.pid < b.pid;
              });

    return processes;
}