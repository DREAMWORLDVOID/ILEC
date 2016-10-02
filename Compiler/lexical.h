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

namespace lexical
{
    const string reserved_keyword[] =
    {
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
        "throw"
    };
    const int len_reserved_keyword = sizeof(reserved_keyword) /sizeof(reserved_keyword[0]);
    const string symbol[] = {"1","2"};
    
    int current_parser = -1;
}


#endif /* lexical_h */