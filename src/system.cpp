#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { 
    return cpu_; 
}

//std::vector<Process> processes_ = {};
// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    processes_.clear();
    vector<int> pids = LinuxParser::Pids();
    for (int pid : pids) {
        processes_.emplace_back(Process(pid));
    }
    std::sort(processes_.begin(), processes_.end());
    return processes_; 
}

// /proc/version 3rd token
std::string System::Kernel() { return LinuxParser::Kernel(); }

// /proc/meminfo 
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// /etc/os-release
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// /proc/stat //under procs_running
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// /proc/stat //under processes
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// /proc/uptime
// TODO: Return the number of seconds since the system started running
long System::UpTime() { return LinuxParser::UpTime(); }