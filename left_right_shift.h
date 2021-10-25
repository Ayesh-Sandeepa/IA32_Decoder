#include <stdio.h>
#include <bitset>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <queue>
#include <map>
#include <list>

#include "common.h"

using namespace std;

class Left_shift
{
private:
    bool opSize;
    string regs_8[8];
    string regs_16[8];
    string regs_32[8];
public:
    //map<string, int> &registers;
    //Adder(map<string, int> &registers);
    ofstream& myoutput;
    Common common;
    queue<short> &instruction;
    map<string, int> &registers;
    map<string, int> &memories32bit;
    map<string, int16_t> &memories16bit;
    map<string, int8_t> &memories8bit;
    list<string> &memoryAccesses;

    Left_shift(Common com, queue<short> &instruction, map<string, int> &registers, map<string, int> &memories32bit, map<string, int16_t> &memories16bit, map<string, int8_t> &memories8bit, list<string> &memoryAccesses,ofstream& myoutput);

    string decode_shl(short prefixes[4]);
    string decode_displacement_with_SIB(int d, int w, int mod, int index, int scale, int base);
    string decode_displacement_without_SIB(int d, int w, int mod, int rm);
    string decode_SIB(int d, int w, int mod);
    string decode_mod_00(int d, int w, int rm);
    string decode_mod_01(int d, int w, int rm);
    string decode_mod_10(int d, int w, int rm);
    string decode_mod_11(int d, int w, int rm);
};

class Left_shift_override
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

    Left_shift_override(Common com, queue<short> &instruction, map<string, int> &registers, map<string, int> &memories32bit, map<string, int16_t> &memories16bit, map<string, int8_t> &memories8bit, list<string> &memoryAccesses);

    string decode_shl(short prefixes[4]);
    string decode_displacement_with_SIB (int d, int w,  int mod, int index, int scale, int base);
    string decode_displacement_without_SIB(int d, int w, int mod, int rm);
    string decode_SIB(int d, int w, int mod);
    string decode_mod_00(int d, int w, int rm);
    string decode_mod_01(int d, int w, int rm);
    string decode_mod_10(int d, int w, int rm);
    string decode_mod_11(int d, int w, int rm);
};

class Right_shift
{
private:
    bool opSize;
    string regs_8[8];
    string regs_16[8];
    string regs_32[8];
public:
    //map<string, int> &registers;
    //Adder(map<string, int> &registers);
    ofstream& myoutput;
    Common common;
    queue<short> &instruction;
    map<string, int> &registers;
    map<string, int> &memories32bit;
    map<string, int16_t> &memories16bit;
    map<string, int8_t> &memories8bit;
    list<string> &memoryAccesses;

    Right_shift(Common com, queue<short> &instruction, map<string, int> &registers, map<string, int> &memories32bit, map<string, int16_t> &memories16bit, map<string, int8_t> &memories8bit, list<string> &memoryAccesses,ofstream& myoutput);

    string decode_shr(short prefixes[4], int d, int w);
    string decode_displacement_with_SIB(int d, int w, int mod, int index, int scale, int base);
    string decode_displacement_without_SIB(int d, int w, int mod, int rm);
    string decode_SIB(int d, int w, int mod);
    string decode_mod_00(int d, int w, int rm);
    string decode_mod_01(int d, int w, int rm);
    string decode_mod_10(int d, int w, int rm);
    string decode_mod_11(int d, int w, int rm);
};

class Right_shift_override
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

    Right_shift_override(Common com, queue<short> &instruction, map<string, int> &registers, map<string, int> &memories32bit, map<string, int16_t> &memories16bit, map<string, int8_t> &memories8bit, list<string> &memoryAccesses);

    string decode_shr(short prefixes[4], int d, int w);
    string decode_displacement_with_SIB(int d, int w,  int mod, int index, int scale, int base);
    string decode_displacement_without_SIB(int d, int w, int mod, int rm);
    string decode_SIB(int d, int w, int mod);
    string decode_mod_00(int d, int w, int rm);
    string decode_mod_01(int d, int w, int rm);
    string decode_mod_10(int d, int w, int rm);
    string decode_mod_11(int d, int w, int rm);
};

class Left_shift_immediate
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

    Left_shift_immediate(Common com, queue<short> &instruction, map<string, int> &registers, map<string, int> &memories32bit, map<string, int16_t> &memories16bit, map<string, int8_t> &memories8bit, list<string> &memoryAccesses);

    string decode_shl(short prefixes[4]);
    string decode_displacement_with_SIB(int w, int mod, int index, int scale, int base);
    string decode_displacement_without_SIB(int w, int mod, int rm);
    string decode_SIB(int w, int mod);
    string decode_mod_00(int w, int rm);
    string decode_mod_01(int w, int rm);
    string decode_mod_10(int w, int rm);
    string decode_mod_11(int w, int rm);
};

class Left_shift_override_immediate
{
private:
    bool opSize;
    string regs_8[8];
    string regs_16[8];
    string regs_32[8];
    uint16_t *list1[2], *list2[2], *list3[4];
    string regs_16bitmode[8];

public:
    Common common;
    queue<short> &instruction;
    map<string, int> &registers;
    map<string, int> &memories32bit;
    map<string, int16_t> &memories16bit;
    map<string, int8_t> &memories8bit;
    list<string> &memoryAccesses;

    Left_shift_override_immediate(Common com, queue<short> &instruction, map<string, int> &registers, map<string, int> &memories32bit, map<string, int16_t> &memories16bit, map<string, int8_t> &memories8bit, list<string> &memoryAccesses);

    string decode_shl(short prefixes[4]);
    string decode_displacement_with_SIB(int w,int mod, int index, int scale, int base);
    string decode_displacement_without_SIB(int w,int mod, int rm);
    string decode_SIB(int w,int mod);
    string decode_mod_00(int w,int rm);
    string decode_mod_01(int w,int rm);
    string decode_mod_10(int w,int rm);
    string decode_mod_11(int w,int rm);
};

class Right_shift_immediate
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

    Right_shift_immediate(Common com, queue<short> &instruction, map<string, int> &registers, map<string, int> &memories32bit, map<string, int16_t> &memories16bit, map<string, int8_t> &memories8bit, list<string> &memoryAccesses);

    string decode_shr(short prefixes[4], int w, int mod, int rm);
    string decode_displacement_with_SIB(int w, int mod, int index, int scale, int base);
    string decode_displacement_without_SIB(int w, int mod, int rm);
    string decode_SIB(int w, int mod);
    string decode_mod_00(int w, int rm);
    string decode_mod_01(int w, int rm);
    string decode_mod_10(int w, int rm);
    string decode_mod_11(int w, int rm);
};

class Right_shift_override_immediate
{
private:
    bool opSize;
    string regs_8[8];
    string regs_16[8];
    string regs_32[8];
    uint16_t *list1[2], *list2[2], *list3[4];
    string regs_16bitmode[8];

public:
    Common common;
    queue<short> &instruction;
    map<string, int> &registers;
    map<string, int> &memories32bit;
    map<string, int16_t> &memories16bit;
    map<string, int8_t> &memories8bit;
    list<string> &memoryAccesses;

    Right_shift_override_immediate(Common com, queue<short> &instruction, map<string, int> &registers, map<string, int> &memories32bit, map<string, int16_t> &memories16bit, map<string, int8_t> &memories8bit, list<string> &memoryAccesses);

    string decode_shr(short prefixes[4], int w,int mod, int rm);
    string decode_displacement_with_SIB(int w,int mod, int index, int scale, int base);
    string decode_displacement_without_SIB(int w,int mod, int rm);
    string decode_SIB(int w,int mod);
    string decode_mod_00(int w,int rm);
    string decode_mod_01(int w,int rm);
    string decode_mod_10(int w,int rm);
    string decode_mod_11(int w,int rm);
};