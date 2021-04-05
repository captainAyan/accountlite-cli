#pragma once
#include<iostream>
#include<vector>

// This can be resolved with use of a hashmap
class MetaData {
  private:
  std::string _name, _content;

  public:
  MetaData(const std::string &name, const std::string &content) {
    _name = name;
    _content = content;
  }

  std::string getName() {
    return _name;
  }

  std::string getContent() {
    return _content;
  }
};

// and this can be resolved with the use of a array of hashmap
class MetaStore {
  private:
  std::vector<MetaData> _metaDataVector;

  public:
  MetaStore(std::vector<MetaData> metaDataVector) {
    _metaDataVector = metaDataVector;
  }

  std::string findMetaData(std::string &name) {
    for (int i=0; i<_metaDataVector.size(); i++) {
      if (_metaDataVector.at(i).getName() == name) {
        return _metaDataVector.at(i).getContent();
      }
    }
    return "";
  }

  void addMetaData(MetaData metaData) {
    _metaDataVector.push_back(metaData);
  }

  void print() { // TODO for debugging
    for(int i=0; i<_metaDataVector.size(); i++) {
      std::cout << "Name: " << _metaDataVector.at(i).getName() << std::endl;
      std::cout << "Content: " << _metaDataVector.at(i).getContent() << std::endl;
      std::cout << "-----------------" << std::endl;
    }
  }

};