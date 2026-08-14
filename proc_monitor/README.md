Team Member Details:
S.No          Name            Roll number
1. 
2. 
3. 


Build & Run:

Build:

- Using the Makefile (recommended)

    make

- Or compile directly with g++

    g++ -Wall -Wextra -std=c++17 -o proc_monitor src/*.cpp

Run modes:

- Static (one-shot)

    ./proc_monitor

  Shows a single snapshot of CPU, memory, system statistics and running processes.

- Live (repeating)

    ./proc_monitor -T 5

  Refreshes the display every 5 seconds. Press Ctrl+C to stop.

Clean build artifacts:

    make clean

Notes:

- Run these commands from the proc_monitor project directory.
- On Windows, use WSL or a Linux environment to access /proc.


Proposed File Structure:
proc_monitor/
├── Makefile
├── README.md
├── src/
│   ├── main.cpp                  # To decide live or static mode
│   ├── cpu_info.cpp              # CPU information related functions (task 1)
│   ├── cpu_info.hpp
│   ├── memory_info.cpp           # Memory information related functions (task 1)
│   ├── memory_info.hpp 
│   ├── stats_info.cpp            # Stats information related functions (task 2)
│   ├── stats_info.hpp
│   ├── process_info.cpp          # Process information related functions (task 3 and 4)
│   ├── process_info.hpp
│   └── display.cpp
│   └── display.hpp


