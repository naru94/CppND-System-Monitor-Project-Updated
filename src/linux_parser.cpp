#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// (cd .. && make build)
// ./monitor

// // TODO: Read and return the number of jiffies for the system
// long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
// long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// // TODO: Read and return the number of active jiffies for the system
// long LinuxParser::ActiveJiffies() { return 0; }

// // TODO: Read and return the number of idle jiffies for the system
// long LinuxParser::IdleJiffies() { return 0; }

// /proc/[PID]/stat file.
// TODO: Read and return CPU utilization
// vector<string> LinuxParser::CpuUtilization() { return {}; }

float LinuxParser::CpuUtilization_Process(int pid) {

  std::string PID = std::to_string(pid);
  long sys_uptime = LinuxParser::UpTime();

  std::string line;
  std::string temp;

  long utime;
  long stime;
  long cutime;
  long cstime;
  long starttime;
  float Hertz = sysconf(_SC_CLK_TCK);

  long total_time;
  float seconds;
  float cpu_usage;

  std::ifstream stream("/proc/" + PID + "/stat");
  if (stream.is_open()) {
    std::getline(stream, line);  
      // std::cout << "next line" << std::endl;
    std::istringstream linestream(line);  //set up streamer for lines
        // std::cout << line << std::endl;
    for (int i= 0; i < 22; i++) {
      linestream >> temp;
      switch (i) {
        case 13:
          utime = std::stol(temp);
          // std::cout<< "utime: " << utime << std::endl;
          continue;
        case 14:
          stime = std::stol(temp);
          // std::cout<< "stime: " << stime << std::endl;
          continue;
        case 15:
          cutime = std::stol(temp);
          // std::cout<< "cuttime: " << cuttime << std::endl;
          continue;
        case 16:
          cstime = std::stol(temp);
          // std::cout<< "cstime: " << cstime << std::endl;
          continue;
        case 21:
          starttime = std::stol(temp);
          // std::cout<< "starttime: " << starttime << std::endl;
          continue;
      }
    }
    total_time = utime + stime; // total time spent for process
    total_time += cutime + cstime; // include children processes as well
    seconds = (starttime / Hertz); //seconds =  total time elapsed in seconds since process started
    seconds = sys_uptime - seconds;
    cpu_usage = total_time/Hertz; 
    cpu_usage /= seconds ;
    // cpu_usage *= 100;
  }
  return cpu_usage;
}

string LinuxParser::Command(int pid) { 
  std::string PID = std::to_string(pid);
  std::string line;

  std::ifstream stream("/proc/" + PID + "/cmdline");
  if (stream.is_open()) {
    std::getline(stream,line);
    // std::cout<< line << std::endl;
    return line;
    }
  return line;
}

string LinuxParser::Ram(int pid) {
  std::string PID = std::to_string(pid);
  std::string line;
  std::string name;
  std::string ram;
  long ram_long;

  std::ifstream stream("/proc/" + PID + "/status");
  if (stream.is_open()) {
    while (std::getline(stream,line)) {
      std::istringstream linestream(line);
      linestream >> name >> ram;
      if (name == "VmSize:") {
        ram_long = std::stol(ram);
        ram_long /= 1000;
        return std::to_string(ram_long);
      }
    }
  }
  return std::to_string(ram_long);
}

string LinuxParser::Uid(int pid) { 
    std::string PID = std::to_string(pid);
    // std::cout << "pid within get_uid: " << PID << std::endl;
    std::string line;
    std::string name;
    std::string uid;
    std::string UID;
    std::ifstream stream("/proc/" + PID + "/status"); 
    if (stream.is_open()) {
        while(std::getline(stream, line)) {    //get multiple lines
        // std::cout << "next line" << std::endl;
          std::istringstream linestream(line);  //set up streamer for lines
          // std::cout << line << std::endl;
          linestream >> name >> uid;
          // std::cout<< "name: " << name << " uid: " << uid << std::endl;
          if (name == "Uid:")
            // std::cout << "Uid found!: " << uid << std::endl;
            return uid;
        }
    }
  return uid;
}

string LinuxParser::User(int pid) { 
  std::string UID = LinuxParser::Uid(pid);
  // std::cout<< "UID: " << UID << std::endl;

  std::string line;
  std::string user;
  std::string x;
  std::string uid;
  std::string return_user;

  std::ifstream stream("/etc/passwd"); 
  if (stream.is_open()) {
    while(std::getline(stream, line)) {    //get multiple lines
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);  //set up streamer for lines
      // std::cout<<line<<std::endl;
      linestream >> user >> x >> uid;
      // std::cout<< user << " " << x << " " << uid << std::endl;
      if (uid == UID) {
        // std::cout<< user << std::endl;
        return_user = user;
        return return_user;
      }
    }
  }
  return return_user;
}

long LinuxParser::UpTime(int pid) { 
  std::string PID = std::to_string(pid);
  // std::cout << "pid within get_uid: " << PID << std::endl;
  std::string line;
  std::string uptime;
  long uptime_sec;

  std::ifstream stream("/proc/" + PID + "/stat"); 
  if (stream.is_open()) {
    std::getline(stream, line);  
      // std::cout << "next line" << std::endl;
    std::istringstream linestream(line);  //set up streamer for lines
        // std::cout << line << std::endl;
    for (int i= 0; i < 22; i++) {
      linestream >> uptime;
      // std::cout << "uptime: " << uptime <<  std::endl;
    }
  }
  uptime_sec = std::stol(uptime);
  uptime_sec /= sysconf(_SC_CLK_TCK);
  return uptime_sec;
}

int LinuxParser::TotalProcesses() { 
  string key;
  int value;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename); // /proc/stat
  if (stream.is_open()) {
    while ( std::getline(stream, line)) {//for each line
      std::istringstream linestream(line); 
      while (linestream >> key >> value ) {//pull tokens off the stream 
         if (key == "processes") {
           return value;
         }
      }
    }
  }
  return value;
}

int LinuxParser::RunningProcesses() { 
  string key;
  int value;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename); // /proc/stat
  if (stream.is_open()) {
    while ( std::getline(stream, line)) {//for each line
      std::istringstream linestream(line); 
      while (linestream >> key >> value ) {//pull tokens off the stream 
         if (key == "procs_running") {
           return value;
         }
      }
    }
  }
  return value;
}

string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath); // /etc/os-release
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) { //multiple lines therefore while loop
      std::replace(line.begin(), line.end(), ' ', '_'); // "replace " " with "_" so we can use delimiters
      std::replace(line.begin(), line.end(), '=', ' '); // "replace = with " " so we can use delimiters
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) { //pop tokens
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' '); //replace _ with ""
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename); // /proc/version
  if (stream.is_open()) {
    std::getline(stream, line); //each line stored in line
    std::istringstream linestream(line); //stream line into linestream
    linestream >> os >> version >> kernel; //pull tokens off linestream  (os is first token, followed by version then kernal)
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids; 
  DIR* directory = opendir(kProcDirectory.c_str()); // open /proc
  struct dirent* file; //dirent structure contains the character pointer d_name which points to a string that gives the name of a file in the directory
  while ((file = readdir(directory)) != nullptr) { //readdir is read a directory
    if (file->d_type == DT_DIR) { //check if the file type (d_type) is a directory (DT_DIR)
      string filename(file->d_name); // Get the name of all the files
      if (std::all_of(filename.begin(), filename.end(), isdigit)) { //for each file, check if they are digits
        int pid = stoi(filename); //pid of the process is obtained from converting string to int
        pids.push_back(pid);      //add pid to the pids vector
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() { 
  std::string key;
  long value;
  std::string line;
  long MemTotal;
  long MemFree;
  float MemUtil;
  std::ifstream stream(kProcDirectory + kMeminfoFilename); // /proc/meminfo 
  if (stream.is_open()){
    while(std::getline(stream,line)) { //loop through multiple lines
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          MemTotal = value;
        }
        else if(key == "MemFree:") {
          MemFree = value;
          break;
        }
      }
    }
  }
  MemUtil = MemTotal - MemFree;
  MemUtil /= MemTotal;
  return MemUtil;
}

long LinuxParser::UpTime() {
  std::string uptime;
  std::string line;
  std::ifstream stream("/proc/uptime"); // /proc/uptime
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line); 
    linestream >> uptime;
  }
  return std::stol(uptime);
}