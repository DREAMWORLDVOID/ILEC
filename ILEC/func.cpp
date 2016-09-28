#include "func.h"

extern std::unique_ptr<char[]> stack;
extern std::unique_ptr<unsigned char[]> consts;
extern std::vector<std::function<void(char*, char*)>> funcs;

void func::anal(std::istream& stream)
{
    std::string command;
    do
    {
        stream >> command;
        if (command == "CALL")
        {
            uint64_t func, base, _this;
            stream >> func >> base >> _this;
            bytes.push_back({ static_cast<uint64_t>(commands::CALL), func, base, _this });
        }
        if (command == "SETMEM")
        {
            uint64_t base, count, offset;
            stream >> base >> count >> offset;
            bytes.push_back({ static_cast<uint64_t>(commands::SETMEM), base, count, offset });
        }
        if (command == "JUMP")
        {
            int64_t offset;
            stream >> offset;
            bytes.push_back({ static_cast<uint64_t>(commands::JUMP),
                *reinterpret_cast<uint64_t*>(&offset), 0, 0 });
        }
        if (command == "IF")
        {
            uint64_t judge, offtrue, offfalse;
            stream >> judge >> offtrue >> offfalse;
            bytes.push_back({ static_cast<uint64_t>(commands::IF), judge, offtrue, offfalse });
        }
        if (command == "RETURN")
        {
            bytes.push_back({ static_cast<uint64_t>(commands::RETURN), 0, 0, 0 });
        }
    } while (command != "RETURN");
}

void func::operator ()(char* base, char* _this)
{
    auto iter = bytes.cbegin();
    for (;;)
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
        case commands::COPYMEM:
            break;
        case commands::AND:
            rb[cmd[3]] = rb[cmd[1]] && rb[cmd[2]];
            break;
        case commands::OR:
            rb[cmd[3]] = rb[cmd[1]] || rb[cmd[2]];
            break;
        case commands::NOT:
            rb[cmd[2]] = !rb[cmd[1]];
            break;
        case commands::AND8:
            ru8[cmd[3]] = ru8[cmd[1]] & ru8[cmd[2]];
            break;
        case commands::AND16:
            ru16[cmd[3]] = ru16[cmd[1]] & ru16[cmd[2]];
            break;
        case commands::AND32:
            ru32[cmd[3]] = ru32[cmd[1]] & ru32[cmd[2]];
            break;
        case commands::AND64:
            ru64[cmd[3]] = ru64[cmd[1]] & ru64[cmd[2]];
            break;
        case commands::OR8:
            ru8[cmd[3]] = ru8[cmd[1]] | ru8[cmd[2]];
            break;
        case commands::OR16:
            ru16[cmd[3]] = ru16[cmd[1]] | ru16[cmd[2]];
            break;
        case commands::OR32:
            ru32[cmd[3]] = ru32[cmd[1]] | ru32[cmd[2]];
            break;
        case commands::OR64:
            ru64[cmd[3]] = ru64[cmd[1]] | ru64[cmd[2]];
            break;
        case commands::XOR8:
            ru8[cmd[3]] = ru8[cmd[1]] ^ ru8[cmd[2]];
            break;
        case commands::XOR16:
            ru16[cmd[3]] = ru16[cmd[1]] ^ ru16[cmd[2]];
            break;
        case commands::XOR32:
            ru32[cmd[3]] = ru32[cmd[1]] ^ ru32[cmd[2]];
            break;
        case commands::XOR64:
            ru64[cmd[3]] = ru64[cmd[1]] ^ ru64[cmd[2]];
            break;
        case commands::NOT8:
            ru8[cmd[2]] = !ru8[cmd[1]];
            break;
        case commands::NOT16:
            ru16[cmd[2]] = !ru16[cmd[1]];
            break;
        case commands::NOT32:
            ru32[cmd[2]] = !ru32[cmd[1]];
            break;
        case commands::NOT64:
            ru64[cmd[2]] = !ru64[cmd[1]];
            break;
        case commands::SHL8:
            ru8[cmd[3]] = ru8[cmd[1]] << ru8[cmd[2]];
            break;
        case commands::SHL16:
            ru16[cmd[3]] = ru16[cmd[1]] << ru8[cmd[2]];
            break;
        case commands::SHL32:
            ru32[cmd[3]] = ru32[cmd[1]] << ru8[cmd[2]];
            break;
        case commands::SHL64:
            ru64[cmd[3]] = ru64[cmd[1]] << ru8[cmd[2]];
            break;
        case commands::SHR8:
            ru8[cmd[3]] = ru8[cmd[1]] >> ru8[cmd[2]];
            break;
        case commands::SHR16:
            ru16[cmd[3]] = ru16[cmd[1]] >> ru8[cmd[2]];
            break;
        case commands::SHR32:
            ru32[cmd[3]] = ru32[cmd[1]] >> ru8[cmd[2]];
            break;
        case commands::SHR64:
            ru64[cmd[3]] = ru64[cmd[1]] >> ru8[cmd[2]];
            break;
        case commands::LESSi8:
            rb[cmd[3]] = rs8[cmd[1]] < rs8[cmd[2]];
            break;
        case commands::LESSi16:
            rb[cmd[3]] = rs16[cmd[1]] < rs16[cmd[2]];
            break;
        case commands::LESSi32:
            rb[cmd[3]] = rs32[cmd[1]] < rs32[cmd[2]];
            break;
        case commands::LESSi64:
            rb[cmd[3]] = rs64[cmd[1]] < rs64[cmd[2]];
            break;
        case commands::LESSu8:
            rb[cmd[3]] = ru8[cmd[1]] < ru8[cmd[2]];
            break;
        case commands::LESSu16:
            rb[cmd[3]] = ru16[cmd[1]] < ru16[cmd[2]];
            break;
        case commands::LESSu32:
            rb[cmd[3]] = ru32[cmd[1]] < ru32[cmd[2]];
            break;
        case commands::LESSu64:
            rb[cmd[3]] = ru64[cmd[1]] < ru64[cmd[2]];
            break;
        case commands::LESSf32:
            rb[cmd[3]] = rf32[cmd[1]] < rf32[cmd[2]];
            break;
        case commands::LESSf64:
            rb[cmd[3]] = rf64[cmd[1]] < rf64[cmd[2]];
            break;
        case commands::MOREi8:
            rb[cmd[3]] = rs8[cmd[1]] > rs8[cmd[2]];
            break;
        case commands::MOREi16:
            rb[cmd[3]] = rs16[cmd[1]] > rs16[cmd[2]];
            break;
        case commands::MOREi32:
            rb[cmd[3]] = rs32[cmd[1]] > rs32[cmd[2]];
            break;
        case commands::MOREi64:
            rb[cmd[3]] = rs64[cmd[1]] > rs64[cmd[2]];
            break;
        case commands::MOREu8:
            rb[cmd[3]] = ru8[cmd[1]] > ru8[cmd[2]];
            break;
        case commands::MOREu16:
            rb[cmd[3]] = ru16[cmd[1]] > ru16[cmd[2]];
            break;
        case commands::MOREu32:
            rb[cmd[3]] = ru32[cmd[1]] > ru32[cmd[2]];
            break;
        case commands::MOREu64:
            rb[cmd[3]] = ru64[cmd[1]] > ru64[cmd[2]];
            break;
        case commands::MOREf32:
            rb[cmd[3]] = rf32[cmd[1]] > rf32[cmd[2]];
            break;
        case commands::MOREf64:
            rb[cmd[3]] = rf64[cmd[1]] > rf64[cmd[2]];
            break;
        case commands::EQULi8:
            rb[cmd[3]] = rs8[cmd[1]] == rs8[cmd[2]];
            break;
        case commands::EQULi16:
            rb[cmd[3]] = rs16[cmd[1]] == rs16[cmd[2]];
            break;
        case commands::EQULi32:
            rb[cmd[3]] = rs32[cmd[1]] == rs32[cmd[2]];
            break;
        case commands::EQULi64:
            rb[cmd[3]] = rs64[cmd[1]] == rs64[cmd[2]];
            break;
        case commands::EQULu8:
            rb[cmd[3]] = ru8[cmd[1]] == ru8[cmd[2]];
            break;
        case commands::EQULu16:
            rb[cmd[3]] = ru16[cmd[1]] == ru16[cmd[2]];
            break;
        case commands::EQULu32:
            rb[cmd[3]] = ru32[cmd[1]] == ru32[cmd[2]];
            break;
        case commands::EQULu64:
            rb[cmd[3]] =
                ru64[cmd[1]] == ru64[cmd[2]];
            break;
        case commands::EQULf32:
            rb[cmd[3]] =
                rf32[cmd[1]] == rf32[cmd[2]];
            break;
        case commands::EQULf64:
            rb[cmd[3]] =
                rf64[cmd[1]] == rf64[cmd[2]];
            break;
        case commands::LESTi8:
            rb[cmd[3]] =
                rs8[cmd[1]] <= rs8[cmd[2]];
            break;
        case commands::LESTi16:
            rb[cmd[3]] =
                rs16[cmd[1]] <= rs16[cmd[2]];
            break;
        case commands::LESTi32:
            rb[cmd[3]] =
                rs32[cmd[1]] <= rs32[cmd[2]];
            break;
        case commands::LESTi64:
            rb[cmd[3]] =
                rs64[cmd[1]] <= rs64[cmd[2]];
            break;
        case commands::LESTu8:
            rb[cmd[3]] =
                ru8[cmd[1]] <= ru8[cmd[2]];
            break;
        case commands::LESTu16:
            rb[cmd[3]] =
                ru16[cmd[1]] <= ru16[cmd[2]];
            break;
        case commands::LESTu32:
            rb[cmd[3]] =
                ru32[cmd[1]] <= ru32[cmd[2]];
            break;
        case commands::LESTu64:
            rb[cmd[3]] =
                ru64[cmd[1]] <= ru64[cmd[2]];
            break;
        case commands::LESTf32:
            rb[cmd[3]] =
                rf32[cmd[1]] <= rf32[cmd[2]];
            break;
        case commands::LESTf64:
            rb[cmd[3]] =
                rf64[cmd[1]] <= rf64[cmd[2]];
            break;
        case commands::MORTi8:
            rb[cmd[3]] =
                rs8[cmd[1]] >= rs8[cmd[2]];
            break;
        case commands::MORTi16:
            rb[cmd[3]] =
                rs16[cmd[1]] >= rs16[cmd[2]];
            break;
        case commands::MORTi32:
            rb[cmd[3]] =
                rs32[cmd[1]] >= rs32[cmd[2]];
            break;
        case commands::MORTi64:
            rb[cmd[3]] =
                rs64[cmd[1]] >= rs64[cmd[2]];
            break;
        case commands::MORTu8:
            rb[cmd[3]] =
                ru8[cmd[1]] >= ru8[cmd[2]];
            break;
        case commands::MORTu16:
            rb[cmd[3]] =
                ru16[cmd[1]] >= ru16[cmd[2]];
            break;
        case commands::MORTu32:
            rb[cmd[3]] =
                ru32[cmd[1]] >= ru32[cmd[2]];
            break;
        case commands::MORTu64:
            rb[cmd[3]] =
                ru64[cmd[1]] >= ru64[cmd[2]];
            break;
        case commands::MORTf32:
            rb[cmd[3]] =
                rf32[cmd[1]] >= rf32[cmd[2]];
            break;
        case commands::MORTf64:
            rb[cmd[3]] =
                rf64[cmd[1]] >= rf64[cmd[2]];
            break;
        case commands::INCi8: 
            ++rs8[cmd[1]];
            break;
        case commands::INCi16:
            ++rs16[cmd[1]];
            break;
        case commands::INCi32:
            ++rs32[cmd[1]];
            break;
        case commands::INCi64:
            ++rs64[cmd[1]];
            break;
        case commands::INCu8:
            ++ru8[cmd[1]];
            break;
        case commands::INCu16:
            ++ru16[cmd[1]];
            break;
        case commands::INCu32:
            ++ru32[cmd[1]];
            break;
        case commands::INCu64:
            ++ru64[cmd[1]];
            break;
        case commands::INCf32:
            ++rf32[cmd[1]];
            break;
        case commands::INCf64:
            ++rf64[cmd[1]];
            break;
        case commands::DECi8:
            --rs8[cmd[1]];
            break;
        case commands::DECi16:
            --rs16[cmd[1]];
            break;
        case commands::DECi32:
            --rs32[cmd[1]];
            break;
        case commands::DECi64:
            --rs64[cmd[1]];
            break;
        case commands::DECu8:
            --ru8[cmd[1]];
            break;
        case commands::DECu16:
            --ru16[cmd[1]];
            break;
        case commands::DECu32:
            --ru32[cmd[1]];
            break;
        case commands::DECu64:
            --ru64[cmd[1]];
            break;
        case commands::DECf32:
            --rf32[cmd[1]];
            break;
        case commands::DECf64:
            --rf64[cmd[1]];
            break;
        case commands::ADDSi8:
            rs8[cmd[1]] += rs8[cmd[2]];
            break;
        case commands::ADDSi16:
            rs16[cmd[1]] += rs16[cmd[2]];
            break;
        case commands::ADDSi32:
            rs32[cmd[1]] += rs32[cmd[2]];
            break;
        case commands::ADDSi64:
            rs64[cmd[1]] += rs64[cmd[2]];
            break;
        case commands::ADDSu8:
            ru8[cmd[1]] += ru8[cmd[2]];
            break;
        case commands::ADDSu16:
            ru16[cmd[1]] += ru16[cmd[2]];
            break;
        case commands::ADDSu32:
            ru32[cmd[1]] += ru32[cmd[2]];
            break;
        case commands::ADDSu64:
            ru64[cmd[1]] += ru64[cmd[2]];
            break;
        case commands::ADDSf32:
            rf32[cmd[1]] += rf32[cmd[2]];
            break;
        case commands::ADDSf64:
            rf64[cmd[1]] += rf64[cmd[2]];
            break;
        case commands::MINSi8:
            rs8[cmd[1]] -= rs8[cmd[2]];
            break;
        case commands::MINSi16:
            rs16[cmd[1]] -= rs16[cmd[2]];
            break;
        case commands::MINSi32:
            rs32[cmd[1]] -= rs32[cmd[2]];
            break;
        case commands::MINSi64:
            rs64[cmd[1]] -= rs64[cmd[2]];
            break;
        case commands::MINSu8:
            ru8[cmd[1]] -= ru8[cmd[2]];
            break;
        case commands::MINSu16:
            ru16[cmd[1]] -= ru16[cmd[2]];
            break;
        case commands::MINSu32:
            ru32[cmd[1]] -= ru32[cmd[2]];
            break;
        case commands::MINSu64:
            ru64[cmd[1]] -= ru64[cmd[2]];
            break;
        case commands::MINSf32:
            rf32[cmd[1]] -= rf32[cmd[2]];
            break;
        case commands::MINSf64:
            rf64[cmd[1]] -= rf64[cmd[2]];
            break;
        case commands::MULSi8:
            rs8[cmd[1]] *= rs8[cmd[2]];
            break;
        case commands::MULSi16:
            rs16[cmd[1]] *= rs16[cmd[2]];
            break;
        case commands::MULSi32:
            rs32[cmd[1]] *= rs32[cmd[2]];
            break;
        case commands::MULSi64:
            rs64[cmd[1]] *= rs64[cmd[2]];
            break;
        case commands::MULSu8:
            ru8[cmd[1]] *= ru8[cmd[2]];
            break;
        case commands::MULSu16:
            ru16[cmd[1]] *= ru16[cmd[2]];
            break;
        case commands::MULSu32:
            ru32[cmd[1]] *= ru32[cmd[2]];
            break;
        case commands::MULSu64:
            ru64[cmd[1]] *= ru64[cmd[2]];
            break;
        case commands::MULSf32:
            rf32[cmd[1]] *= rf32[cmd[2]];
            break;
        case commands::MULSf64:
            rf64[cmd[1]] *= rf64[cmd[2]];
            break;
        case commands::DIVSi8:
            rs8[cmd[1]] /= rs8[cmd[2]];
            break;
        case commands::DIVSi16:
            rs16[cmd[1]] /= rs16[cmd[2]];
            break;
        case commands::DIVSi32:
            rs32[cmd[1]] /= rs32[cmd[2]];
            break;
        case commands::DIVSi64:
            rs64[cmd[1]] /= rs64[cmd[2]];
            break;
        case commands::DIVSu8:
            ru8[cmd[1]] /= ru8[cmd[2]];
            break;
        case commands::DIVSu16:
            ru16[cmd[1]] /= ru16[cmd[2]];
            break;
        case commands::DIVSu32:
            ru32[cmd[1]] /= ru32[cmd[2]];
            break;
        case commands::DIVSu64:
            ru64[cmd[1]] /= ru64[cmd[2]];
            break;
        case commands::DIVSf32:
            rf32[cmd[1]] /= rf32[cmd[2]];
            break;
        case commands::DIVSf64:
            rf64[cmd[1]] /= rf64[cmd[2]];
            break;
        case commands::MODSi8:
            rs8[cmd[1]] %= rs8[cmd[2]];
            break;
        case commands::MODSi16:
            rs16[cmd[1]] %= rs16[cmd[2]];
            break;
        case commands::MODSi32:
            rs32[cmd[1]] %= rs32[cmd[2]];
            break;
        case commands::MODSi64:
            rs64[cmd[1]] %= rs64[cmd[2]];
            break;
        case commands::MODSu8:
            ru8[cmd[1]] %= ru8[cmd[2]];
            break;
        case commands::MODSu16:
            ru16[cmd[1]] %= ru16[cmd[2]];
            break;
        case commands::MODSu32:
            ru32[cmd[1]] %= ru32[cmd[2]];
            break;
        case commands::MODSu64:
            ru64[cmd[1]] %= ru64[cmd[2]];
            break;
        default:
            break;
        }
        ++iter;
    }
}