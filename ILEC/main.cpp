//
//  main.cpp
//  ILEC
//
//  Created by User on 16/9/24.
//  Copyright © 2016年 Infinideastudio. All rights reserved.
//

#include <iostream>
#include <vector>
#include <array>
#include <cstdio>
#include <string>
#include <sstream>
#include <cstring>
#include <functional>

const std::string testprog =
std::string("CONSTANRTS 14 48656C6C6F20576F726C64210A00\n") +
std::string("IMPTFUNC 0 OUTPUT\n") +
std::string("STACKSIZE 1024\n") +
std::string("ENTERANCE\n") +
std::string("SETMEM 0 14 0\n") +
std::string("CALL 0 0 0\n") +
std::string("RETURN\n") +
std::string("END\n");

std::unique_ptr<char[]> stack;
std::unique_ptr<unsigned char[]> consts;
std::vector<std::function<void(char*, char*)>> funcs;
std::function<void(char*, char*)> mainf;

inline uint readbits(char c)
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

enum class commands : size_t
{
    IMPTFUNC, DCELFUNC, ENTRANCE, STACKSIZE,
    SETMEM, IF, JUMP, CALL, RETURN,
    ADDi8, ADDi16, ADDi32, ADDi64, ADDu8, ADDu16, ADDu32, ADDu64,
    MINi8, MINi16, MINi32, MINi64, MINu8, MINu16, MINu32, MINu64,
    MULi8, MULi16, MULi32, MULi64, MULu8, MULu16, MULu32, MULu64,
    DIVi8, DIVi16, DIVi32, DIVi64, DIVu8, DIVu16, DIVu32, DIVu64,
};

std::function<void(char*, char*)> getfunction(const std::string& name)
{
    if (name == "INPUT")
        return [](char* base, char*) { scanf("%s", static_cast<char*>(base)); };
    if (name == "OUTPUT")
        return [](char* base, char*) { std::cout << static_cast<char*>(base); };
    return nullptr;
}

inline void arithmetic(commands kind, void* p1, void* p2, void* res)
{
    switch (kind)
    {
        case ADDi8:
            break;
        case ADDi16: ADDi32, ADDi64, ADDu8, ADDu16, ADDu32, ADDu64,
            MINi8, MINi16, MINi32, MINi64, MINu8, MINu16, MINu32, MINu64,
            MULi8, MULi16, MULi32, MULi64, MULu8, MULu16, MULu32, MULu64,
            DIVi8, DIVi16, DIVi32, DIVi64, DIVu8, DIVu16, DIVu32, DIVu64,
        default:
            break;
    }
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

class func
{
public:
    using com = std::array<uint64_t, 4>;
    std::vector<com> bytes;
    void anal(std::stringstream& stream)
    {
        std::string command;
        do
        {
            stream >> command;
            if (command == "CALL")
            {
                uint64_t func, base, _this;
                stream >> func >> base >> _this;
                bytes.push_back({static_cast<uint64_t>(commands::CALL), func, base, _this});
            }
            if (command == "SETMEM")
            {
                uint64_t base, count, offset;
                stream >> base >> count >> offset;
                bytes.push_back({static_cast<uint64_t>(commands::SETMEM), base, count, offset});
            }
            if (command == "JUMP")
            {
                int64_t offset;
                stream >> offset;
                bytes.push_back({static_cast<uint64_t>(commands::JUMP),
                    *reinterpret_cast<uint64_t*>(&offset), 0, 0});
            }
            if (command == "IF")
            {
                uint64_t judge, offtrue, offfalse;
                stream >> judge >> offtrue >> offfalse;
                bytes.push_back({static_cast<uint64_t>(commands::IF), judge, offtrue, offfalse});
            }
            if (command == "RETURN")
            {
                bytes.push_back({static_cast<uint64_t>(commands::RETURN), 0, 0, 0});
            }
        }while (command != "RETURN");
    }
    void operator ()(char* base, char* _this)
    {
        auto begin = bytes.cbegin();
        auto end = bytes.cend();
        for (auto iter = begin; iter < end;)
        {
            const com& cmd = *iter;
            switch (static_cast<commands>(cmd[0]))
            {
                case commands::CALL:
                    funcs[cmd[1]](base + cmd[2], base + cmd[3]);
                    break;
                case commands::SETMEM:
                    memcpy(base + cmd[1], consts.get() + cmd[3], cmd[2]);
                    break;
                case commands::JUMP:
                    iter += (reinterpret_cast<const int64_t*>(cmd.data())[1]);
                    continue;
                case commands::IF:
                    iter += cmd[1] ? cmd[2] : cmd[3];
                    continue;
                case commands::RETURN:
                    return;
                default:
                    break;
            }
            ++iter;
        }
    }
};

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
    return 0;
}
