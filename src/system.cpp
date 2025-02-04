#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h" 

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/

// DONE: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// NEW: Compare interval function
bool compareInterval(Process a, Process b) {
    return (b < a);
}

// DONE: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    processes_.clear();

    // Parse active pids from /proc/
    std::vector<int> active_pids = LinuxParser::Pids();

    // Iterate over active pids and add them processes_ vector
    for (int pid : active_pids){
        Process process; 
        process.SetPid(pid);
        process.SetUid(LinuxParser::Uid(pid)); 
        process.SetUsername(LinuxParser::User(stoi(process.GetUid()))); 
        process.SetCpuUtilization(100*float(LinuxParser::ActiveJiffies(pid)) / LinuxParser::Jiffies());

        processes_.push_back(process); 
    }

    sort(processes_.begin(), processes_.end(), compareInterval ); 
        
    return processes_; 
}

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// DONE: Return the system's memory utilization percentage
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// DONE: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// DONE: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// DONE: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// DONE: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
