#pragma once

#include <iostream>
#include <string>

namespace ui {

void addJournalEntry(std::vector<Entry>* entryList, std::vector<Ledger>* ledgerList,
  std::vector<Journal>* journalList, std::map<std::string, std::string>* metaDataMap) {

  std::string debit, credit, narration;
  int amount;
  Ledger *debitLedger, *creditLedger;

  // taking debit account number input
  std::cout << "DEBIT ";
  debit = getInput();
  if(debit == EXIT) return; // check for exit command
  debitLedger = ledgerExists(debit, ledgerList);
  if(debitLedger == nullptr) {
    std::cout << "Invalid Ledger: Ledger doesn't exist" << std::endl;
    return;
  }

  // taking credit account number input
  std::cout << "CREDIT ";
  credit = getInput();
  if(credit == EXIT) return; // check for exit command
  creditLedger = ledgerExists(credit, ledgerList);
  if(creditLedger == nullptr) {
    std::cout << "Invalid Ledger: Ledger doesn't exist" << std::endl;
    return;
  }

  // debit and credit cannot have the same account
  if (debit == credit) {
    std::cout << "Invalid Ledger Name: Debit and Credit side cannot have the same ledger name." << std::endl;
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

  int timestamp = timestampNow();

  // adding the journal
  journalList->push_back(Journal(id, amount, timestamp, *debitLedger, *creditLedger, narration));
  entryList->push_back(Entry(id, amount, timestamp, debitLedger->getId(), creditLedger->getId(), narration));

  try {
    fileSave(parser::stringify(entryList, ledgerList, metaDataMap));
    std::cout << "Journal #" <<id<< " Entered" << std::endl;
  }
  catch(const std::exception &ex) {
    std::cout << "Unknown Error: Something went wrong with the database file." << std::endl;
    journalList->pop_back();
    entryList->pop_back();
  }

}

void setupDatabase(std::vector<Entry>* entryList, std::vector<Ledger>* ledgerList,
  std::map<std::string, std::string>* metaDataMap) {

  std::cout << "Setup by filling out the questionnaire." << std::endl;

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
    fileSave(parser::stringify(entryList, ledgerList, metaDataMap));
    std::cout << "Setup Done." << std::endl;
  }
  catch(const std::exception &ex) {
    std::cout << "Unknown Error: Something went wrong with the database file." << std::endl;
  }
}

void viewJournalEntries(std::vector<Journal>* journalList,
  std::map<std::string, std::string>* metaDataMap) {
  std::string from_date_str, to_date_str;
  int from_date_timestamp, to_date_timestamp;

  // suggestion dates
  std::string from_date_suggestion = timestampToString((timestampNow()-(7*24*60*60)));
  std::string to_date_suggestion = timestampToString(timestampNow());

  // starting date
  std::cout << "FROM : <DD/MM/YYYY> [" + from_date_suggestion + "] ";
  from_date_str = getInput();
  if(from_date_str == EXIT) return; // check for exit command
  else if(from_date_str == "") from_date_timestamp = dateStringToTimestamp(from_date_suggestion); // using the suggestion date
  else if(!isValidDateString(from_date_str)) { // check for invalid date format or invalid date
    std::cout << "Invalid Date: Date format should be DD/MM/YYYY and valid." << std::endl;
    return;
  }
  else { // date is valid
    from_date_timestamp = dateStringToTimestamp(from_date_str);
  }

  // ending date
  std::cout << "TO : <DD/MM/YYYY> [" + to_date_suggestion + "] ";
  to_date_str = getInput();
  if(to_date_str == EXIT) return; // check for exit command
  else if(to_date_str == "") to_date_timestamp = dateStringToTimestamp(to_date_suggestion)+(24*60*60)-1; // using the suggestion date
  else if(!isValidDateString(to_date_str)) { // check for invalid date format or invalid date
    std::cout << "Invalid Date: Date format should be DD/MM/YYYY and valid." << std::endl;
    return;
  }
  else { // date is valid
    to_date_timestamp = dateStringToTimestamp(to_date_str);
    to_date_timestamp += (24*60*60)-1; // since the last date needs to be also included
  }

  if (to_date_timestamp < from_date_timestamp) {
    std::cout << "Invalid Date: Ending date is before Starting date." << std::endl;
    return;
  }

  // printing the journals
  statement::journalEntries(journalList, metaDataMap, from_date_timestamp, to_date_timestamp);

}

void viewTrialBalance(std::vector<Journal>* journalList,
  std::map<std::string, std::string>* metaDataMap) {
  std::string as_on_date_str;
  int as_on_date_timestamp;

  std::string as_on_date_suggestion = timestampToString(timestampNow());

  // ending date
  std::cout << "AS ON : <DD/MM/YYYY> [" + as_on_date_suggestion + "] " ;
  as_on_date_str = getInput();
  if(as_on_date_str == EXIT) return; // check for exit command
  else if(as_on_date_str == "") as_on_date_timestamp = dateStringToTimestamp(as_on_date_suggestion)+(24*60*60)-1; // using the suggestion date
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

void viewLedger(std::vector<Journal>* journalList, std::vector<Ledger>* ledgerList,
  std::map<std::string, std::string>* metaDataMap) {
  std::string from_date_str, to_date_str, ledger_name;
  int from_date_timestamp, to_date_timestamp;

  // suggestion dates
  std::string from_date_suggestion = timestampToString((timestampNow()-(7*24*60*60)));
  std::string to_date_suggestion = timestampToString(timestampNow());

  // starting date
  std::cout << "FROM : <DD/MM/YYYY> [" + from_date_suggestion + "] ";
  from_date_str = getInput();
  if(from_date_str == EXIT) return; // check for exit command
  else if(from_date_str == "") from_date_timestamp = dateStringToTimestamp(from_date_suggestion); // using the suggestion date
  else if(!isValidDateString(from_date_str)) { // check for invalid date format or invalid date
    std::cout << "Invalid Date: Date format should be DD/MM/YYYY and valid." << std::endl;
    return;
  }
  else { // date is valid
    from_date_timestamp = dateStringToTimestamp(from_date_str);
  }

  // ending date
  std::cout << "TO : <DD/MM/YYYY> [" + to_date_suggestion + "] ";
  to_date_str = getInput();
  if(to_date_str == EXIT) return; // check for exit command
  else if(to_date_str == "") to_date_timestamp = dateStringToTimestamp(to_date_suggestion)+(24*60*60)-1; // using the suggestion date
  else if(!isValidDateString(to_date_str)) { // check for invalid date format or invalid date
    std::cout << "Invalid Date: Date format should be DD/MM/YYYY and valid." << std::endl;
    return;
  }
  else { // date is valid
    to_date_timestamp = dateStringToTimestamp(to_date_str);
    to_date_timestamp += (24*60*60)-1; // since the last date needs to be also included
  }

  if (to_date_timestamp < from_date_timestamp) {
    std::cout << "Invalid Date: Ending date is before Starting date." << std::endl;
    return;
  }

  // ledger name
  std::cout << "ACCOUNT NAME : ";
  ledger_name = getInput();
  if(ledger_name == EXIT) return; // check for exit command
  if(ledgerExists(ledger_name, ledgerList) == nullptr) {
    std::cout << "Invalid Ledger: Ledger doesn't exist" << std::endl;
    return;
  }

  statement::ledger(journalList, metaDataMap, from_date_timestamp, to_date_timestamp, toLowerCase(ledger_name));
}

void createLedger(std::vector<Entry>* entryList, std::vector<Ledger>* ledgerList,
  std::map<std::string, std::string>* metaDataMap) {
  std::string name, type;

  std::cout << "NAME ";
  name = getInput();
  if(name == EXIT) return; // check for exit command
  if(ledgerExists(name, ledgerList) != nullptr) {
    std::cout << "Invalid Ledger: Ledger already exists" << std::endl;
    return;
  }
  if(name.length() == 0){
    std::cout << "Invalid Ledger: Invalid ledger name" << std::endl;
    return;
  }

  std::cout << std::endl << "REVENUE     - 0" << std::endl;
  std::cout << "EXPENDITURE - 1" << std::endl;
  std::cout << "ASSET       - 2" << std::endl;
  std::cout << "LIABILITY   - 3" << std::endl;
  std::cout << "EQUITY      - 4" << std::endl << std::endl;

  std::cout << "TYPE ";
  type = getInput();
  if(name == EXIT) return; // check for exit command
  if(!(type=="0"||type=="1"||type=="2"||type=="3"||type=="4")) {
    std::cout << "Invalid Ledger: Not a valid ledger type" << std::endl;
    return;
  }

  int id = ledgerList->size() + 1;
  ledgerList->push_back(Ledger(id, std::stoi(type), name));

  try {
    fileSave(parser::stringify(entryList, ledgerList, metaDataMap));
    std::cout << "Ledger #" <<id<< " Created" << std::endl;
  }
  catch(const std::exception &ex) {
    std::cout << "Unknown Error: Something went wrong with the database file." << std::endl;
    ledgerList->pop_back();
  }

}

void viewLedgerList(std::vector<Ledger>* ledgerList) {
  for (size_t i = 0; i < ledgerList->size(); i++)
  {
    Ledger l = ledgerList->at(i);
    std::cout << l.getId() << " " << l.getName();

    switch (l.getType())
    {
      case 0: {
        std::cout << " [REVENUE]" << std::endl;
        break;
      }
      case 1: {
        std::cout << " [EXPENDITURE]" << std::endl;
        break;
      }
      case 2: {
        std::cout << " [ASSET]" << std::endl;
        break;
      }
      case 3: {
        std::cout << " [LIABILITY]" << std::endl;
        break;
      }
      case 4: {
        std::cout << " [EQUITY]" << std::endl;
        break;
      }
    }

  }

}

}