#pragma once

#include <iostream>
#include <map>
#include <cstring>
#include <vector>
#include <string>

namespace parser {

void parse(const std::string& input, std::vector<Entry>* entryList,
  std::vector<Ledger>* ledgerList, std::vector<Journal>* journalList,
  std::map<std::string, std::string>* metaDataMap) {

  std::vector<std::string> lines = split(input, '\n');

  std::map<std::string, std::string> _metaDataMap;
  std::vector<Entry> _entries;
  std::vector<Ledger> _ledgers;
  std::vector<Journal> _journals;

  for(int i=0; i<lines.size(); i++) {
    std::string line = lines[i];

    if (line.at(0) == '#') { // meta
      // parsing a META line
      std::vector<std::string> x = split(line, '=');
      std::string name = x.at(0).substr(1, x.at(0).size()-1);
      std::string content = line.substr(name.length()+2, (line.size() - x.at(0).size()));
      _metaDataMap[name] = content;
    }

    else if(line.at(0) == 'E') { // entry
      // parsing a ENTRY line
      std::vector<std::string> e = split(line, ',');

      int id = std::stoi(e.at(0).substr(1, e.at(0).size()-1));
      int amount = std::stoi(e.at(1));
      int time = std::stoi(e.at(2));
      int debitId = std::stoi(e.at(3));
      int creditId = std::stoi(e.at(4));

      /*
      This is getting the last element (narration) from the string. The code
      is little confusing, but it prevents the necessity of creating a parser
      to take care of string escaping [This is also used for the ledger name]
      */
      int l = e.at(0).size()
      +e.at(1).size()
      +e.at(2).size()
      +e.at(3).size()
      +e.at(4).size() + 5;
      std::string narration = line.substr(l, line.size()-l);

      _entries.push_back(Entry(id, amount, time, debitId, creditId, narration));

      /*
      Subtracting 1 from the ID as the 'primary key' starts from 1 by
      index of vector start from 0
      */
      _journals.push_back(Journal(id, amount, time, _ledgers.at(debitId-1),
         _ledgers.at(creditId-1), narration));
    }

    else if(line.at(0) == 'L') { // ledger
      // parsing a LEDGER line
      std::vector<std::string> e = split(line, ',');

      int id = std::stoi(e.at(0).substr(1, e.at(0).size()-1));
      int type = std::stoi(e.at(1));

      int l = e.at(0).size()
      +e.at(1).size() + 2;
      std::string name = line.substr(l, line.size()-l);

      _ledgers.push_back(Ledger(id, type, name));
    }

  }

  *entryList = _entries;
  *ledgerList = _ledgers;
  *journalList = _journals;
  *metaDataMap = _metaDataMap;
}

std::string stringify(std::vector<Entry>* entryList, std::vector<Ledger>* ledgerList,
  std::map<std::string, std::string>* metaDataMap) {
  std::string output = "";

  std::map<std::string, std::string>::iterator itr;
  for (itr = metaDataMap->begin(); itr != metaDataMap->end(); itr++) {
    std::string b = "#";
    b += itr->first;
    b += '=';
    b += itr->second;
    b += '\n';
    output += b;
  }

  for (size_t i = 0; i < ledgerList->size(); i++) {
    output += ledgerList->at(i).stringify();
    output += "\n";
  }

  for (size_t i = 0; i < entryList->size(); i++) {
    output += entryList->at(i).stringify();
    output += "\n";
  }

  return output;
}

}