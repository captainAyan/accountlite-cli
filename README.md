<p align="center">
  <h1 align="center">Account Lite 💡</h1>
  <p align="center">
    Lightweight Double-Entry Accounting 💸 Software 🚀
    <br />
    <a href="https://github.com/captainAyan/accountlite/issues">Report Bug 😓</a>
  </p>
</p>

[![Screenshot](https://raw.githubusercontent.com/captainAyan/accountlite/main/screenshots/1.png)](https://github.com/captainAyan/accountlite)

## About

Account Lite is a lightweight accounting software with recording financial transactions (no inventory management available), classification(ledgers), and summarization(trial balance). The data is stored in a human readable format (which is very similar to CSV).

## Getting Started
### As User
_Coming Soon_ 😁

### As Developer
#### Windows
1. Clone the repo and enter the folder
2. Run `make.bat` or `./make.bat`
#### Linux & Mac
_Coming Soon_ 😁

## File Description
File Name | Description
-|-|
book.bk | This is our own format of storing the data. the values starting with Hash sign (#) are meta data, and the others are journal entries.
commands.h | This file contains all the valid commands
journal.h | This is a model for 'Journal Entries' (if you don't know what that is, check double-entry system on wikipedia)
main.cpp | Duh 😒
make.bat | This file compiles the program 
parser.h | This file contains `parser` namespace, which is there to help 'parse' and 'stringify' the data of the ***book.bk*** file.
statement.h | This file contains methods for printing out statements (In this context statements also mean Journal, Ledger, and Trial Balance, although journal and ledger are not actually statements)
table.h | Check out [CppCliTable](https://github.com/captainAyan/CppCliTable)
ui.h | This file contains methods for complex or multi-step interactions.
util.h | Contains some miscellaneous utility functions.

## Contribution
Just send me a pull request. Mention your discord or instagram id.

(if the instructions were unclear, please let me know)

## Contact
Send me a message on discord or instagram. Check out my [Profile Readme](https://github.com/captainAyan)