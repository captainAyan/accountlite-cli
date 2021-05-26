#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <ctime>

// #define DEBUG

#ifdef DEBUG
#define LOG(x) std::cout << x << std::endl
#define LOG_MAP(key, value) std::cout << key << " : " << value << std::endl
#else
#define LOG(x)
#define LOG_MAP(key, value)
#endif

// split string with delimeter
std::vector<std::string> split(std::string strToSplit, char delimeter) {
  std::stringstream ss(strToSplit);
  std::string item;
  std::vector<std::string> splittedStrings;
  while (std::getline(ss, item, delimeter)) {
    splittedStrings.push_back(item);
  }
  return splittedStrings;
}

// this function turns timestamps in to dd/mm/yyyy format e.g.17/05/2021, 03/05/2020
std::string timestampToString(const time_t rawtime) {
  tm *ltm = localtime(&rawtime);
  std::string date_string = (std::to_string(ltm->tm_mday).size()<2 ? "0":"") + std::to_string(ltm->tm_mday) + "/";
  date_string += (std::to_string(1 + ltm->tm_mon).size()<2 ? "0":"") + std::to_string(1 + ltm->tm_mon) + "/";
  date_string += std::to_string(1900 + ltm->tm_year);

  return date_string;
}

// gives the current timestamp
int timestampNow() {
  return std::time(nullptr);
}

// this function returns number of dots (given in as parameter) in a string
// cash A/c ............... Dr. (this kind of dots)
std::string dots(int x) {
  std::string s;
  for (int i = 0; i < x; i++) s += ".";
  return s;
}

// ledger name validator (ledger names cannot contain comma)
bool ledgerNameValidator(std::string& s) {
  size_t f = s.find(',');
  if (f != std::string::npos) return false;
  else return true;
}

// credit printing at the start of the application
void printCredit() {
  std::cout << "######################################" << std::endl;
  std::cout << "#                                    #" << std::endl;
  std::cout << "#   WELCOME TO ACCOUNTLITE           #" << std::endl;
  std::cout << "#               - Ayan Chakraborty   #" << std::endl;
  std::cout << "#                                    #" << std::endl;
  std::cout << "######################################" << std::endl;
}