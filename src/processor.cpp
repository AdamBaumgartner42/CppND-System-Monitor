#include "processor.h"
#include "linux_parser.h"
#include <string>
#include <vector>

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() { 
    // Calculated variables
    float nonIdleTime;
    float idleTime;
    
    float totalTime;

    // Status variables
    long long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;

    // pull latest processor status information
    std::vector<std::string> latestCPU = LinuxParser::CpuUtilization();

    // update status variables  [hard-coded - not ideal]
    user = std::stoll(latestCPU[0]);
    nice = std::stoll(latestCPU[1]);
    system = std::stoll(latestCPU[2]);
    idle = std::stoll(latestCPU[3]);
    iowait = std::stoll(latestCPU[4]);
    irq = std::stoll(latestCPU[5]);
    softirq = std::stoll(latestCPU[6]);
    steal = std::stoll(latestCPU[7]);
    guest = std::stoll(latestCPU[8]);
    guest_nice = std::stoll(latestCPU[9]);

    // update calculated variables
    nonIdleTime = user + nice + system + irq + softirq + steal + guest + guest_nice; // Active Jiffies
    idleTime = idle + iowait; // Idle Jiffies
    totalTime = nonIdleTime + idleTime; // Jiffies

    return nonIdleTime / totalTime;  

}
