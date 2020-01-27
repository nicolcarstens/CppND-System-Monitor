/******************************************************************************
*
*  System monitor similar to mini version of "htop"
*
*  Work done by Nicol Carstens, January 2020
*  Towards completion Project 2 of Udacity Nanodegree in C++
*  Baseline code provided by udacity.com
*
*  Copyright: Nicol Carstens & Udacity 2020
*
******************************************************************************/

#include "system.h"
#include "ncurses_display.h"

int main() {
  System system;
  NCursesDisplay::Display(system);
}