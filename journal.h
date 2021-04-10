#pragma once

#include <string>

class Journal {
  private: 
  int _id, _amount, _time;
  std::string _debit, _credit, _narration;

  public:

  Journal(int id, 
  int amount, 
  int time,
  const std::string &debit, 
  const std::string &credit, 
  const std::string &narration) {
    _id = id;
    _amount = amount;
    _time = time;
    _debit = debit;
    _credit = credit;
    _narration = narration;
  }

  int getId() {
    return _id;
  };
  int getAmount() {
    return _amount;
  }
  int getTime() {
    return _time;
  }
  std::string getDebit() {
    return _debit;
  }
  std::string getCredit() {
    return _credit;
  }
  std::string getNarration() {
    return _narration;
  }

};