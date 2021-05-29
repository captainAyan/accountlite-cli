#pragma once

#include <iostream>

#include "util.h"
#include "table.h"
#include "journal.h"

namespace statement {

void trialBalance(std::vector<Journal>* journalList, std::string currency) {
  std::map<std::string, int> ledgerBalances;
  for(size_t i=0; i < journalList->size(); i++){
    ledgerBalances[journalList->at(i).getDebit()] += journalList->at(i).getAmount();
    ledgerBalances[journalList->at(i).getCredit()] -= journalList->at(i).getAmount();
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
      r[1] = currency + std::to_string(itr->second);
      r[2] = "-";
    }
    else if (itr->second < 0) {
      int v = itr->second * -1; // since negative values need to converted to positive
      r[1] = "-";
      r[2] = currency + std::to_string(v);
    }
    else continue;
    table.addRow(r);
  }
  table.draw();
}

void journalEntries(std::vector<Journal>* journalList, std::string currency) {
  clitable::Table table;
  short particular_column_size = 30;

  clitable::Column c[4] = {
    clitable::Column("Date", clitable::Column::CENTER_ALIGN, clitable::Column::LEFT_ALIGN, 1,10, clitable::Column::NON_RESIZABLE),
    clitable::Column("Particular", clitable::Column::CENTER_ALIGN, clitable::Column::LEFT_ALIGN, 1,particular_column_size, clitable::Column::NON_RESIZABLE),
    clitable::Column("Debit", clitable::Column::CENTER_ALIGN, clitable::Column::RIGHT_ALIGN, 1,5, clitable::Column::RESIZABLE),
    clitable::Column("Credit", clitable::Column::CENTER_ALIGN, clitable::Column::RIGHT_ALIGN, 1,5, clitable::Column::RESIZABLE)
  };

  for (size_t i = 0; i < 4; i++) table.addColumn(c[i]);

  for (size_t i = 0; i < journalList->size(); i++) {
    std::string particulars = journalList->at(i).getDebit() + " A/c";
    particulars += dots((particular_column_size - journalList->at(i).getDebit().size())-7) + "Dr.";
    particulars += "To. " + journalList->at(i).getCredit() + " A/c\n";
    particulars += "(" + journalList->at(i).getNarration() + ")";

    std::string r[4] {
      timestampToString(journalList->at(i).getTime()),
      particulars,
      currency+std::to_string(journalList->at(i).getAmount()),
      "\n"+currency+std::to_string( journalList->at(i).getAmount())
    };
    table.addRow(r);
  }
  table.draw();
}

void journalEntriesByDate(std::vector<Journal>* journalList, std::string currency, 
  int from_time, int to_time) {

  // filter journal with from_time and to_time
  std::vector<Journal> filteredJournalList;

  // filtering process
  for (size_t i = 0; i < journalList->size(); i++) {
    if (journalList->at(i).getTime() > from_time && 
        journalList->at(i).getTime() < to_time) {
      filteredJournalList.push_back(journalList->at(i));
    }
    if (journalList->at(i).getTime() > to_time) break;    
  }

  // drawing the entries
  journalEntries(&filteredJournalList, currency);

}

void trialBalanceAsOnDate(std::vector<Journal>* journalList, std::string currency,
  int as_on_date) {

  // filter journal with from_time and to_time
  std::vector<Journal> filteredJournalList;

  // filtering process
  for (size_t i = 0; i < journalList->size(); i++) {
    if (journalList->at(i).getTime() < as_on_date) {
      filteredJournalList.push_back(journalList->at(i));
    }
    if (journalList->at(i).getTime() > as_on_date) break;    
  }

  // drawing the trial balance
  trialBalance(&filteredJournalList, currency);
}

}