//
//  lexical.h
//  Compiler
//
//  Created by 佐々木みこと on 2016/10/2.
//  Copyright © 2016年 Infinideastudio. All rights reserved.
//

#ifndef lexical_h
#define lexical_h

using namespace std;

namespace lexical {
    
    const string reserved_keyword[] = {
        "alignas",
        "continue",
        "friend",
        "register",
        "true",
        "alignof",
        "decltype",
        "goto",
        "reinterpret_cast",
        "try",
        "asm",
        "default",
        "if",
        "return",
        "typedef",
        "auto",
        "delete",
        "inline",
        "short",
        "typeid",
        "bool",
        "do",
        "int",
        "signed",
        "typename",
        "break",
        "double",
        "long",
        "sizeof",
        "union",
        "case",
        "dynamic_cast",
        "mutable",
        "static",
        "unsigned",
        "catch",
        "else",
        "namespace",
        "static_assert",
        "using",
        "char",
        "enum",
        "new",
        "static_cast",
        "virtual",
        "char16_t",
        "explicit",
        "noexcept",
        "struct",
        "void",
        "char32_t",
        "export",
        "nullptr",
        "switch",
        "volatile",
        "class",
        "extern",
        "operator",
        "template",
        "wchar_t",
        "const",
        "false",
        "private",
        "this",
        "while",
        "constexpr",
        "float",
        "protected",
        "thread_local",
        "const_cast",
        "for",
        "public",
        "throw",
        "override",
        "final" // override and final are not keywords, but reserved in this case for simplificiation
    };
    const int len_reserved_keyword = sizeof(reserved_keyword) / sizeof(reserved_keyword[0]);
    const string symbol[] = {
        "{","}","[","]","#","##","(",")",
        "<:",":>","<%","%>","%:","%:%:",";",":","...",
        "new","delete","?","::",".",".*",
        "+","-","*","/","%","ˆ","&","|","~",
        "!","=","<",">","+=","-=","*=","/=","%=",
        "ˆ=","&=","|=","<<",">>",">>=","<<=","==","!="
        "<=",">=","&&","||","++","--",",","->*","->"
        "and","and_eq","bitand","bitor","compl","not","not_eq"
        "or","or_eq","xor","xor_eq"
    };
    const int len_symbol = sizeof(symbol) / sizeof(symbol[0]);
    
    int index_keyword(string keyword){
        for (int i=0; i < len_reserved_keyword; ++i)
            if (keyword == reserved_keyword[i])
                return i;
        
        for (int i=0; i < len_symbol; ++i)
            if (keyword == symbol[i])
                return i + len_reserved_keyword;
        
        return -1; // Error: Not Found
    }
    
    int index_keyword_check_ambiguity(string keyword_part){
        
    }
    
    int current_parser = -1;
    
    
}


#endif /* lexical_h */
