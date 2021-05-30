#pragma once

#include <iostream>

#include "util.h"
#include "table.h"
#include "journal.h"

namespace statement {

void trialBalance(std::vector<Journal>* journalList, 
  std::map<std::string, std::string>* metaDataMap, int as_on_date) {

  // filter journal with as_on_date
  std::vector<Journal> filteredJournalList;

  // if this condition is true, then filtering is not required (for optimization)
  if(as_on_date > journalList->at(journalList->size()-1).getTime()) {
    filteredJournalList = *journalList;
  }
  else {
    // filtering process
    for (size_t i = 0; i < journalList->size(); i++) {
      if (journalList->at(i).getTime() < as_on_date) {
        filteredJournalList.push_back(journalList->at(i));
      }
      if (journalList->at(i).getTime() > as_on_date) break;
    }
  }
  

  std::string currency = (*metaDataMap)["CURRENCY"];
  std::string currencyFormat = (*metaDataMap)["CURRENCY_FORMAT"];
  std::string businessName = (*metaDataMap)["BUSINESS"];

  std::cout << std::endl << "In the books of " << businessName << std::endl;
  std::cout << "As on " << timestampToString(as_on_date) << std::endl;

  std::map<std::string, int> ledgerBalances;
  for(size_t i=0; i < filteredJournalList.size(); i++){
    ledgerBalances[filteredJournalList.at(i).getDebit()] += filteredJournalList.at(i).getAmount();
    ledgerBalances[filteredJournalList.at(i).getCredit()] -= filteredJournalList.at(i).getAmount();
  }

  clitable::Table table;
  clitable::Column c[3] = {
    clitable::Column("Particular", clitable::Column::CENTER_ALIGN, clitable::Column::LEFT_ALIGN, 1,24, clitable::Column::NON_RESIZABLE),
    clitable::Column("Debit", clitable::Column::CENTER_ALIGN, clitable::Column::RIGHT_ALIGN, 1,5, clitable::Column::RESIZABLE),
    clitable::Column("Credit", clitable::Column::CENTER_ALIGN, clitable::Column::RIGHT_ALIGN, 1,5, clitable::Column::RESIZABLE)
  };

  for (size_t i = 0; i < 3; i++) table.addColumn(c[i]);

  std::map<std::string, int>::iterator itr;
  for (itr = ledgerBalances.begin(); itr != ledgerBalances.end(); itr++) {
    std::string r[3];
    r[0] = itr->first + " A/c";
    if (itr->second > 0) {
      r[1] = currency + formatCurrency(itr->second, currencyFormat);
      r[2] = "-";
    }
    else if (itr->second < 0) {
      int v = itr->second * -1; // since negative values need to converted to positive
      r[1] = "-";
      r[2] = currency + formatCurrency(v, currencyFormat);
    }
    else continue;
    table.addRow(r);
  }
  table.draw();
}

void journalEntries(std::vector<Journal>* journalList, 
  std::map<std::string, std::string>* metaDataMap, int from_time, int to_time) {

  // filter journal with from_time and to_time
  std::vector<Journal> filteredJournalList;

  // if this condition is true, then filtering is not required (for optimization)
  if(from_time == 0 && 
    to_time > journalList->at(journalList->size()-1).getTime()) {
    filteredJournalList = *journalList;
  }
  else {
    // filtering process
    for (size_t i = 0; i < journalList->size(); i++) {
      if (journalList->at(i).getTime() > from_time && 
          journalList->at(i).getTime() < to_time) {
        filteredJournalList.push_back(journalList->at(i));
      }
      if (journalList->at(i).getTime() > to_time) break;
    }
  }
  
  clitable::Table table;
  short particular_column_size = 30;

  std::string currency = (*metaDataMap)["CURRENCY"];
  std::string currencyFormat = (*metaDataMap)["CURRENCY_FORMAT"];
  std::string businessName = (*metaDataMap)["BUSINESS"];

  std::cout << std::endl << "In the books of " << businessName <<std::endl;

  clitable::Column c[4] = {
    clitable::Column("Date", clitable::Column::CENTER_ALIGN, clitable::Column::LEFT_ALIGN, 1,10, clitable::Column::NON_RESIZABLE),
    clitable::Column("Particular", clitable::Column::CENTER_ALIGN, clitable::Column::LEFT_ALIGN, 1,particular_column_size, clitable::Column::NON_RESIZABLE),
    clitable::Column("Debit", clitable::Column::CENTER_ALIGN, clitable::Column::RIGHT_ALIGN, 1,5, clitable::Column::RESIZABLE),
    clitable::Column("Credit", clitable::Column::CENTER_ALIGN, clitable::Column::RIGHT_ALIGN, 1,5, clitable::Column::RESIZABLE)
  };

  for (size_t i = 0; i < 4; i++) table.addColumn(c[i]);

  for (size_t i = 0; i < filteredJournalList.size(); i++) {
    std::string particulars = filteredJournalList.at(i).getDebit() + " A/c";
    particulars += dots((particular_column_size - filteredJournalList.at(i).getDebit().size())-7) + "Dr.";
    particulars += "To. " + filteredJournalList.at(i).getCredit() + " A/c\n";
    particulars += "(" + filteredJournalList.at(i).getNarration() + ")";

    std::string r[4] {
      timestampToString(filteredJournalList.at(i).getTime()),
      particulars,
      currency+formatCurrency(filteredJournalList.at(i).getAmount(), currencyFormat),
      "\n"+currency+formatCurrency( filteredJournalList.at(i).getAmount(), currencyFormat)
    };
    table.addRow(r);
  }
  table.draw();
}

}