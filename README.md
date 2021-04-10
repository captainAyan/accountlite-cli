<p align="center">
  <h1 align="center">Account Lite 💡</h1>
  <p align="center">
    Lightweight Double-Entry Accounting Software
    <br />
    <a href="https://github.com/captainAyan/accountlite/issues">Report Bug 😓</a>
  </p>
</p>

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
journal.h | This is a model for 'Journal Entries' (if you don't know what that is, check double-entry system on wikipedia)
main.cpp | Duh 😒
make.bat | This file compiles the program 
parser.h | This file contains two very important things i.e. parse  function and Values class. <br /><br />**Parser**: The parser function takes in an array of strings (the string elements are lines from the book.bk file), parses those strings and stores the data in the *Values* class. <br /><br />**Values**: The instance of Values class contains two 'public properties' (Lol 😂) i.e. metaDataMap, and journalVector. The 'metaDataMap' stores a c++ 'Map' with the meta data values. The 'journalVector' contain vector of Journal (Journal class).
util.h | Contains some miscellaneous utility functions.

## Contribution
Just send me a pull request. Mention your discord or instagram id.

(if the instructions were unclear, please let me know)

## Contact
Send me a message on discord or instagram. Check out my [Profile Readme](https://github.com/captainAyan)