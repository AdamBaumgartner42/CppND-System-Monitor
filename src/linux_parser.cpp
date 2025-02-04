#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  // % = MemFree / MemTotal
  std::string key, value, unit, line;
  std::string const total_key = "MemTotal:";
  std::string const free_key = "MemFree:";
  float total = 0;
  float free = 0;

  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      while(linestream >> key >> value >> unit){
        if (key == total_key){ total = std::stof(value); } 
        else if (key == free_key){ free = std::stof(value); }
      }
    }
  }
  if (free / total > 0 ){ return free / total; }
  else { return -1; }
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() { 
  // Uptime = Overall seconds not in idle
  std::string uptime, idle;
  std::string line;

  std::ifstream stream (kProcDirectory + kUptimeFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idle;
  }
  return std::stol(uptime);
}

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  vector<string> latestCPU = LinuxParser::CpuUtilization(); 
  long long jiffies = 0;
  for(string value : latestCPU){
    jiffies += std::stoll(value);
  }
  return jiffies; 
}

// DONE: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  // sum index 14, 15, 16, 17
  int const utime = 14;
  int const stime = 15;
  int const cutime = 16;
  int const cstime = 17;
  int index = 1;
  long long sum = 0;
 
  std::string line;
  std::ifstream filestream (kProcDirectory + to_string(pid) + kStatFilename);

  if (filestream.is_open()){
    std::getline(filestream, line); 
    std::stringstream linestream(line); 
    for(string word;std::getline(linestream, word, ' ');){ 
      if(index == utime || index == stime || index == cutime || index == cstime){
        sum += stoll(word);
      }  
      index++;    
    }
  }
  return sum; 
}

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  vector<string> latestCPU = LinuxParser::CpuUtilization();
  long long active_jiffies = 0;
  for(unsigned int i = 0; i < latestCPU.size(); i++){
    if (i == 3 || i == 4){ 
      continue; // skip 3: idle, 4: iowait
    } else {
      active_jiffies += std::stoll(latestCPU[i]);
    }
  } 
  return active_jiffies; 
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> latestCPU = LinuxParser::CpuUtilization();
  long long idle_jiffies = 0;
  for(unsigned int i = 0; i < latestCPU.size(); i++){
    if (i == 3 || i == 4){
      idle_jiffies += std::stoll(latestCPU[i]);
    } else {
      continue;
    }
  } 
  return idle_jiffies; 
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<std::string> cpu_usage;
  std::string val; 
  std::string line;
  std::ifstream filestream (kProcDirectory + kStatFilename);
  
  if (filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream (line);
    
    while (linestream >> val){ // while a value exists in the line
      if (val == "cpu"){ continue; }
      else { cpu_usage.push_back(val); }
    }
  }
  return cpu_usage; 
}

// DONE: Read and return the total number of processes (Similarity to active running proccesses)
int LinuxParser::TotalProcesses() {
  std::string category, count;
  std::string line;

  std::ifstream filestream (kProcDirectory + kStatFilename);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream (line);
      while (linestream >> category >> count){
        if(category == "processes"){
          return std::stoi(count);
        }
      }
    }
  }
  return 0; 
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  std::string category, count;
  std::string line;

  std::ifstream filestream (kProcDirectory + kStatFilename);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream (line);
      while (linestream >> category >> count){
        if(category == "procs_running"){
          return std::stoi(count);
        }
      }
    }
  }
  return 0; 
}

// DONE: Read and return the command associated with a process
// *Problem*: ncurses_display doesn't match HTOP
string LinuxParser::Command(int pid) { 
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  string line;
  if(getline(filestream, line)){
    return line;
  } else {
    return string();
  }
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  std::string line, key, value, value_MB_str;
  std::string const mem_key = "VmSize:";
  
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()){
    while (std::getline(filestream, line)){
      std::stringstream linestream(line);
      if(linestream >> key >> value){
        if (key == mem_key){
          if (int value_kB = stoi(value)){
            double scale = 0.01;
            double value_MB = value_kB/std::pow(2,10);
            value_MB = (int)(value_MB/scale)*scale;
            value_MB_str = to_string(value_MB);
            
            return value_MB_str;
          }
        }
      }
    }
  }
  return std::string();
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 

  std::string uid_path = kProcDirectory + std::to_string(pid) + kStatusFilename;
  std::string line;

  std::string key;
  std::string value;

  std::ifstream filestream(uid_path);
  while(std::getline(filestream, line)){
    std::istringstream linestream (line);
    while(linestream >> key >> value){
      if (key == "Uid:"){
        return value;
      }
    }
  }
  return string(); // return a blank string as fall through value?
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int uid) { 

  string uid_ = std::to_string(uid); // safe? Always works? 

  string key, x, value;
  string line;
  string user_path = kPasswordPath;
  std::ifstream filestream(user_path);
  while(std::getline(filestream,line)){
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
      while (linestream >> key >> x >> value) {
        if (value == uid_) {
          return key;
        }
      }
  }
  return string(); 
}

// DONE: Read and return the uptime of a process
// *Problem*: values don't match HTOP
long LinuxParser::UpTime(int pid) { 
  std::string line;
  int index = 1;
  int const uptime_idx = 22;
  std::ifstream filestream("/proc/" + std::to_string(pid) + "/stat");
  if (filestream.is_open()){
    std::getline(filestream, line);
    std::stringstream linestream(line);
    for(std::string word;std::getline(linestream, word, ' ');){
      if(index == uptime_idx){
        return (stoll(word)/sysconf(_SC_CLK_TCK));
      }
      index ++;
    }
  }
  return 0; 
}



// Helper Functions

string LinuxParser::parse_delimiter (std::ifstream& filestream, string delimiter){
  string cmd;
  for(string line; getline(filestream,line);){
    vector<string> info = splitOnString(line, delimiter);
    for(auto data : info){ // issue with first space included
      cmd = cmd + delimiter + data;
    }
  }
  return cmd;
}


// SplitOnString helper function
vector<string> LinuxParser::splitOnString(string &input, string delimiter){
  vector<string> result;
  int start = 0;
  int end = input.substr(0).find(delimiter); // begin .find() at index 0 of the string
  while(start <= end) {
    string word = input.substr(start, end-start); // end - start is length
    result.push_back(word);
    start = end + delimiter.size(); // update start index
    end = start + input.substr(start).find(delimiter); // update end index
  }
  // last word : no more delimiters in string (here is where the cpu data is going.  Verified)
  string word = input.substr(start);
  result.push_back(word);
  return result;
}