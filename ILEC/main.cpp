//
//  main.cpp
//  ILEC
//
//  Created by User on 16/9/24.
//  Copyright © 2016年 Infinideastudio. All rights reserved.
//

#include <iostream>
#include <vector>
#include <cstdio>
#include <string>
#include <sstream>
#include <functional>

const std::string testprog =
std::string("IMPTFUNC 0 INPUT\n") +
std::string("IMPTFUNC 1 OUTPUT\n") +
std::string("STACKSIZE 1024\n") +
std::string("ENTERANCE\n") +
std::string("CALL 2 0 40\n") +
std::string("RETURN\n") +
std::string("DCELFUNC 2") +
std::string("CALL 0 0 40\n") +
std::string("CALL 1 0 40\n") +
std::string("RETURN\n") +
std::string("END\n");

enum class commands : size_t
{
    IMPTFUNC,
    DCELFUNC,
    ENTRANCE,
    STACKSIZE,
    SETMEM,
    CALL,
    RETURN,
};

std::function<void(char*, char*)> getfunction(const std::string& name)
{
    if (name == "INPUT")
        return [](char* arg, char*) { scanf("%s", static_cast<char*>(arg)); };
    if (name == "OUTPUT")
        return [](char* arg, char*) { std::cout << static_cast<char*>(arg); };
    return nullptr;
}

std::vector<std::function<void(char*, char*)>> funcs;
std::unique_ptr<char[]> stack;
std::function<void(char*, char*)> mainf;

class func
{
public:
    std::vector<uint64_t> bytes;
    void anal(std::stringstream& stream)
    {
        std::string command;
        do
        {
            stream >> command;
            if (command == "CALL")
            {
                bytes.push_back(static_cast<uint64_t>(commands::CALL));
                uint64_t func, arg, base;
                stream >> func >> arg >> base;
                bytes.push_back(func);
                bytes.push_back(arg);
                bytes.push_back(base);
            }
            if (command == "RETURN")
            {
                bytes.push_back(static_cast<uint64_t>(commands::RETURN));
            }
        }while (command != "RETURN");
    }
    void operator ()(char* arg, char* base)
    {
        auto begin = bytes.cbegin();
        auto end = bytes.cend();
        for (auto iter = begin; iter < end; ++iter)
        {
            switch (static_cast<commands>(*iter))
            {
                case commands::CALL:
                {
                    auto a = ++iter;
                    auto b = ++iter;
                    auto c = ++iter;
                    funcs[*a](base + *b, base + *c);
                    break;
                }
                case commands::RETURN:
                    return;
                default:
                    break;
            }
        }
    }
};

void anal(std::stringstream& stream)
{
    std::string command;
    do
    {
        stream >> command;
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
    mainf(nullptr, stack.get());
    return 0;
}
