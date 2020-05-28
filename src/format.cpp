#include <string>
#include <cmath>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:ss
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  std::string elapsedtime;
  int remainder;
  int hh;
  int mm;
  int ss;
  std::string HH;
  std::string MM;
  std::string SS;

  //get the hours, minutes and seconds.
  hh = std::floor(seconds/3600); 
  remainder = seconds - (hh*3600);
  mm = std::floor(remainder/60);
  ss = remainder%60;

  //Make sure they are presented in double digit format
  if (hh/10 > 0)  
    HH = std::to_string(hh);
  else
    HH = "0" + std::to_string(hh);
  if (mm/10 > 0)  
    MM = std::to_string(mm);
  else
    MM = "0" + std::to_string(mm);
  if (ss/10 > 0)  
    SS = std::to_string(ss);
  else
    SS = "0" + std::to_string(ss);

  elapsedtime = HH +":"+ MM +":"+ SS;
  return elapsedtime;    
}