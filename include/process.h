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

#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
 public:
  Process(const int pidint);  // NICOL ADDS constructor
  bool UpdateInfo(void);      // NICOL ADDS Populate / Update fields
  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization();
  std::string Ram();
  long int UpTime();
  bool operator<(Process const& a) const;

  // DONE: Declare any necessary private members
 private:
  int pid_;                // NICOL ADDS
  std::string user_;       // NICOL ADDS
  std::string command_;    // NICOL ADDS
  float cpu_utilization_;  // NICOL ADDS
  std::string ram_;        // NICOL ADDS
  long int up_time_;       // NICOL ADDS
};

#endif