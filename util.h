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

// this function turns "DD/MM/YYYY" into timestamp
int dateStringToTimestamp(std::string& d) {
  std::vector<std::string> a = split(d, '/');
  int day, month, year;
  day = std::stoi(a.at(0));
  month = std::stoi(a.at(1));
  year = std::stoi(a.at(2));

  struct tm t = {0};
  t.tm_mday = day;
  t.tm_mon = month - 1;
  t.tm_year = year - 1900;

  time_t when = mktime(&t); // the when variable gives the timestamp
  const struct tm *norm = localtime(&when);

  return when;
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

// check if date is valid or not (DD/MM/YYYY format)
bool isValidDateString(std::string& d) {
  try {
    std::vector<std::string> a = split(d, '/');
    int day, month, year;
    day = std::stoi(a.at(0));
    month = std::stoi(a.at(1));
    year = std::stoi(a.at(2));

    struct tm t = {0};
    t.tm_mday = day;
    t.tm_mon = month - 1;
    t.tm_year = year - 1900;

    time_t when = mktime(&t); // the when variable gives the timestamp
    const struct tm *norm = localtime(&when);

    return (norm->tm_mday == day    &&
            norm->tm_mon  == month - 1 &&
            norm->tm_year == year - 1900);
  }
  catch(std::invalid_argument e) {return false;}
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