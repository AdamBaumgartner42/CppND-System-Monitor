#include <string>

#include "format.h"

using std::string;

// DONE: Complete this helper function
// Output: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
  int sec = seconds % 60;
  int min = ((seconds - sec) % 3600) / 60; // remainder 1 hour seconds divided by 60
  int hr = seconds / 3600; // no remainder

  std::string output = std::to_string(hr) + ":" + std::to_string(min) + ":" + std::to_string(sec);
    
  return output; 
}