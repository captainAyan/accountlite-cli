#include <iostream>
#include <ctime>
#include <fstream>
#include "parser.h"
#include "journal.h"
#include "util.h"

int main()
{

    std::ifstream ifs("./book.bk");
    std::string input;
    input.assign((std::istreambuf_iterator<char>(ifs)),
                (std::istreambuf_iterator<char>()));
    
    Values values(parse(input));
    LOG(values.journalVector.at(0).getNarration());

    return 0;
}
