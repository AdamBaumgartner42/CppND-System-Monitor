#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp
  
  // New Functions
  void SetPid(int pid);
  void SetUid(std::string uid);
  void SetUsername(std::string user);
  std::string GetUid();

  // TODO: Declare any necessary private members
 private:
   int pid_;
   std::string uid_;
   std::string user_;
};

#endif