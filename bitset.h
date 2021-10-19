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
class Bitset
{
private:
    bool opSize;
    bool imm;
    string regs_8[8];
    string regs_16[8];
    string regs_32[8];

public:
    //map<string, int> &registers;
    //Adder(map<string, int> &registers);
    Common common;
    //queue<short> &instruction;
    map<string, int> cs;
    map<string, int> &registers;
    map<string, int> &memories32bit;
    map<string, int16_t> &memories16bit;
    map<string, int8_t> &memories8bit;
    list<string> &memoryAccesses;

    Bitset(Common com, map<string, int> cs, map<string, int> &registers, map<string, int> &memories32bit, map<string, int16_t> &memories16bit, map<string, int8_t> &memories8bit, list<string> &memoryAccesses);

    string decode_bt(short prefixes[4]);
    string decode_displacement_with_SIB(int mod, int reg, int index, int scale, int base);
    string decode_displacement_without_SIB(int mod, int reg, int rm);
    string decode_SIB(int mod, int reg);
    string decode_mod_00(int reg, int rm);
    string decode_mod_01(int reg, int rm);
    string decode_mod_10(int reg, int rm);
    string decode_mod_11(int reg, int rm);
};

class Bitset_addOverride
{
private:
    bool opSize;
    bool imm;
    uint16_t *list1[2], *list2[2], *list3[4];
    string regs_16bitmode[8];
    string regs_8[8];
    string regs_16[8];
    string regs_32[8];

public:
    Common common;
    //queue<short> &instruction;
    map<string, int> cs;
    map<string, int> &registers;
    map<string, int> &memories32bit;
    map<string, int16_t> &memories16bit;
    map<string, int8_t> &memories8bit;
    list<string> &memoryAccesses;

    Bitset_addOverride(Common com, map<string, int> cs, map<string, int> &registers, map<string, int> &memories32bit, map<string, int16_t> &memories16bit, map<string, int8_t> &memories8bit, list<string> &memoryAccesses);

    string decode_bt(short prefixes[4]);
    string decode_displacement_with_SIB(int mod, int reg, int index, int scale, int base);
    string decode_displacement_without_SIB(int mod, int reg, int rm);
    string decode_SIB(int mod, int reg);
    string decode_mod_00(int reg, int rm);
    string decode_mod_01(int reg, int rm);
    string decode_mod_10(int reg, int rm);
    string decode_mod_11(int reg, int rm);
};