#pragma once

#include <vector>
#include <iostream>
#include "util.h"

class Table {
  private:
  int _columns;

  std::vector<std::string> _headings;
  std::vector<std::vector<std::string>> _rows;
  
  std::vector<int> _columnSizes;

  std::vector<short> _headingsAlignment;
  std::vector<short> _columnsAlignment;

  unsigned int _padding;

  public:

  static const short LEFT_ALIGN = 1;
  static const short RIGHT_ALIGN = 2;
  static const short CENTER_ALIGN = 3;

  Table(int columns) {
    _columns = columns;
    _padding = 0;
  }

  void setHeadingsAlignment(short alignment[]) {
    for (int i = 0; i < _columns; i++){
      _headingsAlignment.push_back(alignment[i]);
    }
    
  }

  void setColumnsAlignment(short alignment[]) {
    for (int i = 0; i < _columns; i++){
      _columnsAlignment.push_back(alignment[i]);
    }
  }

  void setPadding(unsigned int padding) {
    _padding = padding;
  }

  void setHeadings(std::string h[]) {
    for(int i = 0; i < _columns; i++) {
      _headings.push_back(h[i]);
      _columnSizes.push_back(_headings.back().size());
    }
  }

  void addRow(std::string t[]) {
    std::vector<std::string> _t;
    for(int i = 0; i < _columns; i++) {
      _t.push_back(t[i]);

      if(_columnSizes.at(i) < t[i].length()) {
        _columnSizes.at(i) = t[i].length();
      }
    }
    _rows.push_back(_t);
  }

  void draw() {
    // printing headings
    printSeparator('=');
    printRow(_headings, _headingsAlignment);
    printSeparator('=');

    // row printing
    for (int i=0; i<_rows.size(); i++) {
      printRow(_rows.at(i), _columnsAlignment);
      printSeparator('-');
    }
  }

  private: 
  void printSeparator(char dash) {
    std::cout << '+';
    for (int i=0; i<_columnSizes.size(); i++) {
      int k = _columnSizes[i];
      for(int j = 0; j<(k + _padding*2); j++) std::cout << dash;
      std::cout << '+';
    }
    std::cout << std::endl;
  }

  void printRow(std::vector<std::string> values, std::vector<short> alignment) {
    std::cout << '|';
    for(int i=0; i<_columns; i++) { // value by columns in a row
      std::string value = values.at(i);
      
      short align;
      if(alignment.size() == 0) align = Table::LEFT_ALIGN;
      else align = alignment.at(i);

      if(_columnSizes.at(i) > value.length()) {
        int diff = _columnSizes.at(i) - value.length();
        for (int j = 0; j < diff; j++) {

          if(align == RIGHT_ALIGN) value.insert(0, " ");
          else if(align == CENTER_ALIGN) {
            if (j%2 == 0) value.append(" ");
            if (j%2 == 1) value.insert(0, " ");
          }
          else value.append(" ");
        }
      }

      for (int j = 0; j < _padding; j++){
        value.append(" ");
        value.insert(0, " ");
      }
      std::cout << value << "|";
    }
    std::cout << std::endl;
  }

};
