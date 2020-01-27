/******************************************************************************
*
*  Helper function to format time output HH:MM:SS
*
*  Work done by Nicol Carstens, January 2020
*  Towards completion Project 2 of Udacity Nanodegree in C++
*  Baseline code provided by udacity.com
*
*  Copyright: Nicol Carstens & Udacity 2020
*
******************************************************************************/

#include <string>

#include "format.h"

using std::string;
using std::to_string;

//=============================================================================
// DONE:   Complete this helper function.
//         Did not include extra libs. Just used what was provided.
// INPUT:  Long int measuring seconds
// OUTPUT: string formated HH:MM:SS
// TODO:   Could add more testing for display... tested by hand. Looks fine.
//=============================================================================
string Format::ElapsedTime(long seconds) {
  int sec = (seconds % 60);
  int hrs = (seconds / 3600);
  int min = (seconds - hrs * 3600 - sec) / 60;

  string str_hrs = (hrs < 10 ? ("0" + to_string(hrs)) : (to_string(hrs)));
  string str_min = (min < 10 ? ("0" + to_string(min)) : (to_string(min)));
  string str_sec = (sec < 10 ? ("0" + to_string(sec)) : (to_string(sec)));

  string temp = (str_hrs + ":" + str_min + ":" + str_sec);

  return temp;
}