#include <iostream>
#include <ctime>
#include <fstream>
#include <map>
#include "parser.h"
#include "journal.h"
#include "statement.h"

int main()
{

  std::ifstream ifs("./book.bk");
  std::string input;
  input.assign((std::istreambuf_iterator<char>(ifs)), 
                (std::istreambuf_iterator<char>()));

  std::vector<Journal> journalList;
  std::map<std::string, std::string> metaDataMap;

  parse(input, &journalList, &metaDataMap);

  // journal entries
  // displayJournalEntries(journalList, metaDataMap["CURRENCY"]);
  // trial balance
  displayTrialBalance(journalList, metaDataMap["CURRENCY"]);

  return EXIT_SUCCESS;
}
