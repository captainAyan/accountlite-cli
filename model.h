#pragma once

#include <string>

class Entry {
  private:
  int _id, _amount, _time, _debitId, _creditId;
  std::string _narration;

  public:

  Entry(int id, int amount, int time, int debitId, int creditId, const std::string &narration) {
    _id = id;
    _amount = amount;
    _time = time;
    _debitId = debitId;
    _creditId = creditId;
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
  int getDebitId() {
    return _debitId;
  }
  int getCreditId() {
    return _creditId;
  }
  std::string getNarration() {
    return _narration;
  }

  std::string stringify() {
    return "E"+std::to_string(_id)+","+std::to_string(_amount)
    +","+std::to_string(_time)+","+std::to_string(_debitId)+","+
    std::to_string(_creditId)+","+_narration;
  }

};

class Ledger {
  private:
  int _id, _type;
  std::string _name;

  public:
  static const int REVENUE = 0;
  static const int EXPENDITURE = 1;
  static const int ASSET = 2;
  static const int LIABILITY = 3;
  static const int EQUITY = 4;

  Ledger(int id, int type, const std::string &name) {
    _id = id;
    _type = type;
    _name = name;
  }

  int getId() {
    return _id;
  }

  int getType() {
    return _type;
  }

  std::string getName() {
    return _name;
  }

  std::string stringify() {
    return "L"+std::to_string(_id)+","+std::to_string(_type)+","+_name;
  }

};

class Journal {
  private:
  int _id, _amount, _time;
  std::string _narration;
  Ledger _debit, _credit;

  public:

  Journal(int id, int amount, int time, Ledger debit, Ledger credit,
    const std::string &narration ):_debit(debit),_credit(credit) {
    _id = id;
    _amount = amount;
    _time = time;
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
  Ledger getDebit() {
    return _debit;
  }
  Ledger getCredit() {
    return _credit;
  }
  std::string getNarration() {
    return _narration;
  }

};