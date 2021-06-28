/*

This code is available in this repo 👇
https://github.com/captainAyan/CppCliTable (v1.0.0)

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
  std::vector<std::string> _titles;

  public:
  void addColumn(Column c) {
    _columns.push_back(c);
  }

  void addTitle(std::string h) {
    _titles.push_back(h);
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

  std::string draw() {
    std::string p = "";
    int width=1; // table width
    // just making a list (string list) of headers
    std::vector<std::string> headers;
    for (size_t i = 0; i < _columns.size(); i++) {
      headers.push_back(_columns.at(i).getHeading());
      width += (_columns.at(i).getSize() + 1);
      width += (_columns.at(i).getPadding()*2);
    }

    for (size_t i=0; i<_titles.size(); i++)
      p += printTitle(_titles.at(i), width);

    p += printSeparator('=', _columns);
    p += printRow(headers, _columns, true);
    p += printSeparator('=', _columns);

    for (size_t i=0; i<_rows.size(); i++) {
      p += printRow(_rows.at(i), _columns, false);
      p += printSeparator('-', _columns);
    }
    return p;
  }


  private: 
  std::string printTitle(std::string title, int width) {
    std::string p = "";
    int padding = 0;
    if(title.size() < width) padding = (width - title.size())/2;
    for (size_t i=0; i<padding; i++) p += " ";
    p += title + "\n";
    return p;
  }

  std::string printSeparator(char dash, std::vector<Column> columns) {
    std::string p = "";
    p += '+';
    for (size_t i=0; i<columns.size(); i++) {
      int k = columns.at(i).getSize();
      for(size_t j = 0; j<(k + columns.at(i).getPadding()*2); j++) p += dash;
      p += '+';
    }
    p += "\n";
    return p;
  }

  std::string printRow(std::vector<std::string> v, std::vector<Column> c, bool isHeader) {
    std::string p = "";

    // starting the row
    p += '|';

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
      p += value + "|";
    }
    // ending the row
    p += "\n";

    // checking if buffer is empty or not
    for (size_t i = 0; i < buffer.size(); i++) {
      if (buffer.at(i) != "") {
        p += printRow(buffer, c, isHeader);
        break;
      }
    }
    return p;
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