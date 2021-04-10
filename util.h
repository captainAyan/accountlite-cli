#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iostream>

#define DEBUG

#ifdef DEBUG
#define LOG(x) std::cout << x << std::endl
#define LOG_MAP(key, value) std::cout << key << " : " << value << std::endl;
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