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

// this function returns number of dots (given in as parameter) in a string
// cash A/c ............... Dr. (this kind of dots)
std::string dots(int x) {
  std::string s;
  for (int i = 0; i < x; i++) s += ".";
  return s;
}