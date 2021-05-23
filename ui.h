#pragma once

#include <iostream>
#include <string>

#include "journal.h"
#include "util.h"

namespace UI {

void addJournalEntry(std::vector<Journal>* _journalList) {
  std::string debit, credit, narration;
  int amount;

  // taking debit account number input
  std::cout << "DEBIT ";
  std::getline(std::cin, debit);
  if(debit == ":exit") return; // check for exit command
  if(!ledgerNameValidator(debit)) {
    std::cout << "Invalid Ledger Name: Cannot include comma in ledger name." << std::endl;
    return;
  }

  // taking credit account number input
  std::cout << "CREDIT ";
  std::getline(std::cin, credit);
  if(credit == ":exit") return; // check for exit command
  else if(!ledgerNameValidator(credit)) {
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
  int id = _journalList->size() + 1;
  
  // adding the journal
  _journalList->push_back(Journal(id, amount, timestampNow(), debit, credit, narration));

  std::cout << "Journal #" <<id<< " Entered" << std::endl;

}

}