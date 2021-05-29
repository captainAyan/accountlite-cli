#include <iostream>
#include <ctime>
#include <map>

#define FILE_NAME "./book.bk"

// #define DEBUG

#include "parser.h"
#include "journal.h"
#include "statement.h"
#include "util.h"
#include "ui.h"

int main()
{

  std::vector<Journal> journalList;
  std::map<std::string, std::string> metaDataMap;

  printCredit();

  std::ifstream ifs(FILE_NAME);
  std::string input;
  std::ifstream infile(FILE_NAME);
  if(!infile.good()) { // first time setup
    ui::setupDatabase(&journalList, &metaDataMap);
  }
  else { // not first time
    input.assign((std::istreambuf_iterator<char>(ifs)), 
                  (std::istreambuf_iterator<char>()));
    parser::parse(input, &journalList, &metaDataMap);
  }

  // Read–eval–print loop
  while(true) {
    std::cout << "> ";
    std::string input = getInput();

    // exit
    if(input == ":exit") return EXIT_SUCCESS;
    // add journal entry
    else if(input == "j") ui::addJournalEntry(&journalList, &metaDataMap);
    // view trial balance
    else if(input == "t") statement::trialBalance(&journalList, metaDataMap["CURRENCY"]);
    // view trial balance "for the period ended on"/"as on date"
    else if(input == "tp") ui::viewTrialBalanceAsOnDate(&journalList, &metaDataMap);
    // view journals
    else if(input == "jd") statement::journalEntries(&journalList, metaDataMap["CURRENCY"]);
    // view journals using dates
    else if(input == "jdf") ui::viewJournalEntriesWithDateFilter(&journalList, &metaDataMap);
    // invalid
    else std::cout << "Unknown command." << std::endl;
  }

  return EXIT_SUCCESS;
}
