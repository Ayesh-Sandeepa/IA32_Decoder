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

using namespace std;
class Adder
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

    Adder(Common com, queue<short> &instruction, map<string, int> &registers, map<string, int> &memories32bit, map<string, int16_t> &memories16bit, map<string, int8_t> &memories8bit, list<string> &memoryAccesses);

    string decode_add(short prefixes[4]);
    string decode_displacement_with_SIB(int w, int d, int mod, int reg, int index, int scale, int base);
    string decode_displacement_without_SIB(int w, int d, int mod, int reg, int rm);
    string decode_SIB(int w, int d, int mod, int reg);
    string decode_mod_00(int w, int d, int reg, int rm);
    string decode_mod_01(int w, int d, int reg, int rm);
    string decode_mod_10(int w, int d, int reg, int rm);
    string decode_mod_11(int w, int d, int reg, int rm);
};

class Adder_addOverride
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

    Adder_addOverride(Common com, queue<short> &instruction, map<string, int> &registers, map<string, int> &memories32bit, map<string, int16_t> &memories16bit, map<string, int8_t> &memories8bit, list<string> &memoryAccesses);

    string decode_add(short prefixes[4]);
    string decode_displacement_with_SIB(int w, int d, int mod, int reg, int index, int scale, int base);
    string decode_displacement_without_SIB(int w, int d, int mod, int reg, int rm);
    string decode_SIB(int w, int d, int mod, int reg);
    string decode_mod_00(int w, int d, int reg, int rm);
    string decode_mod_01(int w, int d, int reg, int rm);
    string decode_mod_10(int w, int d, int reg, int rm);
    string decode_mod_11(int w, int d, int reg, int rm);
};
