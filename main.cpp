#include <iostream>
#include <ctime>
#include <fstream>
#include <map>
#include "parser.h"
#include "journal.h"
#include "util.h"
#include "table.h"

int main()
{

  std::ifstream ifs("./book.bk");
  std::string input;
  input.assign((std::istreambuf_iterator<char>(ifs)), 
                (std::istreambuf_iterator<char>()));

  Values values(parse(input));

  // trial balance
  std::map<std::string, int> ledgerBalances;
  for(int i=0; i < values.journalVector.size(); i++){
    // handle debit side
    ledgerBalances[values.journalVector[i].getDebit()] += values.journalVector[i].getAmount();
    // handle credit side
    ledgerBalances[values.journalVector[i].getCredit()] -= values.journalVector[i].getAmount();
  }

  // testing the table printer
  std::string h[3] = {"Name", "Topic", "Country"};
  std::string r[2][3] = {{"Samuel", "Coding Memes", "Venezuela"}, {"DevEd", "Web Development", "Germany"}};
  short ha[3] = {Table::LEFT_ALIGN, Table::RIGHT_ALIGN, Table::LEFT_ALIGN};
  short ra[3] = {Table::LEFT_ALIGN, Table::RIGHT_ALIGN, Table::RIGHT_ALIGN};
  
  Table table(3);
  table.setHeadingsAlignment(ha);
  table.setColumnsAlignment(ra);
  table.setPadding(1);
  table.setHeadings(h);
  table.addRow(r[0]);
  table.addRow(r[1]);
  table.draw();

  return EXIT_SUCCESS;
}
