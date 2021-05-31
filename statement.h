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

void ledger(std::vector<Journal>* journalList, 
  std::map<std::string, std::string>* metaDataMap, int from_time, int to_time, std::string account) {

  int openingBalance=0, closingBalance=0, adjustedBalance=0, total=0;
  std::vector<Journal> debitSidePostings, creditSidePostings;

  std::string currency = (*metaDataMap)["CURRENCY"];
  std::string currencyFormat = (*metaDataMap)["CURRENCY_FORMAT"];
  std::string businessName = (*metaDataMap)["BUSINESS"];

  for (size_t i = 0; i < journalList->size(); i++) {

    // entry is related to given account
    if (journalList->at(i).getDebit() == account 
      || journalList->at(i).getCredit() == account) { 

      // create check for entries like cash A/c to cash A/c (these entries have no effect on the outcome)
      if (journalList->at(i).getDebit() == journalList->at(i).getCredit()) continue;

      // calculation of opening balance
      if (journalList->at(i).getTime() < from_time) {
        if (journalList->at(i).getDebit() == account) {
          openingBalance += journalList->at(i).getAmount();
        }
        else { // if (journalList->at(i).getCredit() == account)
          openingBalance -= journalList->at(i).getAmount();
        }
      }
      // calculation of closing balance
      else if (journalList->at(i).getTime() <= to_time) {
        if (journalList->at(i).getDebit() == account) {
          adjustedBalance += journalList->at(i).getAmount();
          debitSidePostings.push_back(journalList->at(i));
        }
        else { // if (journalList->at(i).getCredit() == account)
          adjustedBalance -= journalList->at(i).getAmount();
          creditSidePostings.push_back(journalList->at(i));
        }
      }
      else break;
    }
  }

  // adjusting to get the actual closing balance
  closingBalance = openingBalance + adjustedBalance;
  // adjusting to get the total
  // total = openingBalance + adjustedBalance*-1;

  // drawing the statement
  clitable::Table table;
  short particular_column_size = 16;

  std::cout << std::endl << "In the books of " << businessName <<std::endl;
  std::cout << account << " A/c" << std::endl;

  clitable::Column c[8] = {
    clitable::Column("Date", clitable::Column::CENTER_ALIGN, clitable::Column::LEFT_ALIGN, 1,10, clitable::Column::NON_RESIZABLE),
    clitable::Column("Particular", clitable::Column::CENTER_ALIGN, clitable::Column::LEFT_ALIGN, 1,particular_column_size, clitable::Column::NON_RESIZABLE),
    clitable::Column("F", clitable::Column::CENTER_ALIGN, clitable::Column::RIGHT_ALIGN, 0,0, clitable::Column::RESIZABLE),
    clitable::Column("Amount", clitable::Column::CENTER_ALIGN, clitable::Column::RIGHT_ALIGN, 1,5, clitable::Column::RESIZABLE),
    clitable::Column("Date", clitable::Column::CENTER_ALIGN, clitable::Column::LEFT_ALIGN, 1,10, clitable::Column::NON_RESIZABLE),
    clitable::Column("Particular", clitable::Column::CENTER_ALIGN, clitable::Column::LEFT_ALIGN, 1,particular_column_size, clitable::Column::NON_RESIZABLE),
    clitable::Column("F", clitable::Column::CENTER_ALIGN, clitable::Column::RIGHT_ALIGN, 0,0, clitable::Column::RESIZABLE),
    clitable::Column("Amount", clitable::Column::CENTER_ALIGN, clitable::Column::RIGHT_ALIGN, 1,5, clitable::Column::RESIZABLE)
  };

  for (size_t i = 0; i < 8; i++) table.addColumn(c[i]);

  int maxLen;
  {
    int dr=debitSidePostings.size(), cr=creditSidePostings.size();
    if (openingBalance > 0) dr += 1; // opening balance is debit
    if (openingBalance < 0) cr += 1; // opening balance is credit
    if (closingBalance < 0) dr += 1; // closing balance is debit
    if (closingBalance > 0) cr += 1; // closing balance is credit

    if(dr > cr) maxLen = dr; // debit side has more posting
    else if(dr < cr) maxLen = cr; // credit side has more posting
    else maxLen = dr; // doesn't matter, both are some (dr == cr)
  }

  for (size_t i = 0; i < maxLen; i++) {
    std::string r[8];

    if(i == 0) { // for printing the opening balance
      if(openingBalance > 0) { // debit balance
        r[0] = timestampToString(from_time);
        r[1] = "To.balance B/D";
        r[2] = "-";
        r[3] = currency + formatCurrency(openingBalance, currencyFormat);

        if(creditSidePostings.size() != 0) { // if there are postings
          r[4] = timestampToString(creditSidePostings.at(0).getTime());
          r[5] = "By." + creditSidePostings.at(0).getDebit() + " A/c";
          r[6] = std::to_string(creditSidePostings.at(0).getId());
          r[7] = currency + formatCurrency(creditSidePostings.at(0).getAmount(), currencyFormat);

          creditSidePostings.erase(creditSidePostings.begin()); // removing the first element
        }
        else { // if there are no more posting left
          r[4] = ""; r[5] = ""; r[6] = ""; r[7] = "";
        }

        table.addRow(r);
      }
      else if(openingBalance < 0) { // credit balance
        if(debitSidePostings.size() != 0) { // if there are postings
          r[0] = timestampToString(debitSidePostings.at(0).getTime());
          r[1] = "To." + debitSidePostings.at(0).getCredit() + " A/c";
          r[2] = std::to_string(debitSidePostings.at(0).getId());
          r[3] = currency + formatCurrency(debitSidePostings.at(0).getAmount(), currencyFormat);

          debitSidePostings.erase(debitSidePostings.begin()); // removing the first element
        }
        else { // if there are no more posting left
          r[0] = ""; r[1] = ""; r[2] = ""; r[3] = "";
        }

        r[4] = timestampToString(from_time);
        r[5] = "By.Balance B/D";
        r[6] = "-";
        r[7] = currency + formatCurrency((openingBalance * -1), currencyFormat);

        table.addRow(r);
      }
    }

    else if(i == maxLen-1) {
      if(closingBalance < 0) { // debit balance
        r[0] = timestampToString(to_time);
        r[1] = "To.balance C/D";
        r[2] = "-";
        r[3] = currency + formatCurrency((closingBalance * -1), currencyFormat);

        if(creditSidePostings.size() != 0) { // if there are postings
          r[4] = timestampToString(creditSidePostings.at(0).getTime());
          r[5] = "By." + creditSidePostings.at(0).getDebit() + " A/c";
          r[6] = std::to_string(creditSidePostings.at(0).getId());
          r[7] = currency + formatCurrency(creditSidePostings.at(0).getAmount(), currencyFormat);

          creditSidePostings.erase(creditSidePostings.begin()); // removing the first element
        }
        else { // if there are no more posting left
          r[4] = ""; r[5] = ""; r[6] = ""; r[7] = "";
        }
        table.addRow(r);
      }
      else if(closingBalance > 0) { // credit balance
        if(debitSidePostings.size() != 0) { // if there are postings
          r[0] = timestampToString(debitSidePostings.at(0).getTime());
          r[1] = "To." + debitSidePostings.at(0).getCredit() + " A/c";
          r[2] = std::to_string(debitSidePostings.at(0).getId());
          r[3] = currency + formatCurrency(debitSidePostings.at(0).getAmount(), currencyFormat);

          debitSidePostings.erase(debitSidePostings.begin()); // removing the first element
        }
        else { // if there are no more posting left
          r[0] = ""; r[1] = ""; r[2] = ""; r[3] = "";
        }

        r[4] = timestampToString(to_time);
        r[5] = "By.Balance C/D";
        r[6] = "-";
        r[7] = currency + formatCurrency(closingBalance, currencyFormat);
        table.addRow(r);
      }
    }

    else if (i != 0) { // all the other postings
      if(debitSidePostings.size() != 0) { // if there are postings
        r[0] = timestampToString(debitSidePostings.at(0).getTime());
        r[1] = "To." + debitSidePostings.at(0).getCredit() + " A/c";
        r[2] = std::to_string(debitSidePostings.at(0).getId());
        r[3] = currency + formatCurrency(debitSidePostings.at(0).getAmount(), currencyFormat);

        debitSidePostings.erase(debitSidePostings.begin()); // removing the first element
      }
      else { // if there are no more posting left
        r[0] = ""; r[1] = ""; r[2] = ""; r[3] = "";
      }

      // printing the credit side
      if(creditSidePostings.size() != 0) { // if there are postings
        r[4] = timestampToString(creditSidePostings.at(0).getTime());
        r[5] = "By." + creditSidePostings.at(0).getDebit() + " A/c";
        r[6] = std::to_string(creditSidePostings.at(0).getId());
        r[7] = currency + formatCurrency(creditSidePostings.at(0).getAmount(), currencyFormat);

        creditSidePostings.erase(creditSidePostings.begin()); // removing the first element
      }
      else { // if there are no more posting left
        r[4] = ""; r[5] = ""; r[6] = ""; r[7] = "";
      }

      table.addRow(r);
    }
  }

  table.draw();
  LOG(openingBalance);
  LOG(closingBalance);
  LOG(adjustedBalance);
  LOG(total);
}

}