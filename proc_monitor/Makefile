CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
SRC_DIR = src
BUILD_DIR = build

SRCS = $(SRC_DIR)/main.cpp
# add more as you go:
SRCS += $(SRC_DIR)/cpu_info.cpp
SRCS += $(SRC_DIR)/mem_info.cpp
SRCS += $(SRC_DIR)/stats_info.cpp
SRCS += $(SRC_DIR)/proc_info.cpp

TARGET = proc_monitor

all: $(TARGET)

$(TARGET): $(SRCS)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)
	rm -rf $(BUILD_DIR)