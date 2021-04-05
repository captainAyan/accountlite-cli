#include <iostream>
#include <ctime>
#include <fstream>
#include "parser.h"
#include "journal.h"
#include "meta.h"

int main()
{

    std::ifstream ifs("./book.bk");
    std::string input;
    input.assign((std::istreambuf_iterator<char>(ifs)),
                (std::istreambuf_iterator<char>()));
    
    Values values(Parser::parse(input));

    values.metaStore.print();

    std::cout << values.journalVector.at(0).getNarration() << std::endl;

    return 0;
}
