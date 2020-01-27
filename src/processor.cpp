/******************************************************************************
*
*  Processor (CPU) class
*
*  Work done by Nicol Carstens, January 2020
*  Towards completion Project 2 of Udacity Nanodegree in C++
*  Baseline code provided by udacity.com
*
*  Copyright: Nicol Carstens & Udacity 2020
*
******************************************************************************/

#include "processor.h"
#include "linux_parser.h"

//=============================================================================
// DONE: Return the aggregate CPU utilization as a fraction
//       NB: CPU => sum of all CPUs!!
//       Calculation based on the provided link...
//       https://github.com/LubuntuFu/DevopsWiki/wiki/How-Linux-CPU-Usage-
//               Time-and-Percentage-is-calculated
//       Total CPU time since boot => LinuxParser::ActiveJiffies()
//       Total CPU idle time since boot => LinuxParser::IdleJiffies()
//       Total CPU usage time since boot => LinuxParser::Jiffies()
//=============================================================================
float Processor::Utilization() {
  return ((float)LinuxParser::Jiffies() / (float)LinuxParser::ActiveJiffies());
}