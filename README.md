<p align="center">
  <h1 align="center">AccountLite CLI 💡</h1>
  <p align="center">
    Lightweight Double-Entry Accounting 💸 Software 🚀
    <br />
    <a href="https://captainayan.github.io/accountlite/cli/#docs">Documentation</a> &nbsp;
    <a href="https://github.com/captainAyan/accountlite-cli/issues">Report Bug 😓</a>
  </p>
</p>

[![Screenshot](https://raw.githubusercontent.com/captainAyan/accountlite-cli/main/screenshots/1.png)](https://github.com/captainAyan/accountlite-cli)

## About

AccountLite CLI is a lightweight accounting software with recording financial transactions (no inventory management available), classification(ledgers), and summarization(trial balance). The data is stored in a human readable format (which is very similar to CSV).

## Getting Started
### As User
Download the `accountlite.exe` binary executable file from the latest [release](https://github.com/captainAyan/accountlite-cli/releases/latest)

### As Developer
⚠ You must have [MinGW](https://sourceforge.net/projects/mingw/) installed and setup on your computer. Check by typing `g++ --h` in your command line.

#### Windows
1. Clone the repo and enter the folder
2. Run `make.bat` or `./make.bat`

#### Linux & Mac
1. Clone the repo and enter the folder
2. Run `make.sh` or `./make.sh`

## Documentation (For User)
Documentation is available [here](https://captainayan.github.io/accountlite/cli/#docs)

## File Description (For Developer)
File Name | Description
-|-|
book.bk | This is our own format of storing the data.
commands.h | This file contains all the valid commands
model.h | This is a model for Journal, Ledger, and Entry
main.cpp | Duh 😒
make.bat | This file compiles the program for windows
<span>make</span>.sh | This file compiles the program for mac and linux
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
