//
//  lexical.h
//  Compiler
//
//  Created by 佐々木みこと on 2016/10/2.
//  Copyright © 2016年 Infinideastudio. All rights reserved.
//

#ifndef lexical_h
#define lexical_h

#include "error.h"
#include <string>
#include <list>
#include <memory>

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
        "final" 
    };
    const int len_reserved_keyword = sizeof(reserved_keyword) / sizeof(reserved_keyword[0]);
    const string b_preprocessing_op_or_punc[] = {
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
    const int len_preprocessing_op_or_punc = sizeof(b_preprocessing_op_or_punc) / sizeof(b_preprocessing_op_or_punc[0]);

    struct lexical_character final
    {
        char set;
        size_t pos;
    };
    namespace preprocessing_token
    {
        enum tokens
        {
            t_header_name,
            t_identifier,
            t_pp_number,
            t_character_literal,
            t_user_defined_character_literal,
            t_string_literal,
            t_user_defined_string_literal,
            t_preprocessing_op_or_punc,
            t_others
        };

        class token
        {
        public:
            tokens type;
            size_t pos, length;
            class physical_file* file;
        };
    }

    class physical_file final
    {
    public:
        std::string m_physical_path, m_name;
        physical_file(std::string physical_path);
        std::list<std::unique_ptr<preprocessing_token::token>> analyze();
    };
}


#endif /* lexical_h */
