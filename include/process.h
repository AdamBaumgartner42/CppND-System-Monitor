#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               
  std::string User();                      
  std::string Command();                   
  float CpuUtilization();                  
  std::string Ram();                       
  long int UpTime();                       
  bool operator<(Process const& a) const; 
  
  // New Functions
  void SetPid(int pid);
  void SetUid(std::string uid);
  void SetUsername(std::string user);
  void SetCpuUtilization(float cpuUtilization);

  std::string GetUid();


  // DONE: Declare any necessary private members
 private:
   int pid_;
   std::string uid_;
   std::string user_;
   float cpuUtilization_;
};

#endif