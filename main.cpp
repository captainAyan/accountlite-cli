#include <iostream>
#include <map>

#define FILE_NAME "./book.bk"

#define DEBUG

#include "commands.h"
#include "model.h"
#include "util.h"
#include "parser.h"
#include "table.h"
#include "statement.h"
#include "ui.h"

int main()
{

  std::vector<Journal> journalList;

  std::vector<Entry> entryList;
  std::vector<Ledger> ledgerList;
  std::map<std::string, std::string> metaDataMap;

  printCredit();

  std::ifstream ifs(FILE_NAME);
  std::string input;
  std::ifstream infile(FILE_NAME);
  if(!infile.good()) { // first time setup
    ui::setupDatabase(&entryList, &ledgerList, &metaDataMap);
  }
  else { // not first time
    input.assign((std::istreambuf_iterator<char>(ifs)),
                  (std::istreambuf_iterator<char>()));
    // parser::parse(input, &journalList, &metaDataMap);
    parser::parse(input, &entryList, &ledgerList, &journalList, &metaDataMap);
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
      ui::addJournalEntry(&entryList, &ledgerList, &journalList, &metaDataMap);

    // view journals
    else if(input == VIEW_JOURNALS)
      statement::journalEntries(&journalList, &metaDataMap, 0, timestampNow());

    // view journals using dates
    else if(input == VIEW_JOURNALS_FOR_THE_PERIOD)
      ui::viewJournalEntries(&journalList, &metaDataMap);

    // view trial balance "for the period ended on"/"as on date"
    else if(input == VIEW_TRIAL_BALANCE)
      ui::viewTrialBalance(&journalList, &metaDataMap);

    // create ledger
    else if(input == CREATE_LEDGER)
      ui::createLedger(&entryList, &ledgerList, &metaDataMap);

    // view ledger using dates
    else if(input == VIEW_LEDGER)
      ui::viewLedger(&journalList, &ledgerList, &metaDataMap);

    // view list of ledgers
    else if(input == VIEW_LEDGER_LIST)
      ui::viewLedgerList(&ledgerList);

    // clear screen
    else if (input == CLEAR_SCREEN) {
      clearScreen();
    }

    // invalid
    else std::cout << "Unknown command." << std::endl;
  }

  return EXIT_SUCCESS;
}
