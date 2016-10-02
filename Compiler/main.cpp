//
//  main.cpp
//  Compiler
//
//  Created by 佐々木みこと on 2016/10/2.
//  Copyright © 2016年 Infinideastudio. All rights reserved.
//

#include <iostream>
#include "lexical.h"
#include "lex_literal.h"
#include "fileio.h"

int main(int argc, const char * argv[]) {
    std::cout << lexical::len_reserved_keyword << std::endl;
    std::string line;
    getline(std::cin, line);
    std::cout << lexical::preprocessing_token::identifier::w(const_cast<char*>(line.data())) - line.data() << std::endl;
    std::cout << IO::read("main.cpp") << std::endl;
    return 0;
}
