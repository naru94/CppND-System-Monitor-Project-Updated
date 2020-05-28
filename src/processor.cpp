#include "processor.h"

float Processor::Utilization() { 

    std::vector<int> cpu_stats (8); //8 elements
    //declare all variables required
    int PrevIdle; int PrevNonIdle;  
    int Idle; int NonIdle;
    int PrevTotal; int Total;
    int totald; int idled;
    float CPU_Percentage; 

    //update values at current time
    get_cpu_stats(cpu_stats);

    //get current values
    int user = cpu_stats[0];
    int nice= cpu_stats[1];
    int system=  cpu_stats[2];
    int idle=  cpu_stats[3];
    int iowait=  cpu_stats[4];
    int irq=  cpu_stats[5];
    int softirq= cpu_stats[6];
    int steal= cpu_stats[7];

    PrevIdle = previdle + previowait;
    Idle = idle + iowait;

    PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal;
    NonIdle = user + nice + system + irq + softirq + steal;

    PrevTotal = PrevIdle + PrevNonIdle;
    Total = Idle + NonIdle;

    totald = Total - PrevTotal;
    idled = Idle - PrevIdle;

    //set previous values for next iteration
    set_prev_values(cpu_stats);

    //calculate cpu percentage
    CPU_Percentage = (totald - idled);
    CPU_Percentage /= totald;
    
    return CPU_Percentage; 
}
