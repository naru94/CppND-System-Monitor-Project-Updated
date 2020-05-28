#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// /proc

// TODO: Return this process's ID
int Process::Pid() { 
    return pid_; 
}

float Process::CpuUtilization() {
  this->CpuUtilization_ = LinuxParser::CpuUtilization_Process(pid_);
  return this->CpuUtilization_; 
}

string Process::Command() { return LinuxParser::Command(pid_); }

string Process::Ram() { return LinuxParser::Ram(pid_); }

string Process::User() { return LinuxParser::User(pid_); }

long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

bool Process::operator<(Process const& a) const { 
    return (a.CpuUtilization_ < (this->CpuUtilization_));
}