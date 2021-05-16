/*

This code is available in this repo 👇
https://github.com/captainAyan/CppCliTable

*/

#pragma once

#include <vector>
#include <iostream>
#include <string>

namespace clitable {

class Column {

  private:
  std::string _heading;
  short _heading_alignment, _column_alignment;
  unsigned int _padding, _size;
  bool _resizable;

  public:
  static const short LEFT_ALIGN = 1;
  static const short RIGHT_ALIGN = 2;
  static const short CENTER_ALIGN = 3;

  static const bool RESIZABLE = true;
  static const bool NON_RESIZABLE = false;

  Column(std::string heading, 
    short heading_alignment, 
    short column_alignment, 
    unsigned int padding,
    unsigned int size,
    bool resizable) {
    _heading = heading;
    _heading_alignment = heading_alignment;
    _column_alignment = column_alignment;
    _padding = padding;

    _size = size;
    if (_size == 0) _size = 1;

    _resizable = resizable;
    if(resizable && _heading.size() > _size) _size = _heading.length();
  }

  std::string getHeading() {return _heading;}
  short getHeadingAlignment() {return _heading_alignment;}
  short getColumnAlignment() {return _column_alignment;}
  unsigned int getPadding() {return _padding;}
  unsigned int getSize() {return _size;}
  bool isResizable() {return _resizable;}

  void setSize(unsigned int size) {_size = size;}
};

class Table {

  private:
  std::vector<Column> _columns;
  std::vector<std::vector<std::string>> _rows;

  public:
  void addColumn(Column c) {
    _columns.push_back(c);
  }

  void addRow(std::string t[]) {
    std::vector<std::string> _t;
    for(size_t i = 0; i < _columns.size(); i++) {
      _t.push_back(t[i]);

      std::vector<std::string> s = split(t[i], "\n");

      for (size_t j = 0; j < s.size(); j++) {
        if((_columns.at(i).getSize() < s[j].length()) // if value size is bigger than given suggested column size
        && (_columns.at(i).isResizable())) { // and column is resizable
          _columns.at(i).setSize(s[j].length()); // then change the column size
        }
      }
    }
    _rows.push_back(_t);
  }

  void draw() {
    // just making a list (string list) of headers
    std::vector<std::string> headers;
    for (size_t i = 0; i < _columns.size(); i++) 
      headers.push_back(_columns.at(i).getHeading());

    printSeparator('=', _columns);
    printRow(headers, _columns, true);
    printSeparator('=', _columns);

    for (size_t i=0; i<_rows.size(); i++) {
      printRow(_rows.at(i), _columns, false);
      printSeparator('-', _columns);
    }
  }


  private: 
  void printSeparator(char dash, std::vector<Column> columns) {
    std::cout << '+';
    for (size_t i=0; i<columns.size(); i++) {
      int k = columns.at(i).getSize();
      for(size_t j = 0; j<(k + columns.at(i).getPadding()*2); j++) std::cout << dash;
      std::cout << '+';
    }
    std::cout << std::endl;
  }

  void printRow(std::vector<std::string> v, std::vector<Column> c, bool isHeader) {
    // starting the row
    std::cout << '|';

    std::vector<std::string> buffer;

    for (size_t i = 0; i < c.size(); i++) {

      /* the value of this variable 'value' will be updated 
      (don't use it for reference to the original value) */
      std::string value = v.at(i);

      bool line_break = false;

      // for adding line-breaking
      size_t found = value.find('\n'); // checking for line breaks
      if (found != std::string::npos && found < c.at(i).getSize()) {
        value = value.substr(0, found);
        line_break = true;
      }

      // adjusting next-line buffer
      if(c.at(i).getSize() < value.length()) { // column size < value size
        std::string rem = value.substr(c.at(i).getSize());
        buffer.push_back(rem);
      }
      else if (c.at(i).getSize() > value.length() && line_break){
        std::string rem = v.at(i).substr(value.length()+1);
        buffer.push_back(rem);
      }
      else buffer.push_back("");

      // calculation of extra space remaining in the column after the value
      int diff; // value size ~ column size
      if(c.at(i).getSize() >= value.length()) { // value size >= column size
        diff = c.at(i).getSize() - value.length();
      }
      else {
        diff = 0;
        value = value.substr(0, c.at(i).getSize());
      }

      // adding space for alignment
      for (size_t j = 0; j < diff; j++) {
        switch((isHeader ? c.at(i).getHeadingAlignment() : c.at(i).getColumnAlignment())) {
          case Column::RIGHT_ALIGN: {
            value.insert(0, " ");
            break;
          }
          case Column::CENTER_ALIGN: {
            if (j%2 == 0) value.append(" ");
            if (j%2 == 1) value.insert(0, " ");
            break;
          }
          default: value.append(" ");
        }
      }

      // adding space as padding
      for (size_t j = 0; j < c.at(i).getPadding(); j++) {
        value.append(" ");
        value.insert(0, " ");
      }

      // printing the value
      std::cout << value << "|";
    }
    // ending the row
    std::cout << std::endl;

    // checking if buffer is empty or not
    for (size_t i = 0; i < buffer.size(); i++) {
      if (buffer.at(i) != "") {
        printRow(buffer, c, isHeader);
        break;
      }
    }
  }

  std::vector<std::string> split(std::string str, std::string token) {
    std::vector<std::string>result;
    while(str.size()){
      int index = str.find(token);
      if(index!=std::string::npos){
        result.push_back(str.substr(0,index));
        str = str.substr(index+token.size());
        if(str.size()==0)result.push_back(str);
      }else{
        result.push_back(str);
        str = "";
      }
    }
    return result;
  }

};
}