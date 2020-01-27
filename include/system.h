/******************************************************************************
*
*  System class -> contains RAM, Processor, Processes, etc
*
*  Work done by Nicol Carstens, January 2020
*  Towards completion Project 2 of Udacity Nanodegree in C++
*  Baseline code provided by udacity.com
*
*  Copyright: Nicol Carstens & Udacity 2020
*
******************************************************************************/

#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  Processor& Cpu();
  std::vector<Process>& Processes();
  float MemoryUtilization();
  long UpTime();
  int TotalProcesses();
  int RunningProcesses();
  std::string Kernel();
  std::string OperatingSystem();

  // DONE: Define any necessary private members
 private:
  Processor cpu_;
  std::vector<Process> processes_;
};

#endif