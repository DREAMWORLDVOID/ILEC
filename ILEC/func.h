#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <memory>
#include <cstdio>
#include <string>
#include <sstream>
#include <cstring>
#include <functional>

enum class commands : size_t
{
    IMPTFUNC, DCELFUNC, ENTRANCE, STACKSIZE,
    SETMEM, COPYMEM, IF, JUMP, CALL, RETURN,
    AND, OR, NOT, 
    AND8, AND16, AND32, AND64, 
    OR8, OR16, OR32, OR64,
    XOR8, XOR16, XOR32, XOR64,
    NOT8, NOT16, NOT32, NOT64,
    SHL8, SHL16, SHL32, SHL64,
    SHR8, SHR16, SHR32, SHR64, 

    LESSi8, LESSi16, LESSi32, LESSi64, LESSu8, LESSu16, LESSu32, LESSu64, LESSf32, LESSf64,
    MOREi8, MOREi16, MOREi32, MOREi64, MOREu8, MOREu16, MOREu32, MOREu64, MOREf32, MOREf64,
    EQULi8, EQULi16, EQULi32, EQULi64, EQULu8, EQULu16, EQULu32, EQULu64, EQULf32, EQULf64,

    LESTi8, LESTi16, LESTi32, LESTi64, LESTu8, LESTu16, LESTu32, LESTu64, LESTf32, LESTf64,
    MORTi8, MORTi16, MORTi32, MORTi64, MORTu8, MORTu16, MORTu32, MORTu64, MORTf32, MORTf64,
    INCi8, INCi16, INCi32, INCi64, INCu8, INCu16, INCu32, INCu64, INCf32, INCf64,
    DECi8, DECi16, DECi32, DECi64, DECu8, DECu16, DECu32, DECu64, DECf32, DECf64,

    ADDi8, ADDi16, ADDi32, ADDi64, ADDu8, ADDu16, ADDu32, ADDu64, ADDf32, ADDf64,
    MINi8, MINi16, MINi32, MINi64, MINu8, MINu16, MINu32, MINu64, MINf32, MINf64,
    MULi8, MULi16, MULi32, MULi64, MULu8, MULu16, MULu32, MULu64, MULf32, MULf64,
    DIVi8, DIVi16, DIVi32, DIVi64, DIVu8, DIVu16, DIVu32, DIVu64, DIVf32, DIVf64,
    MODi8, MODi16, MODi32, MODi64, MODu8, MODu16, MODu32, MODu64,
    ADDSi8, ADDSi16, ADDSi32, ADDSi64, ADDSu8, ADDSu16, ADDSu32, ADDSu64, ADDSf32, ADDSf64,
    MINSi8, MINSi16, MINSi32, MINSi64, MINSu8, MINSu16, MINSu32, MINSu64, MINSf32, MINSf64,
    MULSi8, MULSi16, MULSi32, MULSi64, MULSu8, MULSu16, MULSu32, MULSu64, MULSf32, MULSf64,
    DIVSi8, DIVSi16, DIVSi32, DIVSi64, DIVSu8, DIVSu16, DIVSu32, DIVSu64, DIVSf32, DIVSf64,
    MODSi8, MODSi16, MODSi32, MODSi64, MODSu8, MODSu16, MODSu32, MODSu64,
};
class func
{
public:
    using com = std::array<uint64_t, 4>;
    std::vector<com> bytes;
    void anal(std::istream& stream);
    void operator ()(char* base, char* _this);
private:
    bool rb[32];
    uint8_t ru8[32];
    uint16_t ru16[32];
    uint32_t ru32[32];
    uint64_t ru64[32];
    int8_t rs8[32];
    int16_t rs16[32];
    int32_t rs32[32];
    int64_t rs64[32];
    float rf32[32];
    double rf64[32];
};
