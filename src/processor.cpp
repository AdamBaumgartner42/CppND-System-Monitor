#include "processor.h"
#include "linux_parser.h"
#include <string>
#include <vector>

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() { 

  float result = float(LinuxParser::ActiveJiffies()) / LinuxParser::Jiffies();

  return result;

}

