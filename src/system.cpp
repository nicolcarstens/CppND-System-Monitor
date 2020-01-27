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

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"

#include "linux_parser.h"

using std::string;
using std::vector;

//=============================================================================
// DONE: Return the system's CPU
//=============================================================================
Processor& System::Cpu() { return cpu_; }

//=============================================================================
// DONE: Return a container composed of the system's processes
//=============================================================================
vector<Process>& System::Processes() {
  processes_.clear();

  vector<int> pids = LinuxParser::Pids();
  for (const int& pidint : pids) {
    Process p(pidint);
    if (p.UpdateInfo()) {
      processes_.push_back(p);
    } else {
      // something went wrong reading info on this PID!
      // Not adding it to the list. Sound an alarm?
      // Going to quiet fail for now. No clear instruction/requirement.
      // std::cout << "BAD pid!! You are = " << pidint << "\n";
    }
  }

  // comparitor overload can be changed to sort based on different field
  sort(processes_.begin(), processes_.end());

  return processes_;
}

//=============================================================================
// DONE: Return the system's kernel identifier (string)
//=============================================================================
string System::Kernel() { return LinuxParser::Kernel(); }

//=============================================================================
// DONE: Return the system's memory utilization
//=============================================================================
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

//=============================================================================
// DONE: Return the operating system name
//=============================================================================
string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

//=============================================================================
// DONE: Return the number of processes actively running on the system
//=============================================================================
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

//=============================================================================
// DONE: Return the total number of processes on the system
//=============================================================================
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

//=============================================================================
// DONE: Return the number of seconds since the system started running
//=============================================================================
long System::UpTime() { return LinuxParser::UpTime(); }