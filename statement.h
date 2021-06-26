#pragma once

#include <iostream>

#include "util.h"
#include "table.h"
#include "journal.h"

namespace statement {

void trialBalance(std::vector<Journal>* journalList, 
  std::map<std::string, std::string>* metaDataMap, int as_on_date) {

  // ledger balance (override the values)
  std::map<std::string, int> ledgerBalances;

  for (size_t i = 0; i < journalList->size(); i++) {
    if (journalList->at(i).getTime() < as_on_date) {
      ledgerBalances[journalList->at(i).getDebit()] += journalList->at(i).getAmount();
      ledgerBalances[journalList->at(i).getCredit()] -= journalList->at(i).getAmount();
    }
    if (journalList->at(i).getTime() > as_on_date) break;
  }

  std::string currency = (*metaDataMap)["CURRENCY"];
  std::string currencyFormat = (*metaDataMap)["CURRENCY_FORMAT"];
  std::string businessName = (*metaDataMap)["BUSINESS"];

  std::cout << std::endl;

  clitable::Table table;

  table.addTitle("In the books of " + businessName);
  table.addTitle("As on " + timestampToString(as_on_date));

  clitable::Column c[3] = {
    clitable::Column("Particular", clitable::Column::CENTER_ALIGN, clitable::Column::LEFT_ALIGN, 1,24, clitable::Column::NON_RESIZABLE),
    clitable::Column("Debit", clitable::Column::CENTER_ALIGN, clitable::Column::RIGHT_ALIGN, 1,5, clitable::Column::RESIZABLE),
    clitable::Column("Credit", clitable::Column::CENTER_ALIGN, clitable::Column::RIGHT_ALIGN, 1,5, clitable::Column::RESIZABLE)
  };

  for (size_t i = 0; i < 3; i++) table.addColumn(c[i]);

  int total = 0; // total at the end of trial balance
  std::map<std::string, int>::iterator itr;
  for (itr = ledgerBalances.begin(); itr != ledgerBalances.end(); itr++) {
    std::string r[3];
    r[0] = itr->first + " A/c";
    if (itr->second > 0) {
      r[1] = currency + formatCurrency(itr->second, currencyFormat);
      r[2] = "-";
      total += itr->second;
    }
    else if (itr->second < 0) {
      int v = itr->second * -1; // since negative values need to converted to positive
      r[1] = "-";
      r[2] = currency + formatCurrency(v, currencyFormat);
    }
    else continue;
    table.addRow(r);
  }
  std::string r[3] = {"TOTAL",
    currency+formatCurrency(total, currencyFormat),
    currency+formatCurrency(total, currencyFormat)
  };
  table.addRow(r);

  table.draw();
}

void journalEntries(std::vector<Journal>* journalList, 
  std::map<std::string, std::string>* metaDataMap, int from_time, int to_time) {

  clitable::Table table;
  short particular_column_size = 30;

  std::string currency = (*metaDataMap)["CURRENCY"];
  std::string currencyFormat = (*metaDataMap)["CURRENCY_FORMAT"];
  std::string businessName = (*metaDataMap)["BUSINESS"];

  std::cout << std::endl;
  table.addTitle("In the books of " + businessName);

  clitable::Column c[4] = {
    clitable::Column("Date", clitable::Column::CENTER_ALIGN, clitable::Column::LEFT_ALIGN, 1,10, clitable::Column::NON_RESIZABLE),
    clitable::Column("Particular", clitable::Column::CENTER_ALIGN, clitable::Column::LEFT_ALIGN, 1,particular_column_size, clitable::Column::NON_RESIZABLE),
    clitable::Column("Debit", clitable::Column::CENTER_ALIGN, clitable::Column::RIGHT_ALIGN, 1,5, clitable::Column::RESIZABLE),
    clitable::Column("Credit", clitable::Column::CENTER_ALIGN, clitable::Column::RIGHT_ALIGN, 1,5, clitable::Column::RESIZABLE)
  };

  for (size_t i = 0; i < 4; i++) table.addColumn(c[i]);

  for (size_t i = 0; i < journalList->size(); i++) {

    if (journalList->at(i).getTime() > from_time && 
      journalList->at(i).getTime() < to_time) {

      std::string particulars = journalList->at(i).getDebit() + " A/c";
      particulars += dots((particular_column_size - journalList->at(i).getDebit().size())-7) + "Dr.";
      particulars += "To. " + journalList->at(i).getCredit() + " A/c\n";
      particulars += "(" + journalList->at(i).getNarration() + ")";

      std::string r[4] {
        timestampToString(journalList->at(i).getTime()),
        particulars,
        currency+formatCurrency(journalList->at(i).getAmount(), currencyFormat),
        "\n"+currency+formatCurrency( journalList->at(i).getAmount(), currencyFormat)
      };
      table.addRow(r);
    }
    else if(journalList->at(i).getTime() > to_time) break;

  }
  table.draw();
}

void ledger(std::vector<Journal>* journalList, 
  std::map<std::string, std::string>* metaDataMap, int from_time, int to_time, std::string account) {

  int openingBalance=0, closingBalance=0, adjustedBalance=0, total=0;
  std::vector<std::array<std::string, 4>> drRows, crRows;

  std::string currency = (*metaDataMap)["CURRENCY"];
  std::string currencyFormat = (*metaDataMap)["CURRENCY_FORMAT"];
  std::string businessName = (*metaDataMap)["BUSINESS"];

  // first loop  -> goes through all the entries (till starting date) for calculating opening balance
  // second loop -> goes through all the entries (from starting date to ending date) sorting the postings into debit and credit side
  size_t x = 0;
  for (size_t i = 0; i < journalList->size(); i++) {
    // entry is related to given account
    x+=1;
    if (journalList->at(i).getDebit() == account 
      || journalList->at(i).getCredit() == account) {

      // calculation of opening balance
      if (journalList->at(i).getTime() < from_time) {
        if (journalList->at(i).getDebit() == account) openingBalance += journalList->at(i).getAmount();
        else openingBalance -= journalList->at(i).getAmount();
      }
      else {
        // minus 1 because otherwise one entry will get ignored
        if (i > 0) x = i-1;
        else x = 0;
        break;
      }
    }
  }

  // added opening balance
  if (openingBalance > 0) { // debit balance
    std::array<std::string, 4> dr;
    dr[0] = timestampToString(from_time);
    dr[1] = "To.Balance B/D";
    dr[2] = "-";
    dr[3] = currency + formatCurrency(openingBalance, currencyFormat);
    drRows.push_back(dr);
  }
  else if (openingBalance < 0) { // credit balance
    std::array<std::string, 4> cr;
    cr[0] = timestampToString(from_time);
    cr[1] = "By.Balance B/D";
    cr[2] = "-";
    cr[3] = currency + formatCurrency((openingBalance*-1), currencyFormat);
    crRows.push_back(cr);
  }

  for (size_t i = x; i < journalList->size(); i++) {
    if (journalList->at(i).getDebit() == account
      || journalList->at(i).getCredit() == account) { 

      // calculation of closing balance
      if (journalList->at(i).getTime() <= to_time && journalList->at(i).getTime() > from_time) {
        if (journalList->at(i).getDebit() == account) {
          adjustedBalance += journalList->at(i).getAmount();
          total += journalList->at(i).getAmount(); // total needs to keep track of one side
          std::array<std::string, 4> dr;
          dr[0] = timestampToString(journalList->at(i).getTime());
          dr[1] = ("To." + journalList->at(i).getCredit() + " A/c");
          dr[2] = std::to_string(journalList->at(i).getId());
          dr[3] = currency + formatCurrency(journalList->at(i).getAmount(), currencyFormat);
          drRows.push_back(dr);
        }
        else { // if (journalList->at(i).getCredit() == account)
          adjustedBalance -= journalList->at(i).getAmount();
          std::array<std::string, 4> cr;
          cr[0] = timestampToString(journalList->at(i).getTime());
          cr[1] = ("By." + journalList->at(i).getDebit() + " A/c");
          cr[2] = std::to_string(journalList->at(i).getId());
          cr[3] = currency + formatCurrency(journalList->at(i).getAmount(), currencyFormat);
          crRows.push_back(cr);
        }
      }
    }
  }

  // clitable boilerplate (drawing the statement)
  clitable::Table table;
  short particular_column_size = 16;
  std::cout << std::endl;
  table.addTitle("In the books of " + businessName);
  table.addTitle(account + " A/c");
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

  // adjusting to get the actual closing balance
  closingBalance = openingBalance + adjustedBalance;

  // maximum length of the ledger
  size_t maxLen = drRows.size(); // just taking a default value
  if (drRows.size() < crRows.size()) maxLen = crRows.size();

  // added closing balance
  if (closingBalance < 0) { // debit balance
    std::array<std::string, 4> dr;
    dr[0] = timestampToString(to_time);
    dr[1] = "To.Balance C/D";
    dr[2] = "-";
    dr[3] = currency + formatCurrency((closingBalance*-1), currencyFormat);

    // this code is for making sure the balance is the last item
    if (drRows.size() == maxLen) {
      drRows.push_back(dr);
      maxLen += 1;
    }
    else {
      // creating empty rows to make the balance apprear at the end
      for(size_t i=drRows.size(); i<maxLen-1; i++) { // "< maxLen-1" means 2nd last, last one is reserved for the closing entry
        std::array<std::string, 4> drBlank = {"","","",""};
        drRows.push_back(drBlank);
      }
      drRows.push_back(dr);
    }
  }
  else if (closingBalance > 0) { // credit balance
    std::array<std::string, 4> cr;
    cr[0] = timestampToString(to_time);
    cr[1] = "By.Balance C/D";
    cr[2] = "-";
    cr[3] = currency + formatCurrency(closingBalance, currencyFormat);

    // this code is for making sure the balance is the last item
    if (crRows.size() == maxLen) {
      crRows.push_back(cr);
      maxLen += 1;
    }
    else {
      // creating empty rows to make the balance apprear at the end
      for(size_t i=crRows.size(); i<maxLen-1; i++) { // "< maxLen-1" means 2nd last, last one is reserved for the closing entry
        std::array<std::string, 4> crBlank = {"","","",""};
        crRows.push_back(crBlank);
      }
      crRows.push_back(cr);
    }
  }

  // adding all the rows to the table
  for (size_t i=0; i<maxLen; i++) {
    std::string r[8];
    if (drRows.size() > i ) {
      r[0] = drRows.at(i)[0];
      r[1] = drRows.at(i)[1];
      r[2] = drRows.at(i)[2];
      r[3] = drRows.at(i)[3];
    }
    else {r[0]=""; r[1]=""; r[2]=""; r[3]="";} // empty values
    if (crRows.size() > i ) {
      r[4] = crRows.at(i)[0];
      r[5] = crRows.at(i)[1];
      r[6] = crRows.at(i)[2];
      r[7] = crRows.at(i)[3];
    }
    else {r[4]=""; r[5]=""; r[6]=""; r[7]="";} // empty values

    table.addRow(r);
  }

  // adjusting to get the total
  if (openingBalance > 0) total += openingBalance;
  if (closingBalance < 0) total += closingBalance*-1;

  // added the total
  std::string r[8];
  r[0] = "TOTAL";
  r[1] = "-"; r[2] = "-";
  r[3] = currency + formatCurrency(total, currencyFormat);
  r[4] = "TOTAL";
  r[5] = "-"; r[6] = "-";
  r[7] = currency + formatCurrency(total, currencyFormat);
  table.addRow(r);

  table.draw();
}

}