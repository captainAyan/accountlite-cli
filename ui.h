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
  debit = getInput();
  if(debit == EXIT) return; // check for exit command
  if(!isLedgerNameValidator(debit)) {
    std::cout << "Invalid Ledger Name: Cannot include comma in ledger name." << std::endl;
    return;
  }
  if(debit.length() == 0) {
    std::cout << "Invalid Ledger Name: Ledger name cannot be empty." << std::endl;
    return;
  }

  // taking credit account number input
  std::cout << "CREDIT ";
  credit = getInput();
  if(credit == EXIT) return; // check for exit command
  if(!isLedgerNameValidator(credit)) {
    std::cout << "Invalid Ledger Name: Cannot include comma in ledger name." << std::endl;
    return;
  }
  if(credit.length() == 0) {
    std::cout << "Invalid Ledger Name: Ledger name cannot be empty." << std::endl;
    return;
  }

  // taking the amount input
  std::cout << "AMOUNT ";
  std::string a = getInput();
  if(a == EXIT) return; // check for exit command
  try {
    amount = std::stoi(a); // converting string input into number
  }
  catch(std::invalid_argument e) { // checking for invalid number input
    std::cout << "Invalid Amount: Not a number." << std::endl;
    return;
  }
  catch(std::out_of_range e) { // checking for too big number
    std::cout << "Invalid Amount: Number is too big." << std::endl;
    return;
  }
  if(amount <= 0) { // checking for negative and zero as amount
    std::cout << "Invalid Amount: Cannot be less than or equal to zero." << std::endl;
    return;
  }

  // taking narration input
  std::cout << "NARRATION ";
  narration = getInput();
  if(narration == EXIT) return; // check for exit command
  if(narration.length() == 0) {
    std::cout << "Invalid Narration: Narration cannot be empty." << std::endl;
    return;
  }

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
    std::string a = getInput();
    if(a == EXIT) i--; // check for exit command
    else if(a.length() == 0) {
      std::cout << "Invalid Data: field cannot be empty." <<std::endl;
    }
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
  from_date_str = getInput();
  if(from_date_str == EXIT) return; // check for exit command
  if(!isValidDateString(from_date_str)) { // check for invalid date format or invalid date
    std::cout << "Invalid Date: Date format should be DD/MM/YYYY and valid." << std::endl;
    return;
  }
  else { // date is valid
    from_date_timestamp = dateStringToTimestamp(from_date_str);
  }

  // ending date
  std::cout << "TO : <DD/MM/YYYY> ";
  to_date_str = getInput();
  if(to_date_str == EXIT) return; // check for exit command
  if(!isValidDateString(to_date_str)) { // check for invalid date format or invalid date
    std::cout << "Invalid Date: Date format should be DD/MM/YYYY and valid." << std::endl;
    return;
  }
  else { // date is valid
    to_date_timestamp = dateStringToTimestamp(to_date_str);
    to_date_timestamp += (24*60*60)-1; // since the last date needs to be also included
  }

  // printing the journals
  statement::journalEntriesByDate(journalList, metaDataMap, from_date_timestamp, to_date_timestamp);

}

void viewTrialBalance(std::vector<Journal>* journalList, 
  std::map<std::string, std::string>* metaDataMap) {
  std::string as_on_date_str;
  int as_on_date_timestamp;

  // ending date
  std::cout << "AS ON : <DD/MM/YYYY>";
  as_on_date_str = getInput();
  if(as_on_date_str == EXIT) return; // check for exit command
  if(as_on_date_str == "") as_on_date_timestamp = timestampNow();
  else if(!isValidDateString(as_on_date_str)) { // check for invalid date format or invalid date
    std::cout << "Invalid Date: Date format should be DD/MM/YYYY and valid." << std::endl;
    return;
  }
  else { // date is valid
    as_on_date_timestamp = dateStringToTimestamp(as_on_date_str);
    as_on_date_timestamp += (24*60*60)-1; // since the last date needs to be also included
  }

  statement::trialBalance(journalList, metaDataMap, as_on_date_timestamp);
}

}