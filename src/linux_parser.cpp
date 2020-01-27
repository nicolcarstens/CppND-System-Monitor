/******************************************************************************
*
*  Collection of Linux helper fuctions to parse data from (virtual) files
*
*  See http://man7.org/linux/man-pages/man5/proc.5.html
*
*  Work done by Nicol Carstens, January 2020
*  Towards completion Project 2 of Udacity Nanodegree in C++
*  Baseline code provided by udacity.com
*
*  Copyright: Nicol Carstens & Udacity 2020
*
******************************************************************************/

#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::stol;
using std::stoi;
using std::getline;
using std::replace;
using std::string;
using std::to_string;
using std::vector;
using std::ifstream;
using std::istringstream;

//=============================================================================
// As an engineer using "Hertz" rather than a name like clock ticks per second
// hurts my eyes... It's silly. I know.
//=============================================================================
int LinuxParser::ClkTPS(void) { return sysconf(_SC_CLK_TCK); }

//=============================================================================
// DONE: An example of how to read data from the filesystem
//=============================================================================
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      replace(line.begin(), line.end(), ' ', '_');
      replace(line.begin(), line.end(), '=', ' ');
      replace(line.begin(), line.end(), '"', ' ');
      istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

//=============================================================================
// DONE: An example of how to read data from the filesystem
//=============================================================================
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    getline(stream, line);
    istringstream linestream(line);
    linestream >> os >> kernel >> version;
  }
  return version;
}

//=============================================================================
// BONUS: Update this to use std::filesystem
// NOTE:  Not going to do. Learning value not justified. Time vs Money.
//        Requires updated compiler and the like.
//=============================================================================
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR *directory = opendir(kProcDirectory.c_str());
  struct dirent *file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

//=============================================================================
// DONE: Read & return the system memory utilization as a fraction of total
// NOTE: Calculating & interpreting memory availability and use it not trivial.
//       The virtual file /proc/meminfo gives what looks like a realistic
//       summary that I have validated against htop. If you accumulate the
//       memory in the provided example, the machine has more than 60GB of
//       memory. I don't think so. It would appear the shared/virtual memory
//       gets listed between multiple processes more than once. The goal is to
//       learn C++. Not debate the technical details of memory management.
//=============================================================================
float LinuxParser::MemoryUtilization() {
  float memTotal = (float)LinuxParser::ReadProcMeminfo("MemTotal");
  float memAvailable = (float)LinuxParser::ReadProcMeminfo("MemAvailable");
  return (memTotal - memAvailable) / memTotal;
}

//=============================================================================
// TODO: Read and return the system uptime (in seconds)
// NOTE: UpTime taken from "/proc/uptime" (field #1)
//       Note that there are potentially multiple processors running at the same
//       time and thus Juffies NOT = uptime in seconds * ticks per second!!
//       It can be 2... or 3 .... or more times higher! Given number of cores!
//       http://man7.org/linux/man-pages/man5/proc.5.html -> searh "proc/uptime"
//       https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-
//           cpu-usage-of-an-application-from-proc-pid-stat
//=============================================================================
long LinuxParser::UpTime() {
  string line;
  string str_value;
  long value = 0;
  ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    getline(filestream, line);
    istringstream linestream(line);
    linestream >> str_value;
    value = stol(str_value);
  }
  // no error handling happening...
  return value;
}

//=============================================================================
// TODO: Read and return the number of jiffies for the system
//       Total CPU time (in system ticks / "jiffies") since boot
// NOTE: About jiffies (clock ticks)...
//       run >> getconf CLK_TCK to get jiffies per second
//       My VM gives answer of 100 => 100 jiffies / second = 10ms / jiffie
//       http://man7.org/linux/man-pages/man5/proc.5.html -> searh "proc/uptime"
//       https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-
//               cpu-usage-of-an-application-from-proc-pid-stat
//       https://github.com/LubuntuFu/DevopsWiki/wiki/How-Linux-CPU-Usage-
//               Time-and-Percentage-is-calculated
//=============================================================================
long LinuxParser::Jiffies() { return (ActiveJiffies() - IdleJiffies()); }

//=============================================================================
// Takes the first line in a specified file and breaks it up into strings
// Removes all ":"
//=============================================================================
vector<string> Line2StrVector(string filename) {
  vector<string> vstr;
  string line;
  string value;
  ifstream filestream(filename);
  if (filestream.is_open()) {
    getline(filestream, line);

    // FOR TESTING:
    // string line{ "968 (Xorg) S 966 966 966 1025 966 4194560 1343231 83084 200
    // 5 86972 46384 269 45 20 0 4 0 2355 824307712 39867 18446744073709551615
    // 94218370744320 94218373105768 140721572940992 0 0 0 0 4096 1098933999 0 0
    // 0 17 0 0 0 157 0 0 94218375206576 94218375263552 94218382950400
    // 140721572945090 140721572945208 140721572945208 140721572945893 0"};
    // #14 => 86972
    // #15 => 46384
    // #16 => 269
    // #17 => 45
    // #22 => 2355

    istringstream linestream(line);
    while (linestream >> value) {
      vstr.push_back(value);
    }
  }
  filestream.close();
  return vstr;
}

//=============================================================================
// DONE: Read and return the number of active jiffies for a PID
//=============================================================================
long LinuxParser::ActiveJiffies(int pid) {
  string filename = kProcDirectory + to_string(pid) + kStatFilename;
  vector<string> vstr = Line2StrVector(filename);

  if (vstr.size() >= 21) {
    return (stol(vstr[13]) + stol(vstr[14]) + stol(vstr[15]) + stol(vstr[16]));
  } else {
    // Fails quietly
    return 0;
  }
}

//=============================================================================
// DONE: Read and return the number of active jiffies for the system
// NOTE: Output in "juffies" (not seconds) - sum for all processors!
//       See "proc/stat" in man-pages
//=============================================================================
long LinuxParser::ActiveJiffies() {
  vector<string> cpu_stats = LinuxParser::CpuUtilization();

  // normal processes executing in user mode
  long user = stol(cpu_stats[LinuxParser::kUser_]);

  // niced processes executing in user mode
  long nice = stol(cpu_stats[LinuxParser::kNice_]);

  // processes executing in kernel mode
  long system = stol(cpu_stats[LinuxParser::kSystem_]);

  // twiddling thumbs
  long idle = stol(cpu_stats[LinuxParser::kIdle_]);

  // waiting for I/O to complete
  long iowait = stol(cpu_stats[LinuxParser::kIOwait_]);

  // servicing interrupts
  long irq = stol(cpu_stats[LinuxParser::kIRQ_]);

  // servicing softirqs
  long softirq = stol(cpu_stats[LinuxParser::kSoftIRQ_]);

  // involuntary wait
  long steal = stol(cpu_stats[LinuxParser::kSteal_]);

  return (user + nice + system + idle + iowait + irq + softirq + steal);
}

//=============================================================================
// DONE: Read and return the number of idle jiffies for the system
//=============================================================================
long LinuxParser::IdleJiffies() {
  vector<string> cpu_stats = LinuxParser::CpuUtilization();

  // twiddling thumbs
  long idle = stol(cpu_stats[LinuxParser::kIdle_]);

  // waiting for I/O to complete
  long iowait = stol(cpu_stats[LinuxParser::kIOwait_]);

  return (idle + iowait);
}

//=============================================================================
// DONE: Read and return CPU utilization (total processor use)
// NOTE: https://github.com/LubuntuFu/DevopsWiki/wiki/How-Linux-CPU-Usage-
//               Time-and-Percentage-is-calculated
//=============================================================================
vector<string> LinuxParser::CpuUtilization() {
  vector<string> temp_strings;
  string line, temp;
  ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    getline(stream, line);
    istringstream linestream(line);
    linestream >> temp;
    // I have adding kEndCPUStates_ to end of enum CPUStates in linux_parser.h
    for (int ii = 0; ii < kEndCPUStates_; ii++) {
      linestream >> temp;
      temp_strings.push_back(temp);
    }
  }
#if 0
  // TESTING:
  std::vector<std::string> cpu{"4705","356","584","3699","23","23","0","0","0","0"};  
  return cpu;//temp_strings;
#else
  return temp_strings;
#endif
}

//=============================================================================
// DONE: Read and return the total number of processes
// NOTE: Looking at the example provided, it can easily be 30k-40k
//=============================================================================
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  int value;
  ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return value;
        }
      }
    }
  }
  return 0;
}

//=============================================================================
// DONE: Read and return the number of running processes
// NOTE: Looking at the example this is typically 2/3/4/5 - not many!
//=============================================================================
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  int value;
  ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return value;
        }
      }
    }
  }
  // Fails quietly.
  return 0;
}

//=============================================================================
// DONE: Read and return the command associated with a process
// QUESTION: some processes appear to have no command
//           but htop lists some of them in green?
//=============================================================================
string LinuxParser::Command(int pid) {
  string line{"BADLINE"};
  ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    getline(filestream, line);
  }
  return line;
}

//=============================================================================
// ADD: NICOL ADDS to read field number x from /proc/[pid]/stat
// VERY useful file
//=============================================================================
long LinuxParser::ReadProcPIDStat(const int &pid, const int &field_number) {
  string line;
  string str_value;
  long value;
  ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    if (getline(filestream, line)) {
      istringstream linestream(line);
      for (int ii = 0; ii < field_number; ii++) {
        linestream >> str_value;
        break;
      };
    };
  };
  value = stol(str_value);
  return value;
}

//=============================================================================
// ADD: NICOL ADDS to read field number x from /proc/[pid]/stat
//      VERY useful file
//=============================================================================
long LinuxParser::ReadProcPIDStatus(const int &pid,
                                    const std::string &search_key) {
  string line;
  string key;
  string str_value;
  long value = 0;
  ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      replace(line.begin(), line.end(), ':', ' ');
      istringstream linestream(line);
      linestream >> key >> str_value;
      if (key == search_key) {
        value = stol(str_value);
        return value;
      }
    }
  }
  // no error handling taking place
  return value;
}

//=============================================================================
// ADD: NICOL ADDS to read field number x from /proc/meminfo
//      /proc/meminfo => VERY useful file
//=============================================================================
long LinuxParser::ReadProcMeminfo(const std::string &search_key) {
  string line;
  string key;
  string str_value;
  long value = 0;
  ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      replace(line.begin(), line.end(), ':', ' ');
      istringstream linestream(line);
      linestream >> key >> str_value;
      if (key == search_key) {
        value = stol(str_value);
        return value;
      }
    }
  }
  // no error handling taking place
  return value;
}

//=============================================================================
// DONE: Read and return the memory used by a process (not processor/system!)
// NOTE: http://man7.org/linux/man-pages/man5/proc.5.html
//       Follow files contain memory info:
//        --> /proc/[pid]/status
//        --> /proc/[pid]/stat
//        --> /proc/[pid]/statm
//        "htop" has the option to display Resident, Virtual, Share, etc mem
//        The requirement is not very clear with regards to the mem display!
//=============================================================================
string LinuxParser::Ram(int pid) {
  int kB2mB = 1000;
  int mbMem = (ReadProcPIDStatus(pid, "VmRSS") / kB2mB);
  return to_string(mbMem);
}

//=============================================================================
// DONE: Read and return the user ID associated with a process
//=============================================================================
string LinuxParser::Uid(int pid) {
  int uid = ReadProcPIDStatus(pid, "Uid");
  return to_string(uid);
}

//=============================================================================
// DONE: Read and return the user associated with a process
//       example = "root" or "nicol"
//=============================================================================
string LinuxParser::User(int pid) {
  string uid_str_search = LinuxParser::Uid(pid);
  string line;
  string user{"NONE"};
  string uid_str;
  ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      replace(line.begin(), line.end(), ' ', '_');
      replace(line.begin(), line.end(), 'x', ' ');
      replace(line.begin(), line.end(), ':', ' ');
      istringstream linestream(line);
      while (linestream >> user >> uid_str) {
        if (uid_str == uid_str_search) {
          return user;
        }
      }
    }
  }
  // Failing quietly. No clear instruction/requirement here.
  return user;
}

//=============================================================================
// DONE: Read and return the uptime of a process (in seconds)
// NOTE: Not 100% sure what is asked for here
//       The time in seconds that the process has been running?
//       This would be the difference (CPU Uptime - time process started)
//=============================================================================
long LinuxParser::UpTime(int pid) {
  string filename = kProcDirectory + to_string(pid) + kStatFilename;
  vector<string> vstr = Line2StrVector(filename);

  if (vstr.size() >= 21) {
    long starttime = stol(vstr[21]);
    long pidUptime = UpTime() - (starttime / ClkTPS());
    return pidUptime;
  } else {
    // Failing quietly. No clear instruction/requirement here.
    return 0;
  }
}