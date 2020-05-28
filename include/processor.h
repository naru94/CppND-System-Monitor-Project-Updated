#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>

class Processor {
 public:
    float Utilization();  
    void set_prev_values(std::vector<int> cpu_stats) {
        prevuser = cpu_stats[0];
        prevnice = cpu_stats[1];
        prevsystem = cpu_stats[2];
        previdle = cpu_stats[3];
        previowait = cpu_stats[4];
        previrq = cpu_stats[5];
        prevsoftirq = cpu_stats[6];
        prevsteal = cpu_stats[7];
    }
    void get_cpu_stats(std::vector<int> &cpu_stats) {
        std::string cpu_name;
        std::string line;
        std::ifstream stream("/proc/stat"); 
        if (stream.is_open()) {
            std::getline(stream, line);
            std::istringstream linestream(line); 
            linestream >> cpu_name;
            for (int i=0; i<8; i++) {
            linestream >> cpu_stats[i];
            }
        }
    }


 private:
    int previdle;
    int previowait;
    int prevuser; 
    int prevnice;
    int prevsystem;
    int previrq;
    int prevsoftirq;
    int prevsteal;
};

#endif