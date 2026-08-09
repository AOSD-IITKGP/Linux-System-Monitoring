#ifndef STATS_INFO_HPP
#define STATS_INFO_HPP

struct StatsInfo {
    double cpu_usage;
    double uptime_seconds;
    double load_1;
    double load_5;
    double load_15;
};

StatsInfo get_stats_info();

#endif