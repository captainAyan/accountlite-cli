#include <iostream>
#include <ctime>
#include <map>

#define FILE_NAME "./book.bk"

#define DEBUG

#include "commands.h"
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
    if(input == EXIT) 
      return EXIT_SUCCESS;
    
    // add journal entry
    else if(input == CREATE_JOURNAL) 
      ui::addJournalEntry(&journalList, &metaDataMap);

    // view trial balance "for the period ended on"/"as on date"
    else if(input == VIEW_TRIAL_BALANCE) 
      ui::viewTrialBalance(&journalList, &metaDataMap);
    
    // view journals
    else if(input == VIEW_JOURNALS) 
      statement::journalEntries(&journalList, &metaDataMap, 0, timestampNow());
    
    // view journals using dates
    else if(input == VIEW_JOURNALS_FOR_THE_PERIOD) 
      ui::viewJournalEntries(&journalList, &metaDataMap);

    // view journals using dates
    else if(input == "l") {
      statement::ledger(&journalList, &metaDataMap, 1421113256, timestampNow(), "cash");
    }

    // clear screen
    else if(input == CLEAR_SCREEN) 
      std::system("cls");
    
    // invalid
    else std::cout << "Unknown command." << std::endl;
  }

  return EXIT_SUCCESS;
}
