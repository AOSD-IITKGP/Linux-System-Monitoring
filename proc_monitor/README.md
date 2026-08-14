# proc_monitor — Linux System Monitoring Tool

Reads directly from the `/proc` filesystem to report CPU, memory, and process information.

---

## Team

| S.No | Roll Number | Name |
|:---:|-------------|------|
| 1 | 23CS10013 | Chintada Yesheeth Sree Narayana |
| 2 | 23CS10017 | Dasari Veera Venkata Abhinav Teja |
| 3 | 23CS10052 | Hadwik Payidiparthy |

---

## File Structure

```text
proc_monitor/
├── Makefile
├── README.md
├── Documentation.pdf    # Contains design choices and testing
└── src/
    ├── main.cpp          # Entry point — decides static vs live mode
    ├── cpu_info.hpp
    ├── cpu_info.cpp      # Task 1 — CPU model and logical processor count
    ├── mem_info.hpp
    ├── mem_info.cpp      # Task 1 — Memory usage statistics
    ├── stats_info.hpp
    ├── stats_info.cpp    # Task 2 — CPU utilization, uptime, load averages
    ├── proc_info.hpp
    ├── proc_info.cpp     # Task 3 & 4 — Process listing and top consumers
    ├── display.hpp
    └── display.cpp       # Formatted output for all tasks
```

---

## Build

**Using Makefile (recommended):**
```bash
make
```

**Or directly with g++:**
```bash
g++ -Wall -Wextra -std=c++17 -o proc_monitor src/*.cpp
```

**Clean build artifacts:**
```bash
make clean
```

---

## Run

**Static mode — single snapshot:**
```bash
./proc_monitor
```
Displays a one-shot report of CPU info, memory usage, system statistics, running processes, and top resource consumers, then exits.

**Live mode — auto-refreshing:**
```bash
./proc_monitor -T <seconds>
```
Example:
```bash
./proc_monitor -T 5
```
Refreshes the display every 5 seconds. Press `Ctrl+C` to stop.

> **Note:** The interval must be greater than 2 seconds. The tool internally
> uses two 1-second sampling windows to compute CPU utilization — one for
> overall CPU usage (`/proc/stat`) and one for per-process CPU usage
> (`/proc/[pid]/stat`). An interval of 2 seconds or less will not leave
> enough time between refreshes for accurate measurements.

---

## What It Reports

| Section | Source | Description |
|---------|--------|-------------|
| CPU Info | `/proc/cpuinfo` | Model name, logical CPU count |
| Memory Info | `/proc/meminfo` | Total, free, available memory and usage % |
| CPU Utilization | `/proc/stat` | CPU usage % over a 1-second interval |
| System Uptime | `/proc/uptime` | Days, hours, minutes since boot |
| Load Averages | `/proc/loadavg` | 1, 5, and 15 minute load averages |
| Process List | `/proc/[pid]/status` | PID, PPID, state, threads, VmSize for all processes |
| Top 5 Memory | `/proc/[pid]/status` | Top 5 processes by RSS (actual RAM used) |
| Top 5 CPU | `/proc/[pid]/stat` | Top 5 processes by CPU usage over 1-second interval |

---

## Requirements

- Linux environment with access to `/proc` (bare metal, VM, or WSL)
- g++ with C++17 support
- No external dependencies

---

## Notes

- All commands should be run from the `proc_monitor/` project directory.
- On Windows, use WSL (Windows Subsystem for Linux) to access `/proc`.
- Kernel threads (e.g. `kthreadd`) will show `VmSize = 0` — this is expected since they have no user-space memory mapping.
- Processes that exit between directory listing and file read are silently skipped — this is normal behaviour when reading from `/proc`.