#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// DONE: Return this process's ID
int Process::Pid() { return pid_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() {
  return 100*float(LinuxParser::ActiveJiffies(pid_)) / LinuxParser::Jiffies();
}

// Active: Return the command that generated this process
string Process::Command() { 
  return LinuxParser::Command(pid_); 
}

// TODO: Return this process's memory utilization
string Process::Ram() { return string(); }

// DONE: Return the user (name) that generated this process
string Process::User() { return user_; } 

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return 0; }

// TODO: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }


// Additional Functions
void Process::SetPid(int pid){ pid_ = pid; }
void Process::SetUid(std::string uid){ uid_ = uid; }
void Process::SetUsername(std::string user){ user_ = user; }
string Process::GetUid(){ return uid_; }