#include <iostream>
#include <ctime>
#include <fstream>
#include <map>

#include "parser.h"
#include "journal.h"
#include "statement.h"
#include "util.h"
#include "ui.h"

int main()
{

  const std::string FILE_NAME = "./book.bk";

  std::vector<Journal> journalList;
  std::map<std::string, std::string> metaDataMap;

  std::ifstream ifs(FILE_NAME);
  std::string input;
  input.assign((std::istreambuf_iterator<char>(ifs)), 
                (std::istreambuf_iterator<char>()));

  parser::parse(input, &journalList, &metaDataMap);

  printCredit();

  // Read–eval–print loop
  while(true) {
    std::string input;
    std::cout << "> ";
    std::getline(std::cin, input);

    // exit
    if(input == ":exit") return EXIT_SUCCESS;
    // add journal entry
    else if(input == "j") {
      UI::addJournalEntry(&journalList);
      std::ofstream f;
      f.open (FILE_NAME);
      f << parser::stringify(&journalList, &metaDataMap);
      f.close();
    }
    // view trial balance
    else if(input == "t") displayTrialBalance(journalList, metaDataMap["CURRENCY"]);
    // view journals
    else if(input == "jd") displayJournalEntries(journalList, metaDataMap["CURRENCY"]);
    // invalid
    else std::cout << "Unknown command" << std::endl;
  }

  return EXIT_SUCCESS;
}
