#pragma once

#include <iostream>
#include <cstring>
#include <vector>
#include <string>
#include "meta.h"
#include "util.h"
#include "journal.h"

class Values {
  public:
  MetaStore metaStore;
  std::vector<Journal> journalVector;

  Values():metaStore(MetaStore(std::vector<MetaData>())), journalVector(std::vector<Journal>()) {}
  Values(MetaStore m, std::vector<Journal> j):metaStore(m), journalVector(j) {}
};

class Parser {
  private: Parser(){}
  public:
  static Values parse(std::string input) {
    std::vector<std::string> lines = split(input, '\n');
    std::vector<MetaData> metaDataVector;
    std::vector<Journal> journals;

    for(int i=0; i<lines.size(); i++) {
      if (lines.at(i).at(0) == '#') { // meta
        std::vector<std::string> x = split(lines.at(i), '=');
        std::string name = x.at(0).substr(1, x.at(0).size()-1);
        std::string content = lines.at(i).substr(name.length()+2, (lines.at(i).size() - x.at(0).size()));

        metaDataVector.push_back(MetaData(name, content));
      }

      else { // journal
        std::vector<std::string> e = split(lines.at(i), ',');

        int id = std::stoi(e.at(0));
        int amount = std::stoi(e.at(1));
        int time = std::stoi(e.at(2));
        std::string debit = e.at(3);
        std::string credit = e.at(4);

        /*
        This is getting the last element (narration) from the string. The code 
        is little confusing, but it prevents the necessity of creating a parser 
        to take care of string escaping 
        */
        int l = e.at(0).size()
        +e.at(1).size()
        +e.at(2).size()
        +e.at(3).size()
        +e.at(4).size() + 5;
        std::string narration = lines.at(i).substr(l, lines.at(i).size()-l);

        journals.push_back(Journal(id, amount, time, debit, credit, narration));
      }
    }
    MetaStore metaStore(metaDataVector);
    return Values(metaStore, journals);
  }
};