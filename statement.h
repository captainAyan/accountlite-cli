#pragma once

#include <iostream>

#include "util.h"
#include "table.h"

namespace statement {

void trialBalance(std::vector<Journal> journalList, std::string currency) {
  std::map<std::string, int> ledgerBalances;
  for(size_t i=0; i < journalList.size(); i++){
    ledgerBalances[journalList[i].getDebit()] += journalList[i].getAmount();
    ledgerBalances[journalList[i].getCredit()] -= journalList[i].getAmount();
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

void journalEntries(std::vector<Journal> journalList, std::string currency) {
  clitable::Table table;
  short particular_column_size = 30;

  clitable::Column c[4] = {
    clitable::Column("Date", clitable::Column::CENTER_ALIGN, clitable::Column::LEFT_ALIGN, 1,10, clitable::Column::NON_RESIZABLE),
    clitable::Column("Particular", clitable::Column::CENTER_ALIGN, clitable::Column::LEFT_ALIGN, 1,particular_column_size, clitable::Column::NON_RESIZABLE),
    clitable::Column("Debit", clitable::Column::CENTER_ALIGN, clitable::Column::RIGHT_ALIGN, 1,5, clitable::Column::RESIZABLE),
    clitable::Column("Credit", clitable::Column::CENTER_ALIGN, clitable::Column::RIGHT_ALIGN, 1,5, clitable::Column::RESIZABLE)
  };

  for (size_t i = 0; i < 4; i++) table.addColumn(c[i]);

  for (size_t i = 0; i < journalList.size(); i++) {
    std::string particulars = journalList[i].getDebit() + " A/c";
    particulars += dots((particular_column_size - journalList[i].getDebit().size())-7) + "Dr.";
    particulars += "To. " + journalList[i].getCredit() + " A/c\n";
    particulars += "(" + journalList[i].getNarration() + ")";

    std::string r[4] {
      timestampToString(journalList[i].getTime()),
      particulars,
      currency+std::to_string(journalList[i].getAmount()),
      "\n"+currency+std::to_string( journalList[i].getAmount())
    };
    table.addRow(r);
  }
  table.draw();
}
}