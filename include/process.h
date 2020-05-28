#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "linux_parser.h"
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/

class Process {
 public:
  //constructor
  Process(int p) : pid_(p){
    CpuUtilization();
  }

  int Pid();                               //returns pid to the other functions 
  std::string User();                      // Done
  std::string Command();                   // Done
  float CpuUtilization();                  // Done 
  std::string Ram();                       // Done
  long int UpTime();                       // Done
  bool operator<(Process const& a) const;  //

  // TODO: Declare any necessary private members
 private:
    float CpuUtilization_;
    int pid_;
};

#endif