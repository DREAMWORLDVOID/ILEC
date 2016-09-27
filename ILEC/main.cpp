//
//  main.cpp
//  ILEC
//
//  Created by User on 16/9/24.
//  Copyright © 2016年 Infinideastudio. All rights reserved.
//
#include "func.h"
const std::string testprog =
std::string("CONSTANRTS 14 48656C6C6F20576F726C64210000\n") +
std::string("IMPTFUNC 0 OUTPUT\n") +
std::string("STACKSIZE 1024\n") +
std::string("ENTERANCE\n") +
std::string("SETMEM 0 14 0\n") +
std::string("CALL 0 0 0\n") +
std::string("JUMP -1\n") +
std::string("RETURN\n") +
std::string("END\n");

std::unique_ptr<char[]> stack;
std::unique_ptr<unsigned char[]> consts;
std::vector<std::function<void(char*, char*)>> funcs;
std::function<void(char*, char*)> mainf;

inline uint8_t readbits(char c)
{
    switch (c)
    {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return (c - '0');
            break;
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
            return (10 + c - 'A');
            break;
    }
    return 0;
}


std::function<void(char*, char*)> getfunction(const std::string& name)
{
    if (name == "INPUT")
        return [](char* base, char*) { scanf("%s", static_cast<char*>(base)); };
    if (name == "OUTPUT")
        return [](char* base, char*) { std::cout << static_cast<char*>(base); };
    return nullptr;
}

void readconst(std::stringstream& stream)
{
    std::string read;
    size_t length;
    stream >> length >> read;
    auto begin = read.cbegin();
    auto end = read.end();
    consts = std::unique_ptr<unsigned char[]>(new unsigned char[length]);
    auto i = consts.get();
    for (auto iter = begin; iter < end; ++iter)
        *(i++) = readbits(*iter) * 16 + readbits(*(++iter));
}

void anal(std::stringstream& stream)
{
    std::string command;
    do
    {
        stream >> command;
        if (command == "CONSTANRTS")
        {
            readconst(stream);
        }
        if (command == "IMPTFUNC")
        {
            int id;
            std::string name;
            stream >> id >> name;
            if (funcs.size() <= id)
                funcs.resize(id + 1);
            funcs[id] = getfunction(name);
        }
        if (command == "DCELFUNC")
        {
            int id;
            stream >> id;
            if (funcs.size() <= id)
                funcs.resize(id + 1);
            func f;
            f.anal(stream);
            funcs[id] = f;
        }
        if (command == "ENTERANCE")
        {
            func f;
            f.anal(stream);
            mainf = f;
        }
        if (command == "STACKSIZE")
        {
            size_t size;
            stream >> size;
            stack = std::unique_ptr<char[]>(new char[size]);
        }
    }while (command != "END");
}

int main(int argc, const char * argv[])
{
    std::stringstream stream;
    stream << testprog;
    anal(stream);
    mainf(stack.get(), stack.get());
    system("pause");
    return 0;
}
