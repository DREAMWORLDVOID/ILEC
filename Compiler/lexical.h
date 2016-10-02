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
    const string preprocessing_op_or_punc[] = {
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
    const int len_preprocessing_op_or_punc = sizeof(preprocessing_op_or_punc) / sizeof(preprocessing_op_or_punc[0]);
    
    int index_keyword(const char* keyword){
        for (int i=0; i < len_reserved_keyword; ++i)
            if (strcmp(reserved_keyword[i].c_str(),keyword) == 0)
                return i;
        
        for (int i=0; i < len_preprocessing_op_or_punc; ++i)
            if (strcmp(preprocessing_op_or_punc[i].c_str(),keyword) == 0)
                return i + len_reserved_keyword;
        
        return error::not_found; // Error: Not Found
    }
    
    int index_keyword_check_ambiguity(const char* keyword_part){
        int first_result = error::not_found;
        for (int i=0; i < len_reserved_keyword; ++i){
            if (strlen(keyword_part) > reserved_keyword[i].length())
                continue;
            if (strcmp(keyword_part, reserved_keyword[i].substr(0,strlen(keyword_part)).c_str())){
                if (first_result == error::not_found){
                    first_result = i;
                }
                else{
                    return error::ambiguity;
                }
            }
        }
        for (int i=0; i < len_preprocessing_op_or_punc; ++i){
            if (strlen(keyword_part) > preprocessing_op_or_punc[i].length())
                continue;
            if (strcmp(keyword_part, preprocessing_op_or_punc[i].substr(0,strlen(keyword_part)).c_str())){
                if (first_result == error::not_found){
                    first_result = len_reserved_keyword + i;
                }
                else{
                    return error::ambiguity;
                }
            }
        }
        return first_result;
    }
    
    int current_token = -1;
    int current_pos = -1;
    int next_token = -1;
    
    int get_next_token(const string file, bool is_goto){
        if ((next_token != -1) && (!is_goto))
            return next_token;
    
        int i = 1;
        while (file[current_pos + i++] == ' ');
        while (file[current_pos + i++] == '\n');
        if ((file[current_pos + i] == '/') && (file[current_pos + i+1] == '/')) // comment style
            while (file[current_pos + i++] != '\n');
        while (file[current_pos + i++] == ' ');
        while (file[current_pos + i++] == '\n');
        
        if (index_keyword_check_ambiguity(file.substr(current_pos, i).c_str()) > -1)
            next_token = index_keyword(file.substr(current_pos, i).c_str());
        else{
            while (index_keyword_check_ambiguity(file.substr(current_pos, i).c_str()) == error::ambiguity){
                if (index_keyword_check_ambiguity(file.substr(current_pos, i+1).c_str()) == error::not_found){
                    next_token = index_keyword(file.substr(current_pos, i).c_str());
                    break;
                }
                ++i;
            }
            if (index_keyword_check_ambiguity(file.substr(current_pos, i).c_str()) > -1)
                next_token = index_keyword(file.substr(current_pos, i).c_str());
        }
        if (is_goto){
            current_pos += i;
            current_token = next_token;
            next_token = -1;
            return current_token;
        }
        return next_token;
    }
}


#endif /* lexical_h */
