/******************************************************************************
*
*  Process ("tasks") class
*
*  Basic class for Process representation
*  It contains relevant attributes as shown below
*
*  Work done by Nicol Carstens, January 2020
*  Towards completion Project 2 of Udacity Nanodegree in C++
*  Baseline code provided by udacity.com
*
*  Copyright: Nicol Carstens & Udacity 2020
*
******************************************************************************/

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

//=============================================================================
// DONE: NICOL adds constructor for Process.
//       The provided code used the default constructor, which is okay, but...
//       A process with no PID makes no sense
// TODO: could add more parameters to constructor and have have default values
//       them rather than initializing them in the constructor? Does it matter?
//=============================================================================
Process::Process(const int pidint) : pid_(pidint) {
  user_ = "";
  command_ = "";
  ram_ = "";
  up_time_ = 0;
  cpu_utilization_ = 0.0;
};

//=============================================================================
// Update the information for the PID
// Return FALSE if something went wrong
// Return TRUE if no errors were detected
//=============================================================================
bool Process::UpdateInfo(void) {
  bool validUpdate = false;
  // if we have a valid PID, then let's update the private fields
  if (pid_ > 0) {
    float processUpTime = (float)LinuxParser::UpTime(pid_);

    // concerned about divide by zero ...
    if ((processUpTime) < 0.00000000000001) {
      cpu_utilization_ = 0.0;
    } else {
      cpu_utilization_ =
          ((float)LinuxParser::ActiveJiffies(pid_) / LinuxParser::ClkTPS()) /
          processUpTime;
    }

    // should never happen that cpu_utilization_ > 100.0 ... but in case...
    if (cpu_utilization_ > 100.0) cpu_utilization_ = 100.0;

    command_ = LinuxParser::Command(pid_);
    ram_ = LinuxParser::Ram(pid_);
    user_ = LinuxParser::User(pid_);
    up_time_ = LinuxParser::UpTime(pid_);

    validUpdate = true;
  };
  return validUpdate;
};

//=============================================================================
// DONE: Return this process's ID
//=============================================================================
int Process::Pid() { return pid_; }

//=============================================================================
// DONE: Return this process's CPU utilization
// TODO: Is it possible that the user could call CpuUtilization() before
//       it was calculated?? Check.
//=============================================================================
float Process::CpuUtilization() { return cpu_utilization_; }

//=============================================================================
// DONE: Return the command that generated this process
//=============================================================================
string Process::Command() { return command_; }

//=============================================================================
// DONE: Return this process's memory utilization
//=============================================================================
string Process::Ram() { return ram_; }

//=============================================================================
// DONE: Return the user (name) that generated this process
//=============================================================================
string Process::User() { return user_; }

//=============================================================================
// DONE: Return the age of this process (in seconds)
//=============================================================================
long int Process::UpTime() { return up_time_; }

//=============================================================================
// DONE: Overload the "less than" comparison operator for Process objects
//=============================================================================
bool Process::operator<(Process const& a) const {
  return (cpu_utilization_ > a.cpu_utilization_ ? true : false);
}