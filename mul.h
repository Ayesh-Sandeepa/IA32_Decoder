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
class Multiplier
{
private:
    bool opSize;
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

    Multiplier(Common com, queue<short> &instruction, map<string, int> &registers, map<string, int> &memories32bit, map<string, int16_t> &memories16bit, map<string, int8_t> &memories8bit, list<string> &memoryAccesses);
    
    string decode_mul(short prefixes[4]);
    string decode_displacement_with_SIB(int w,  int mod, int index, int scale, int base);
    string decode_displacement_without_SIB(int w,  int mod,  int rm);
    string decode_SIB(int w,  int mod);
    string decode_mod_00(int w,   int rm);
    string decode_mod_01(int w,   int rm);
    string decode_mod_10(int w,  int rm);
    string decode_mod_11(int w,   int rm);
    void setOverflowCarry8(uint8_t num);
    void setOverflowCarry16(uint16_t num);
    void setOverflowCarry32(unsigned int num);

};