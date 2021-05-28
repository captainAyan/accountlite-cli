#pragma once

#include <iostream>
#include <string>
#include <fstream>

#include "journal.h"
#include "util.h"

namespace ui {

void addJournalEntry(std::vector<Journal>* journalList, 
  std::map<std::string, std::string>* metaDataMap) {
  
  std::string debit, credit, narration;
  int amount;

  // taking debit account number input
  std::cout << "DEBIT ";
  std::getline(std::cin, debit);
  if(debit == ":exit") return; // check for exit command
  if(!isLedgerNameValidator(debit)) {
    std::cout << "Invalid Ledger Name: Cannot include comma in ledger name." << std::endl;
    return;
  }

  // taking credit account number input
  std::cout << "CREDIT ";
  std::getline(std::cin, credit);
  if(credit == ":exit") return; // check for exit command
  else if(!isLedgerNameValidator(credit)) {
    std::cout << "Invalid Ledger Name: Cannot include comma in ledger name." << std::endl;
    return;
  }

  // taking the amount input
  std::cout << "AMOUNT ";
  std::string a;
  std::getline(std::cin, a);
  if(a == ":exit") return; // check for exit command
  try {
    amount = std::stoi(a); // converting string input into number
  }
  catch(std::invalid_argument e) { // checking for invalid number input
    std::cout << "Invalid Amount: Not a number" << std::endl;
    return;
  }
  catch(std::out_of_range e) { // checking for too big number
    std::cout << "Invalid Amount: Number is too big." << std::endl;
    return;
  }
  if(amount <= 0) { // checking for negative and zero as amount
    std::cout << "Invalid Amount: Cannot be less than or equal to zero" << std::endl;
    return;
  }

  // taking narration input
  std::cout << "NARRATION ";
  std::getline(std::cin, narration);
  if(narration == ":exit") return; // check for exit command

  // calculating the journal id
  int id = journalList->size() + 1;
  
  // adding the journal
  journalList->push_back(Journal(id, amount, timestampNow(), debit, credit, narration));

  try {
    std::ofstream f;
    f.open (FILE_NAME);
    f << parser::stringify(journalList, metaDataMap);
    f.close();
    std::cout << "Journal #" <<id<< " Entered" << std::endl;
  }
  catch(const std::exception &ex) {
    std::cout << "Unknown Error: Something went wrong with the database file." << std::endl;
    journalList->pop_back();
  }

}

void setupDatabase(std::vector<Journal>* journalList, 
  std::map<std::string, std::string>* metaDataMap) {
  
  std::cout << "Setup by filling out the questionair." << std::endl;

  std::map<std::string, std::string> _metaDataMap;
  
  // default presets
  _metaDataMap["CURRENCY"] = "Rs.";
  _metaDataMap["CURRENCY_FORMAT"] = "ind";
  
  // set of questions
  std::string questions[2][2]={ // [question][key in the database]
    {"Business name : ", "BUSINESS"},
    {"Your Name : ", "NAME"}
  };

  // ask questions
  for(size_t i=0; i < std::size(questions); i++) {
    std::cout << questions[i][0];
    std::string a;
    std::getline(std::cin, a);
    if(a == ":exit") i--; // check for exit command
    else {
      _metaDataMap[questions[i][1]] = a;
    }
  }

  *metaDataMap = _metaDataMap;

  try {
    std::ofstream f;
    f.open (FILE_NAME);
    f << parser::stringify(journalList, metaDataMap);
    f.close();
    std::cout << "Done setuping up." << std::endl;
  }
  catch(const std::exception &ex) {
    std::cout << "Unknown Error: Something went wrong with the database file." << std::endl;
  }
}

void viewJournalEntriesWithDateFilter(std::vector<Journal>* journalList, 
  std::map<std::string, std::string>* metaDataMap) {
  std::string from_date_str, to_date_str;
  int from_date_timestamp, to_date_timestamp;

  // starting date
  std::cout << "FROM : <DD/MM/YYYY> ";
  std::getline(std::cin, from_date_str);
  if(from_date_str == ":exit") return; // check for exit command
  if(!isValidDateString(from_date_str)) { // check for invalid date format or invalid date
    std::cout << "Invalid Date: Date format should be DD/MM/YYYY and valid" << std::endl;
    return;
  }
  else { // date is valid
    from_date_timestamp = dateStringToTimestamp(from_date_str);
  }

  // ending date
  std::cout << "TO : <DD/MM/YYYY> ";
  std::getline(std::cin, to_date_str);
  if(to_date_str == ":exit") return; // check for exit command
  if(!isValidDateString(to_date_str)) { // check for invalid date format or invalid date
    std::cout << "Invalid Date: Date format should be DD/MM/YYYY and valid" << std::endl;
    return;
  }
  else { // date is valid
    to_date_timestamp = dateStringToTimestamp(to_date_str);
  }

  // printing the journals
  statement::journalEntriesByDate(journalList, (*metaDataMap)["CURRENCY"], from_date_timestamp, to_date_timestamp);

}

}