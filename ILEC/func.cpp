#include "func.h"

extern std::unique_ptr<char[]> stack;
extern std::unique_ptr<unsigned char[]> consts;
extern std::vector<std::function<void(char*, char*)>> funcs;

void func::anal(std::stringstream& stream)
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
        case commands::COPYMEM:
            break;
        case commands::AND:
            (*reinterpret_cast<bool*>(base + cmd[3])) =
                (*reinterpret_cast<bool*>(base + cmd[1])) &&
                (*reinterpret_cast<bool*>(base + cmd[2]));
            break;
        case commands::OR:
            (*reinterpret_cast<bool*>(base + cmd[3])) =
                (*reinterpret_cast<bool*>(base + cmd[1])) ||
                (*reinterpret_cast<bool*>(base + cmd[2]));
            break;
        case commands::NOT:
            (*reinterpret_cast<bool*>(base + cmd[2])) =
                !(*reinterpret_cast<bool*>(base + cmd[1]));
            break;
        case commands::AND8:
            (*reinterpret_cast<uint8_t*>(base + cmd[3])) =
                (*reinterpret_cast<uint8_t*>(base + cmd[1])) &
                (*reinterpret_cast<uint8_t*>(base + cmd[2]));
            break;
        case commands::AND16:
            (*reinterpret_cast<uint16_t*>(base + cmd[3])) =
                (*reinterpret_cast<uint16_t*>(base + cmd[1])) &
                (*reinterpret_cast<uint16_t*>(base + cmd[2]));
            break;
        case commands::AND32:
            (*reinterpret_cast<uint32_t*>(base + cmd[3])) =
                (*reinterpret_cast<uint32_t*>(base + cmd[1])) &
                (*reinterpret_cast<uint32_t*>(base + cmd[2]));
            break;
        case commands::AND64:
            (*reinterpret_cast<uint64_t*>(base + cmd[3])) =
                (*reinterpret_cast<uint64_t*>(base + cmd[1])) &
                (*reinterpret_cast<uint64_t*>(base + cmd[2]));
            break;
        case commands::OR8:
            (*reinterpret_cast<uint8_t*>(base + cmd[3])) =
                (*reinterpret_cast<uint8_t*>(base + cmd[1])) |
                (*reinterpret_cast<uint8_t*>(base + cmd[2]));
            break;
        case commands::OR16:
            (*reinterpret_cast<uint16_t*>(base + cmd[3])) =
                (*reinterpret_cast<uint16_t*>(base + cmd[1])) |
                (*reinterpret_cast<uint16_t*>(base + cmd[2]));
            break;
        case commands::OR32:
            (*reinterpret_cast<uint32_t*>(base + cmd[3])) =
                (*reinterpret_cast<uint32_t*>(base + cmd[1])) |
                (*reinterpret_cast<uint32_t*>(base + cmd[2]));
            break;
        case commands::OR64:
            (*reinterpret_cast<uint64_t*>(base + cmd[3])) =
                (*reinterpret_cast<uint64_t*>(base + cmd[1])) |
                (*reinterpret_cast<uint64_t*>(base + cmd[2]));
            break;
        case commands::XOR8:
            (*reinterpret_cast<uint8_t*>(base + cmd[3])) =
                (*reinterpret_cast<uint8_t*>(base + cmd[1])) ^
                (*reinterpret_cast<uint8_t*>(base + cmd[2]));
            break;
        case commands::XOR16:
            (*reinterpret_cast<uint16_t*>(base + cmd[3])) =
                (*reinterpret_cast<uint16_t*>(base + cmd[1])) ^
                (*reinterpret_cast<uint16_t*>(base + cmd[2]));
            break;
        case commands::XOR32:
            (*reinterpret_cast<uint32_t*>(base + cmd[3])) =
                (*reinterpret_cast<uint32_t*>(base + cmd[1])) ^
                (*reinterpret_cast<uint32_t*>(base + cmd[2]));
            break;
        case commands::XOR64:
            (*reinterpret_cast<uint64_t*>(base + cmd[3])) =
                (*reinterpret_cast<uint64_t*>(base + cmd[1])) ^
                (*reinterpret_cast<uint64_t*>(base + cmd[2]));
            break;
        case commands::NOT8:
            (*reinterpret_cast<uint8_t*>(base + cmd[2])) =
                !(*reinterpret_cast<uint8_t*>(base + cmd[1]));
            break;
        case commands::NOT16:
            (*reinterpret_cast<uint16_t*>(base + cmd[2])) =
                !(*reinterpret_cast<uint16_t*>(base + cmd[1]));
            break;
        case commands::NOT32:
            (*reinterpret_cast<uint32_t*>(base + cmd[2])) =
                !(*reinterpret_cast<uint32_t*>(base + cmd[1]));
            break;
        case commands::NOT64:
            (*reinterpret_cast<uint64_t*>(base + cmd[2])) =
                !(*reinterpret_cast<uint64_t*>(base + cmd[1]));
            break;
        case commands::SHL8:
            break;
        case commands::SHL16:
            break;
        case commands::SHL32:
            break;
        case commands::SHL64:
            break;
        case commands::SHR8:
            break;
        case commands::SHR16:
            break;
        case commands::SHR32:
            break;
        case commands::SHR64:
            break;
        case commands::LESSi8:
            break;
        case commands::LESSi16:
            break;
        case commands::LESSi32:
            break;
        case commands::LESSi64:
            break;
        case commands::LESSu8:
            break;
        case commands::LESSu16:
            break;
        case commands::LESSu32:
            break;
        case commands::LESSu64:
            break;
        case commands::LESSf32:
            break;
        case commands::LESSf64:
            break;
        case commands::MOREi8:
            break;
        case commands::MOREi16:
            break;
        case commands::MOREi32:
            break;
        case commands::MOREi64:
            break;
        case commands::MOREu8:
            break;
        case commands::MOREu16:
            break;
        case commands::MOREu32:
            break;
        case commands::MOREu64:
            break;
        case commands::MOREf32:
            break;
        case commands::MOREf64:
            break;
        case commands::EQULi8:
            break;
        case commands::EQULi16:
            break;
        case commands::EQULi32:
            break;
        case commands::EQULi64:
            break;
        case commands::EQULu8:
            break;
        case commands::EQULu16:
            break;
        case commands::EQULu32:
            break;
        case commands::EQULu64:
            break;
        case commands::EQULf32:
            break;
        case commands::EQULf64:
            break;
        case commands::LESTi8:
            break;
        case commands::LESTi16:
            break;
        case commands::LESTi32:
            break;
        case commands::LESTi64:
            break;
        case commands::LESTu8:
            break;
        case commands::LESTu16:
            break;
        case commands::LESTu32:
            break;
        case commands::LESTu64:
            break;
        case commands::LESTf32:
            break;
        case commands::LESTf64:
            break;
        case commands::MORTi8:
            break;
        case commands::MORTi16:
            break;
        case commands::MORTi32:
            break;
        case commands::MORTi64:
            break;
        case commands::MORTu8:
            break;
        case commands::MORTu16:
            break;
        case commands::MORTu32:
            break;
        case commands::MORTu64:
            break;
        case commands::MORTf32:
            break;
        case commands::MORTf64:
            break;
        case commands::INCi8:
            break;
        case commands::INCi16:
            break;
        case commands::INCi32:
            break;
        case commands::INCi64:
            break;
        case commands::INCu8:
            break;
        case commands::INCu16:
            break;
        case commands::INCu32:
            break;
        case commands::INCu64:
            break;
        case commands::INCf32:
            break;
        case commands::INCf64:
            break;
        case commands::DECi8:
            break;
        case commands::DECi16:
            break;
        case commands::DECi32:
            break;
        case commands::DECi64:
            break;
        case commands::DECu8:
            break;
        case commands::DECu16:
            break;
        case commands::DECu32:
            break;
        case commands::DECu64:
            break;
        case commands::DECf32:
            break;
        case commands::DECf64:
            break;
        case commands::ADDi8:
            break;
        case commands::ADDi16:
            break;
        case commands::ADDi32:
            break;
        case commands::ADDi64:
            break;
        case commands::ADDu8:
            break;
        case commands::ADDu16:
            break;
        case commands::ADDu32:
            break;
        case commands::ADDu64:
            break;
        case commands::ADDf32:
            break;
        case commands::ADDf64:
            break;
        case commands::MINi8:
            break;
        case commands::MINi16:
            break;
        case commands::MINi32:
            break;
        case commands::MINi64:
            break;
        case commands::MINu8:
            break;
        case commands::MINu16:
            break;
        case commands::MINu32:
            break;
        case commands::MINu64:
            break;
        case commands::MINf32:
            break;
        case commands::MINf64:
            break;
        case commands::MULi8:
            break;
        case commands::MULi16:
            break;
        case commands::MULi32:
            break;
        case commands::MULi64:
            break;
        case commands::MULu8:
            break;
        case commands::MULu16:
            break;
        case commands::MULu32:
            break;
        case commands::MULu64:
            break;
        case commands::MULf32:
            break;
        case commands::MULf64:
            break;
        case commands::DIVi8:
            break;
        case commands::DIVi16:
            break;
        case commands::DIVi32:
            break;
        case commands::DIVi64:
            break;
        case commands::DIVu8:
            break;
        case commands::DIVu16:
            break;
        case commands::DIVu32:
            break;
        case commands::DIVu64:
            break;
        case commands::DIVf32:
            break;
        case commands::DIVf64:
            break;
        case commands::MODi8:
            break;
        case commands::MODi16:
            break;
        case commands::MODi32:
            break;
        case commands::MODi64:
            break;
        case commands::MODu8:
            break;
        case commands::MODu16:
            break;
        case commands::MODu32:
            break;
        case commands::MODu64:
            break;
        case commands::MODf32:
            break;
        case commands::MODf64:
            break;
        case commands::ADDSi8:
            break;
        case commands::ADDSi16:
            break;
        case commands::ADDSi32:
            break;
        case commands::ADDSi64:
            break;
        case commands::ADDSu8:
            break;
        case commands::ADDSu16:
            break;
        case commands::ADDSu32:
            break;
        case commands::ADDSu64:
            break;
        case commands::ADDSf32:
            break;
        case commands::ADDSf64:
            break;
        case commands::MINSi8:
            break;
        case commands::MINSi16:
            break;
        case commands::MINSi32:
            break;
        case commands::MINSi64:
            break;
        case commands::MINSu8:
            break;
        case commands::MINSu16:
            break;
        case commands::MINSu32:
            break;
        case commands::MINSu64:
            break;
        case commands::MINSf32:
            break;
        case commands::MINSf64:
            break;
        case commands::MULSi8:
            break;
        case commands::MULSi16:
            break;
        case commands::MULSi32:
            break;
        case commands::MULSi64:
            break;
        case commands::MULSu8:
            break;
        case commands::MULSu16:
            break;
        case commands::MULSu32:
            break;
        case commands::MULSu64:
            break;
        case commands::MULSf32:
            break;
        case commands::MULSf64:
            break;
        case commands::DIVSi8:
            break;
        case commands::DIVSi16:
            break;
        case commands::DIVSi32:
            break;
        case commands::DIVSi64:
            break;
        case commands::DIVSu8:
            break;
        case commands::DIVSu16:
            break;
        case commands::DIVSu32:
            break;
        case commands::DIVSu64:
            break;
        case commands::DIVSf32:
            break;
        case commands::DIVSf64:
            break;
        case commands::MODSi8:
            break;
        case commands::MODSi16:
            break;
        case commands::MODSi32:
            break;
        case commands::MODSi64:
            break;
        case commands::MODSu8:
            break;
        case commands::MODSu16:
            break;
        case commands::MODSu32:
            break;
        case commands::MODSu64:
            break;
        case commands::MODSf32:
            break;
        case commands::MODSf64:
            break;
        default:
            break;
        }
        ++iter;
    }
}