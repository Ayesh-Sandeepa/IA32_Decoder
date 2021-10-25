#include <stdio.h>
#include <bitset>
#include <iostream>
#include <string>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <queue>
#include <map>
#include <list>

#include "common.h"
//#include "immediate.h"

using namespace std;
class Inc
{
private:
    bool opSize;
    string regs_8[8];
    string regs_16[8];
    string regs_32[8];
public:
    //map<string, int> &registers;
    //Adder(map<string, int> &registers);
    Common common;
    queue<short> &instruction;
    map<string, int> &registers;
    map<string, int> &memories32bit;
    map<string, int16_t> &memories16bit;
    map<string, int8_t> &memories8bit;
    list<string> &memoryAccesses;

    Inc(Common com, queue<short> &instruction, map<string, int> &registers, map<string, int> &memories32bit, map<string, int16_t> &memories16bit, map<string, int8_t> &memories8bit, list<string> &memoryAccesses);

    string decode_inc(short prefixes[4]);
    string decode_displacement_with_SIB(int w, int mod, int index, int scale, int base);
    string decode_displacement_without_SIB(int w, int mod, int rm);
    string decode_SIB(int w, int mod);
    string decode_mod_00(int w, int rm);
    string decode_mod_01(int w, int rm);
    string decode_mod_10(int w, int rm);
    string decode_mod_11(int w, int rm);
};

class Inc_override
{
private:
    bool opSize;
    uint16_t *list1[2], *list2[2], *list3[4];
    string regs_16bitmode[8];
    string regs_8[8];
    string regs_16[8];
    string regs_32[8];
public:
    Common common;
    queue<short> &instruction;
    map<string, int> &registers;
    map<string, int> &memories32bit;
    map<string, int16_t> &memories16bit;
    map<string, int8_t> &memories8bit;
    list<string> &memoryAccesses;

    Inc_override(Common com, queue<short> &instruction, map<string, int> &registers, map<string, int> &memories32bit, map<string, int16_t> &memories16bit, map<string, int8_t> &memories8bit, list<string> &memoryAccesses);

    string decode_inc(short prefixes[4]);
    string decode_displacement_with_SIB(int w,  int mod, int index, int scale, int base);
    string decode_displacement_without_SIB(int w, int mod, int rm);
    string decode_SIB(int w, int mod);
    string decode_mod_00(int w, int rm);
    string decode_mod_01(int w, int rm);
    string decode_mod_10(int w, int rm);
    string decode_mod_11(int w, int rm);
};

class Dec
{
private:
    bool opSize;
    string regs_8[8];
    string regs_16[8];
    string regs_32[8];
public:
    //map<string, int> &registers;
    //Adder(map<string, int> &registers);
    Common common;
    queue<short> &instruction;
    map<string, int> &registers;
    map<string, int> &memories32bit;
    map<string, int16_t> &memories16bit;
    map<string, int8_t> &memories8bit;
    list<string> &memoryAccesses;

    Dec(Common com, queue<short> &instruction, map<string, int> &registers, map<string, int> &memories32bit, map<string, int16_t> &memories16bit, map<string, int8_t> &memories8bit, list<string> &memoryAccesses);

    string decode_dec(short prefixes[4], int w);
    string decode_displacement_with_SIB(int w, int mod, int index, int scale, int base);
    string decode_displacement_without_SIB(int w, int mod, int rm);
    string decode_SIB(int w, int mod);
    string decode_mod_00(int w, int rm);
    string decode_mod_01(int w, int rm);
    string decode_mod_10(int w, int rm);
    string decode_mod_11(int w, int rm);
};

class Dec_override
{
private:
    bool opSize;
    uint16_t *list1[2], *list2[2], *list3[4];
    string regs_16bitmode[8];
    string regs_8[8];
    string regs_16[8];
    string regs_32[8];
public:
    Common common;
    queue<short> &instruction;
    map<string, int> &registers;
    map<string, int> &memories32bit;
    map<string, int16_t> &memories16bit;
    map<string, int8_t> &memories8bit;
    list<string> &memoryAccesses;

    Dec_override(Common com, queue<short> &instruction, map<string, int> &registers, map<string, int> &memories32bit, map<string, int16_t> &memories16bit, map<string, int8_t> &memories8bit, list<string> &memoryAccesses);

    string decode_dec(short prefixes[4], int w);
    string decode_displacement_with_SIB(int w,  int mod, int index, int scale, int base);
    string decode_displacement_without_SIB(int w, int mod, int rm);
    string decode_SIB(int w, int mod);
    string decode_mod_00(int w, int rm);
    string decode_mod_01(int w, int rm);
    string decode_mod_10(int w, int rm);
    string decode_mod_11(int w, int rm);
};