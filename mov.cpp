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

#include "mov.h"

using namespace std;

Mov::Mov(Common com, queue<short> &instruction, map<string, int> &registers, map<string, int> &memories32bit, map<string, int16_t> &memories16bit, map<string, int8_t> &memories8bit, list<string> &memoryAccesses)
    : common(com), instruction(instruction), registers(registers), memories32bit(memories32bit), memories16bit(memories16bit), memories8bit(memories8bit), memoryAccesses(memoryAccesses)

{
    regs_32[0]="EAX";
    regs_32[1]="ECX";
    regs_32[2]="EDX";
    regs_32[3]="EBX";
    regs_32[4]="ESP";
    regs_32[5]="EBP";
    regs_32[6]="ESI";
    regs_32[7]="EDI";
    
    regs_16[0]="AX";
    regs_16[1]="CX";
    regs_16[2]="DX";
    regs_16[3]="BX";
    regs_16[4]="SP";
    regs_16[5]="BP";
    regs_16[6]="SI";
    regs_16[7]="DI";
   
    regs_8[0]="AL";
    regs_8[1]="CL";
    regs_8[2]="DL";
    regs_8[3]="BL";
    regs_8[4]="AH";
    regs_8[5]="CH";
    regs_8[6]="DH";
    regs_8[7]="BH";
}

string Mov::decode_displacement_with_SIB(int w, int d, int mod, int reg, int index, int scale, int base)
{
    string dispWithSIB = "";
    int disp_bytes[] = {4, 1, 4};
    int bytes = disp_bytes[mod];

    unsigned int disp;
    string st;

    if (mod == 0 and base != 5)
    {
    }
    else
    {
        disp = common.assemble_bits(bytes, instruction, registers);
        string st = common.getHex(disp,0,0);
    }

    if (mod == 0)
    {
        if (base == 5)   // displacement without base 
        {
            if (w == 0 and d == 1)
            {
                if (index == 4)  // scale none
                {
                    dispWithSIB = "%" + regs_8[reg] + "," + st + "\n";
                    memoryAccesses.push_back("mov %" + regs_8[reg] + "," + st);
                    memoryAccesses.push_back("Read " + to_string(memories8bit[st]) + " from " + st);
                    if (reg < 4)
                    {
                        registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (memories8bit[st] & 0x000000ff); 
                    }
                    else
                    {
                        registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff00ff) | (memories8bit[st] & 0x0000ff00); 
                    } 
                }
                else
                {
                    dispWithSIB =  "%" + regs_8[reg] + "," + st + "(,%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                    memoryAccesses.push_back("mov %" + regs_8[reg] + "," + st + "(,%" + regs_32[index] + "," + to_string(scale) + ")");
                    memoryAccesses.push_back("Read " + to_string(memories8bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)]) + " from " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));
                    if (reg < 4)
                    {
                        registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (memories8bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)] & 0x000000ff); 
                    }
                    else
                    {
                        registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff00ff) | (memories8bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)] & 0x0000ff00);
                    } 
                }
            }
            else if (w == 0 and d == 0)
            {
                if (index == 4)
                {
                    dispWithSIB = st + ",%" + regs_8[reg] + "\n";
                    memoryAccesses.push_back("mov " + st + ",%" + regs_8[reg]);
                    memoryAccesses.push_back("write " +to_string(memories8bit[st]) + "to " + st);
                    if (reg < 4)
                    {
                        memories8bit[st] = common.get_bits(1, 8, registers[regs_32[reg]]);
                    }
                    else
                    {
                        memories8bit[st] = common.get_bits(9, 8, registers[regs_32[reg % 4]]); 
                    } 
                }
                else
                {
                    dispWithSIB = st + "(,%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_8[reg] + "\n";
                    memoryAccesses.push_back("mov " + st + "(,%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_8[reg]);
                    memoryAccesses.push_back("write " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0) + "to" + to_string(memories8bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)]));
                    if (reg < 4)
                    {
                        memories8bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)] = common.get_bits(1, 8, registers[regs_32[reg]]);
                    }
                    else
                    {
                        memories8bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)] = common.get_bits(9, 8, registers[regs_32[reg]]);
                    }
                }
            }
            else if (w == 1 and d == 1)
            {
                if (opSize)
                {
                    if (index == 4)
                    {
                        dispWithSIB = "%" + regs_16[reg] + "," + st + "\n";
                        memoryAccesses.push_back("mov %" + regs_16[reg] + "," + st);
                        memoryAccesses.push_back("Read " + to_string(memories16bit[st]) + " from " + st);
                        registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff0000) | (memories16bit[st] & 0x0000ffff);
                    }
                    else
                    {
                        dispWithSIB =  "%" + regs_16[reg] + "," + st + "(,%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                        memoryAccesses.push_back("mov %" + regs_16[reg] + "," + st + "(,%" + regs_32[index] + "," + to_string(scale) + ")");
                        memoryAccesses.push_back("Read " + to_string(memories16bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)]) + " from " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));
                        registers[regs_32[reg]] =  ((registers[regs_32[reg]]) & 0xffff0000) | (memories16bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)] & 0x0000ffff);
                    }
                }
                else
                {                
                    if (index == 4)
                    {
                        dispWithSIB = "%" + regs_32[reg] + "," + st + "\n";
                        memoryAccesses.push_back("mov %" + regs_32[reg] + "," + st);
                        memoryAccesses.push_back("Read " + to_string(memories32bit[st]) + " from " + st);
                        registers[regs_32[reg]] = memories32bit[st]; 
                    }
                    else
                    {
                        dispWithSIB =  "%" + regs_32[reg] + "," + st + "(,%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                        memoryAccesses.push_back("mov %" + regs_32[reg] + "," + st + "(,%" + regs_32[index] + "," + to_string(scale) + ")");
                        memoryAccesses.push_back("Read " + to_string(memories32bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)]) + " from " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));
                        registers[regs_32[reg]] =  memories32bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)];
                    }
                }
            }
            else
            {
                if (opSize)
                {
                    if (index == 4)
                    {
                        dispWithSIB = st + ",%" + regs_16[reg] + "\n";
                        memoryAccesses.push_back("mov " + st + ",%" + regs_16[reg]);
                        memoryAccesses.push_back("write " +to_string(memories16bit[st]) + "to " + st);
                        memories16bit[st] = common.get_bits(1, 16, registers[regs_32[reg]]);
                    }
                    else
                    {
                        dispWithSIB = st + "(,%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_16[reg] + "\n";
                        memoryAccesses.push_back("mov " + st + "(,%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_16[reg]);
                        memoryAccesses.push_back("write " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0) + "to" + to_string(memories16bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)]));
                        memories16bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)] = common.get_bits(1, 16, registers[regs_32[reg]]);
                    }
                }
                else
                {
                    if (index == 4)
                    {
                        dispWithSIB = st + ",%" + regs_32[reg] + "\n";
                        memoryAccesses.push_back("mov " + st + ",%" + regs_32[reg]);
                        memoryAccesses.push_back("write " +to_string(memories32bit[st]) + "to " + st);
                        memories32bit[st] = registers[regs_32[reg]];
                    }
                    else
                    {
                        dispWithSIB = st + "(,%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_32[reg] + "\n";
                        memoryAccesses.push_back("mov " + st + "(,%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_32[reg]);
                        memoryAccesses.push_back("write " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0) + "to" + to_string(memories32bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)]));
                        memories32bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)] = registers[regs_32[reg]];
                    }
                }
            }
        }
        else
        {
            if (w == 0 and d == 1)
            {
                if (index == 4)
                {
                    dispWithSIB = "%" + regs_8[reg] + "," + "(%" + regs_32[base] + ")" + "\n";
                    memoryAccesses.push_back("mov %" + regs_8[reg] + "," + "(%" + regs_32[base] + ")");
                    memoryAccesses.push_back("Read " + to_string(memories8bit[regs_32[base]]) + " from " + regs_32[base]);
                    if (reg < 4)
                    {
                        registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (memories8bit[regs_32[base]] & 0x000000ff); 
                    }
                    else
                    {
                        registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff00ff) | (memories8bit[regs_32[base]] & 0x0000ff00); 
                    } 
                }
                else
                {
                    dispWithSIB = "%" + regs_8[reg] + "," + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                    memoryAccesses.push_back("mov %" + regs_8[reg] + "," + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")");
                    memoryAccesses.push_back("Read " + to_string(memories8bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale, 0, 0)]) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2, 0, 0));
                    if (reg < 4)
                    {
                        registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (memories8bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale, 0, 0)] & 0x000000ff); 
                    }
                    else
                    {
                        registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff00ff) | (memories8bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale, 0, 0)] & 0x0000ff00);
                    }
                }
            }
            else if (w == 0 and d == 1)
            {
                if (index == 4)
                {
                    dispWithSIB = "(%" + regs_32[base] + "),%" + regs_8[reg] + "\n";
                    memoryAccesses.push_back("mov (%" + regs_32[base] + "),%" + regs_8[reg]);
                    memoryAccesses.push_back("write " +to_string(memories8bit[regs_32[base]]) + "to " + regs_32[base]);
                    if (reg < 4)
                    {
                        memories8bit[common.getHex(registers[regs_32[base]], 0, 0)] = common.get_bits(1, 8, registers[regs_32[reg]]);
                    }
                    else
                    {
                        memories8bit[common.getHex(registers[regs_32[base]], 0, 0)] = common.get_bits(9, 8, registers[regs_32[reg]]); 
                    } 
                }
                else
                {
                    dispWithSIB = "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_8[reg] + "\n";
                    memoryAccesses.push_back("mov (%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_8[reg]);
                    memoryAccesses.push_back("write " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale, 0, 0) + "to" + to_string(memories8bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale, 0, 0)]));
                    if (reg < 4)
                    {
                        memories8bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale, 0, 0)] = common.get_bits(1, 8, registers[regs_32[reg]]);
                    }
                    else
                    {
                        memories8bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale, 0, 0)] = common.get_bits(9, 8, registers[regs_32[reg]]);
                    }
                }
            }
            else if (w == 1 and d == 1)
            {
                if (opSize)
                {
                    if (index == 4)
                    {
                        dispWithSIB = "%" + regs_16[reg] + "," + "(%" + regs_32[base] + ")" + "\n";
                        memoryAccesses.push_back("mov %" + regs_16[reg] + "," + "(%" + regs_32[base] + ")");
                        memoryAccesses.push_back("Read " + to_string(memories16bit[regs_32[base]]) + " from " + st);
                        registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff0000) | (memories16bit[regs_32[base]] & 0x0000ffff); 
                    }
                    else
                    {
                        dispWithSIB = "%" + regs_16[reg] + "," + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                        memoryAccesses.push_back("mov %" + regs_16[reg] + "," + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")");
                        memoryAccesses.push_back("Read " + to_string(memories16bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale, 0, 0)]) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale, 0, 0));
                        registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff0000) | (memories16bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale, 0, 0)] & 0x0000ffff);
                    }
                }
                else
                {
                    if (index == 4)
                    {
                        dispWithSIB = "%" + regs_32[reg] + "," + "(%" + regs_32[base] + ")" + "\n";
                        memoryAccesses.push_back("mov %" + regs_32[reg] + "," + "(%" + regs_32[base] + ")");
                        memoryAccesses.push_back("Read " + to_string(memories32bit[regs_32[base]]) + " from " + st);
                        registers[regs_32[reg]] = memories32bit[regs_32[base]]; 
                    }
                    else
                    {
                        dispWithSIB = "%" + regs_32[reg] + "," + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                        memoryAccesses.push_back("mov %" + regs_32[reg] + "," + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")");
                        memoryAccesses.push_back("Read " + to_string(memories32bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale, 0, 0)]) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale, 0, 0));
                        registers[regs_32[reg]] =  memories32bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale, 0, 0)];
                    }
                }
            }
            else
            {
                if (opSize)
                {
                    if (index == 4)
                    {
                        dispWithSIB = "(%" + regs_32[base] + ")" + ",%" + regs_16[reg] + "\n";
                        memoryAccesses.push_back("mov (%" + regs_32[base] + ")" + ",%" + regs_16[reg]);
                        memoryAccesses.push_back("write " +to_string(memories16bit[regs_32[base]]) + "to " + regs_32[base]);
                        memories16bit[common.getHex(registers[regs_32[base]], 0, 0)] = common.get_bits(1, 16, registers[regs_32[reg]]);
                    }
                    else
                    {
                        dispWithSIB = "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_16[reg] + "\n";
                        memoryAccesses.push_back("mov (%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_16[reg]);
                        memoryAccesses.push_back("write " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale, 0, 0) + "to" + to_string(memories16bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale, 0, 0)]));
                        memories16bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale, 0, 0)] = common.get_bits(1, 16, registers[regs_32[reg]]);
                    }
                }
                else 
                {
                    if (index == 4)
                    {
                        dispWithSIB = "(%" + regs_32[base] + ")" + ",%" + regs_32[reg] + "\n";
                        memoryAccesses.push_back("mov (%" + regs_32[base] + ")" + ",%" + regs_32[reg]);
                        memoryAccesses.push_back("write " +to_string(memories32bit[regs_32[base]]) + "to " + regs_32[base]);
                        memories32bit[common.getHex(registers[regs_32[base]], 0, 0)] = registers[regs_32[reg]];
                    }
                    else
                    {
                        dispWithSIB = "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_32[reg] + "\n";
                        memoryAccesses.push_back("mov (%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_32[reg]);
                        memoryAccesses.push_back("write " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale, 0, 0) + "to" + to_string(memories32bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale, 0, 0)]));
                        memories32bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale, 0, 0)] = registers[regs_32[reg]];
                    }
                }
            }
        }
    }
    else
    { 
        if (w == 0)
        {
            if (d == 1)
            {
                if (index == 4)
                {
                    dispWithSIB = "%" + regs_8[reg] + "," + st + "(%" + regs_32[base] + ") \n";
                    memoryAccesses.push_back("mov %" + regs_8[reg] + "," + st + "(%" + regs_32[base] + ")");
                    memoryAccesses.push_back("Read " + to_string(memories8bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)]) + " from " + common.getHex(registers[regs_32[base]] + disp, 0, 0));
                    if (reg < 4)
                    {
                        registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (memories8bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)] & 0x000000ff);                   
                    }
                    else
                    {
                        registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff00ff) | (memories8bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)] & 0x0000ff00); 
                    }
                }
                else
                {
                    dispWithSIB = "%" + regs_8[reg] + "," + st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                    memoryAccesses.push_back("mov %" + regs_8[reg] + "," + st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")");
                    int8_t num1;
                    if (reg < 4)
                    {
                        num1 = memories8bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));
                        registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (num1 & 0x000000ff);
                    }
                    else
                    {
                        num1 = memories8bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));
                        registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff00ff) | (num1 & 0x0000ff00);
                    }
                }
            }
            else if (d == 0)
            {
                if (index == 4)
                {
                    dispWithSIB = st + "(%" + regs_32[base] + "),%" + regs_8[reg] + "\n";
                    memoryAccesses.push_back("mov " +st + "(%" + regs_32[base] + "), %" + regs_8[reg]);
                    if (reg < 4)
                    {
                        memoryAccesses.push_back("Write " + to_string(common.get_bits(1, 8, registers[regs_32[reg]])) + " to " + common.getHex(registers[regs_32[base]] + disp, 0, 0));
                        memories8bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)] = common.get_bits(1, 8, registers[regs_32[reg]]);
                    }
                    else
                    {
                        memoryAccesses.push_back("Write " + to_string(common.get_bits(9, 8, registers[regs_32[reg]])) + " to " + common.getHex(registers[regs_32[base]] + disp, 0, 0));
                        memories8bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)] = common.get_bits(9, 8, registers[regs_32[reg]]);
                    }
                }
                else
                {
                    int8_t num1;

                    dispWithSIB = st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_8[reg] + "\n"; 
                    memoryAccesses.push_back("mov " + st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_8[reg]);
                    if (reg < 4)
                    {
                        num1 = common.get_bits(1, 8, registers[regs_32[reg]]);
                    }
                    else
                    {
                        num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                    }
                    memories8bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)] = num1;
                    memoryAccesses.push_back("write " + to_string(num1) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));
                }
            }
        }
        else if (w == 1)
        {
            if (opSize)
            {
                if (d == 1)
                {
                    if (index == 4)
                    {
                        dispWithSIB = "%" + regs_16[reg] + "," + st + "(%" + regs_32[base] + ") \n";
                        memoryAccesses.push_back("mov %" + regs_16[reg] + "," + st + "(%" + regs_32[base] + ")");
                        memoryAccesses.push_back("Read " + to_string(memories16bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)]) + " from " + common.getHex(registers[regs_32[base]] + disp, 0, 0));
                        registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff0000) | (memories16bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)] & 0x0000ffff); 
                    }
                    else
                    {
                        dispWithSIB = "%" + regs_16[reg] + "," + st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                        memoryAccesses.push_back("mov %" + regs_16[reg] + "," + st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")");

                        int16_t num1;
                        num1 = memories16bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));
                        registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff0000) | (num1 & 0x0000ffff);
                    }
                }
                else if (d == 0)
                {
                    if (index == 4)
                    {
                        dispWithSIB = st + "(%" + regs_32[base] + "),%" + regs_16[reg] + "\n";
                        memoryAccesses.push_back("mov " + st + "(%" + regs_32[base] + "),%" + regs_16[reg]);
                        memoryAccesses.push_back("Write " + to_string(common.get_bits(1, 16, registers[regs_32[reg]])) + " to " + common.getHex(registers[regs_32[base]] + disp, 0, 0));
                        memories16bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)] = common.get_bits(1, 16, registers[regs_32[reg]]);
                    }
                    else
                    {
                        dispWithSIB = st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_16[reg] + "\n";
                        
                        int16_t num1;
                        num1 = common.get_bits(1, 16, registers[regs_32[reg]]);
                        memories16bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0)] = num1;
                        memoryAccesses.push_back("write " + to_string(num1) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0));
                    }
                }
            }
            else
            {
                if (d == 1)
                {
                    if (index == 4)
                    {
                        dispWithSIB = "%" + regs_32[reg] + "," + st + "(%" + regs_32[base] + ") \n";
                        memoryAccesses.push_back("mov %" + regs_32[reg] + "," + st + "(%" + regs_32[base] + ")");
                        memoryAccesses.push_back("Read " + to_string(memories32bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)]) + " from " + common.getHex(registers[regs_32[base]] + disp, 0, 0));
                        registers[regs_32[reg]] = memories32bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)]; 

                    }
                    else
                    {
                        dispWithSIB = "%" + regs_32[reg] + "," + st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                        memoryAccesses.push_back("mov %" + regs_32[reg] + "," + st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")");
                        
                        int32_t num1;
                        num1 = memories32bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));
                        registers[regs_32[reg]] = num1;
                    }
                }
                else if (d == 0)
                {
                    if (index == 4)
                    {
                        dispWithSIB = st + "(%" + regs_32[base] + "),%" + regs_32[reg] + "\n";
                        memoryAccesses.push_back("mov " + st + "(%" + regs_32[base] + "),%" + regs_32[reg]);
                        memoryAccesses.push_back("Write " + to_string(registers[regs_32[reg]]) + " to " + common.getHex(registers[regs_32[base]] + disp, 0, 0));
                        memories32bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)] = registers[regs_32[reg]];
                    }
                    else
                    {
                        dispWithSIB = st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_32[reg] + "\n";
                        
                        int32_t num1;
                        num1 = registers[regs_32[reg]];
                        memories32bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0)] = num1;
                        memoryAccesses.push_back("write " + to_string(num1) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0));
                    }
                }
            }
        }
    }
    return dispWithSIB;
}

string Mov::decode_displacement_without_SIB(int w, int d, int mod, int reg, int rm)
{
    string dispWithoutSIB = "";
    int disp_bytes[] = {4, 1, 4};
    int bytes = disp_bytes[mod];

    unsigned int disp = common.assemble_bits(bytes, instruction, registers);
    string st = common.getHex(disp,0,0);

    if (mod == 0)
    {
        if (w == 0 and d == 1)
        {
            dispWithoutSIB = "%" + regs_8[reg] + "," + st + "\n";
            memoryAccesses.push_back("mov %" + regs_8[reg] + "," + st);
            memoryAccesses.push_back("Read " + to_string(memories8bit[st]) + " from " + st);
            if (reg < 4)
            {
                registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (memories8bit[st] & 0x000000ff); 
            }
            else
            {
                registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff00ff) | (memories8bit[st] & 0x0000ff00); 
            } 
        }
        else if (w == 0 and d == 0)
        {
            dispWithoutSIB = st + ",%" + regs_8[reg] + "\n";
            memoryAccesses.push_back("mov " + st + ",%" + regs_8[reg]);
            memoryAccesses.push_back("write " +to_string(memories8bit[st]) + "to " + st);
            if (reg < 4)
            {
                memories8bit[st] = common.get_bits(1, 8, registers[regs_32[reg]]);
            }
            else
            {
                memories8bit[st] = common.get_bits(9, 8, registers[regs_32[reg % 4]]); 
            } 
        }
        else if (w == 1 and d == 1)
        {
            if (opSize)
            {

                dispWithoutSIB = "%" + regs_16[reg] + "," + st + "\n";
                memoryAccesses.push_back("mov %" + regs_16[reg] + "," + st);
                memoryAccesses.push_back("Read " + to_string(memories16bit[st]) + " from " + st);
                registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff0000) | (memories16bit[st] & 0x0000ffff);
            }
            else
            {                
                dispWithoutSIB = "%" + regs_32[reg] + "," + st + "\n";
                memoryAccesses.push_back("mov %" + regs_32[reg] + "," + st);
                memoryAccesses.push_back("Read " + to_string(memories32bit[st]) + " from " + st);
                registers[regs_32[reg]] = memories32bit[st]; 
            }
        }
        else
        {
            if (opSize)
            {
                dispWithoutSIB = st + ",%" + regs_16[reg] + "\n";
                memoryAccesses.push_back("mov " + st + ",%" + regs_16[reg]);
                memoryAccesses.push_back("write " +to_string(memories16bit[st]) + "to " + st);
                memories16bit[st] = common.get_bits(1, 16, registers[regs_32[reg]]); 
            }
            else
            {
                dispWithoutSIB = st + ",%" + regs_32[reg] + "\n";
                memoryAccesses.push_back("mov " + st + ",%" + regs_32[reg]);
                memoryAccesses.push_back("write " +to_string(memories32bit[st]) + "to " + st);
                memories32bit[st] = registers[regs_32[reg]];
            }
        }
    }
    else
    {
        if (w == 0)
        {
            if (d == 1)
            {
                dispWithoutSIB = "%" + regs_8[reg] + "," + st + "(%" + regs_32[rm] + ") \n";
                memoryAccesses.push_back("mov %" + regs_8[reg] + "," + st + "(%" + regs_32[rm] + ")");
                memoryAccesses.push_back("Read " + to_string(memories8bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)]) + " from " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));
                if (reg < 4)
                {
                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (memories8bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)] & 0x000000ff);                   
                }
                else
                {
                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff00ff) | (memories8bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)] & 0x0000ff00); 
                }
                
            }
            else if (d == 0)
            {
                dispWithoutSIB = st + "(%" + regs_32[rm] + "),%" + regs_8[reg] + "\n";
                memoryAccesses.push_back("mov " +st + "(%" + regs_32[rm] + "), %" + regs_8[reg]);
                if (reg < 4)
                {
                    memoryAccesses.push_back("Write " + to_string(common.get_bits(1, 8, registers[regs_32[reg]])) + " to " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));
                    memories8bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)] = common.get_bits(1, 8, registers[regs_32[reg]]);
                }
                else
                {
                    memoryAccesses.push_back("Write " + to_string(common.get_bits(9, 8, registers[regs_32[reg % 4]])) + " to " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));
                    memories8bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)] = common.get_bits(9, 8, registers[regs_32[reg]]);
                }
            }
        }
        else if (w == 1)
        {
            if (opSize)
            {
                if (d == 1)
                {
                    dispWithoutSIB = "%" + regs_16[reg] + "," + st + "(%" + regs_32[rm] + ") \n";
                    memoryAccesses.push_back("mov %" + regs_16[reg] + "," + st + "(%" + regs_32[rm] + ")");
                    memoryAccesses.push_back("Read " + to_string(memories16bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)]) + " from " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));
                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff0000) | (memories16bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)] & 0x0000ffff); 
                }
                else if (d == 0)
                {
                    dispWithoutSIB = st + "(%" + regs_32[rm] + "),%" + regs_16[reg] + "\n";
                    memoryAccesses.push_back("mov " + st + "(%" + regs_32[rm] + "),%" + regs_16[reg]);
                    memoryAccesses.push_back("Write " + to_string(common.get_bits(1, 16, registers[regs_32[reg]])) + " to " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));
                    memories16bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)] = common.get_bits(1, 16, registers[regs_32[reg]]);
                }
            }
            else
            {
                if (d == 1)
                {
                    dispWithoutSIB = "%" + regs_32[reg] + "," + st + "(%" + regs_32[rm] + ") \n";
                    memoryAccesses.push_back("mov %" + regs_32[reg] + "," + st + "(%" + regs_32[rm] + ")");
                    memoryAccesses.push_back("Read " + to_string(memories32bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)]) + " from " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));
                    registers[regs_32[reg]] = memories32bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)]; 
                }
                else
                {
                    dispWithoutSIB = st + "(%" + regs_32[rm] + "),%" + regs_32[reg] + "\n";
                    memoryAccesses.push_back("mov " + st + "(%" + regs_32[rm] + "),%" + regs_32[reg]);
                    memoryAccesses.push_back("Write " + to_string(registers[regs_32[reg]]) + " to " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));
                    memories32bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)] = registers[regs_32[reg]];
                }
            }
        }
    }
    return dispWithoutSIB;
};

string Mov::decode_SIB(int w, int d, int mod, int reg)
{
    string stringSib = "";

    int scale = common.get_bits(7, 2, instruction.front());
    int index = common.get_bits(4, 3, instruction.front());
    int base = common.get_bits(1, 3, instruction.front());

    instruction.pop();
    registers["EIP"] = registers["EIP"] + 1;

    scale = pow(2, scale);

    stringSib = decode_displacement_with_SIB(w, d, mod, reg, index, scale, base);
    return stringSib;
}

string Mov::decode_mod_00(int w, int d, int reg, int rm)
{
    string string00 = "";
    if (rm == 4)
    {
        string00 = decode_SIB(w, d, 0, reg);
    }
    else if (rm == 5)
    {
        string00 = decode_displacement_without_SIB(w, d, 0, reg, 5);
    }
    else
    {
        if (w == 0)
        {
            if (d == 1)
            {
                memoryAccesses.push_back("mov %" + regs_8[reg] + ",(%" + regs_32[rm] + ")");
                memoryAccesses.push_back("Read " + to_string(memories8bit[common.getHex(registers[regs_32[rm]], 0, 0)]) + " from " + common.getHex(registers[regs_32[rm]], 0, 0));                
                string00 = "%" + regs_8[reg] + ",(%" + regs_32[rm] + ")\n";
                if (reg < 4)
                {
                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (memories8bit[common.getHex(registers[regs_32[rm]], 0, 0)] & 0x000000ff);                   
                }
                else
                {
                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff00ff) | (memories8bit[common.getHex(registers[regs_32[rm]], 0, 0)] & 0x0000ff00); 
                }
            }
            else if (d == 0)
            {
                memoryAccesses.push_back("mov (%" + regs_32[rm] + "),%" + regs_8[reg] );
                string00 = "(%" + regs_32[rm] + "),%" + regs_8[reg] + "\n";
                if (reg < 4)
                {
                    memoryAccesses.push_back("Write " + common.getHex(common.get_bits(1, 8, registers[regs_32[reg]]), 0, 0) + " to " + common.getHex(registers[regs_32[rm]], 0, 0));
                    memories8bit[common.getHex(registers[regs_32[rm]], 0, 0)] = common.get_bits(1, 8, registers[regs_32[reg]]);
                }
                else
                {
                    memoryAccesses.push_back("Write " + common.getHex(common.get_bits(9, 8, registers[regs_32[reg % 4]]), 0, 0) + " to " + common.getHex(registers[regs_32[rm]], 0, 0));
                    memories8bit[common.getHex(registers[regs_32[rm]], 0, 0)] = common.get_bits(9, 8, registers[regs_32[reg]]);
                }               
            }
        }
        else if (w == 1)
        {
            if (opSize)
            {
                if (d == 1)
                {
                    string00 = "%" + regs_16[reg] + ",(%" + regs_32[rm] + ")\n";
                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff0000) | (memories16bit[common.getHex(registers[regs_32[rm]], 0, 0)] & 0x0000ffff); 
                    memoryAccesses.push_back("mov %" + regs_16[reg] + ",(%" + regs_32[rm] + ")");
                    memoryAccesses.push_back("Read " + to_string(memories16bit[common.getHex(registers[regs_32[rm]], 0, 0)]) + " from " + common.getHex(registers[regs_32[rm]], 0, 0));
                }
                else if (d == 0)
                {;
                    string00 = "(%" + regs_32[rm] + "),%" + regs_16[reg] + "\n";
                    memories16bit[common.getHex(registers[regs_32[rm]], 0, 0)] = common.get_bits(1, 16, registers[regs_32[reg]]);
                    memoryAccesses.push_back("mov (%" + regs_32[rm] + "),%" + regs_16[reg] );
                    memoryAccesses.push_back("Write " + common.getHex(registers[regs_16[reg]],0,0) + " to " + common.getHex(registers[regs_32[rm]],0,0));
                }
            }
            else
            {
                if (d == 1)
                {
                    string00 = "%" + regs_32[reg] + ",(%" + regs_32[rm] + ")\n";
                    registers[regs_32[reg]] = memories32bit[common.getHex(registers[regs_32[rm]], 0, 0)];
                    memoryAccesses.push_back("mov %" + regs_32[reg] + ",(%" + regs_32[rm] + ")");
                    memoryAccesses.push_back("Read " + to_string(memories32bit[common.getHex(registers[regs_32[rm]], 0, 0)]) + " from " + common.getHex(registers[regs_32[rm]], 0, 0));
                }
                else
                {
                    string00 = "(%" + regs_32[rm] + "),%" + regs_32[reg] + "\n";
                    memories32bit[common.getHex(registers[regs_32[rm]], 0, 0)] = registers[regs_32[reg]];
                    memoryAccesses.push_back("mov (%" + regs_32[rm] + "),%" + regs_32[reg] );
                    memoryAccesses.push_back("Write " + common.getHex(registers[regs_32[reg]],0,0) + " to " + common.getHex(registers[regs_32[rm]],0,0));
                }
            }
        }
    }
    return string00;
}

string Mov::decode_mod_01(int w, int d, int reg, int rm)
{
    string string01 = "";
    if (rm == 4)
    {
        string01 = decode_SIB(w, d, 1, reg);
    }
    else
    {
        string01 = decode_displacement_without_SIB(w, d, 1, reg, rm);
    }
    return string01;
}

string Mov::decode_mod_10(int w, int d, int reg, int rm)
{
    string string10 = "";
    if (rm == 4)
    {
        string10 = decode_SIB(w, d, 2, reg);
    }
    else
    {
        string10 = decode_displacement_without_SIB(w, d, 2, reg, rm);
    }
    return string10;
}

string Mov::decode_mod_11(int w, int d, int reg, int rm)
{
    string string11 = "";
    if (w == 0)
    {
        if (d == 1)
        {
            string11 = "%" + regs_8[reg] + ",%" + regs_8[rm] + "\n";
            if (reg < 4)
            {
                registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (registers[regs_32[rm]] & 0x000000ff);                   
            }
            else
            {
                registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff00ff) | ((registers[regs_32[rm]]) & 0x0000ff00); 
            }
        }
        else if (d == 0)
        {
            string11 = "%" + regs_8[rm] + ",%" + regs_8[reg] + "\n";
            registers[regs_8[rm]] = registers[regs_8[reg]];
            if (reg < 4)
            {
                registers[regs_32[rm]] = ((registers[regs_32[rm]]) & 0xffffff00) | (registers[regs_32[reg]] & 0x000000ff);                   
            }
            else
            {
                registers[regs_32[rm]] = ((registers[regs_32[rm]]) & 0xffff00ff) | ((registers[regs_32[reg]]) & 0x0000ff00); 
            }
        }
    }
    else if (w == 1)
    {
        if (opSize)
        {
            if (d == 1)
            {
                string11 = "%" + regs_16[reg] + ",%" + regs_16[rm] + "\n";
                registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff0000) | (registers[regs_32[rm]] & 0x0000ffff);                    
            }
            else if (d == 0)
            {
                string11 = "%" + regs_16[rm] + ",%" + regs_16[reg] + "\n";
                registers[regs_32[rm]] = ((registers[regs_32[rm]]) & 0xffff0000) | (registers[regs_32[reg]] & 0x0000ffff);     
            }
        }
        else
        {
            if (d == 1)
            {
                string11 = "%" + regs_32[reg] + ",%" + regs_32[rm] + "\n";
                registers[regs_32[reg]] = registers[regs_32[rm]];
            }
            else
            {
                string11 = "%" + regs_32[rm] + ",%" + regs_32[reg] + "\n";
                registers[regs_32[rm]] = registers[regs_32[reg]];
            }
        }
    }
    return string11;
}

string Mov::decode_imm_alt(int opCode, int w, int reg)
{
    string dec_imm_alt;
    int imm;

    if (w == 0)
    {
        imm = common.assemble_bits(1, instruction, registers);
        string st = common.getHex(imm, 0, 0);

        dec_imm_alt = "%" + regs_8[reg] + ",$" + st + "\n";

        if (reg < 4)
        {
            registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (imm & 0x000000ff);                  
        }
        else
        {
            registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff00ff) | (imm & 0x0000ff00);
        }
    }
    else
    {
        if (opSize)
        {
            imm = common.assemble_bits(2, instruction, registers);
            string st = common.getHex(imm, 0, 0);

            dec_imm_alt = "%" + regs_16[reg] + ",$" + st + "\n";
            registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff0000) | (imm & 0x0000ffff);
        }
        else
        {
            imm = common.assemble_bits(4, instruction, registers);
            string st = common.getHex(imm, 0, 0);

            dec_imm_alt = "%" + regs_32[reg] + ",$" + st + "\n";
            registers[regs_32[reg]] = imm;
        }
    }

    return dec_imm_alt;
}
string Mov::decode_mov(short prefixes[4])
{
    string decoded_bytes;

    if (prefixes[3] == 0x67)
    {
        //Adder_addOverride adder_addOverride(common, instruction, registers, memories32bit, memories16bit, memories8bit, memoryAccesses);
        //decoded_bytes = adder_addOverride.decode_add(prefixes);
    }
    else
    {
        bool d = common.get_bits(2, 1, instruction.front());
        bool w = common.get_bits(1, 1, instruction.front());

        short opCode = instruction.front();
        instruction.pop();
        registers["EIP"] = registers["EIP"] + 1;

        if (prefixes[2] == 0x66)
        {
            opSize = true;
        }
        else
        {
            opSize = false;
        }
        if(opCode == 0xb0 or opCode == 0xb8){
            bool w = common.get_bits(4, 1, opCode);
            int reg = common.get_bits(0, 3, opCode);
            decoded_bytes = decode_imm_alt(opCode,w,reg);
        }
        else{
            int mod = instruction.front() >> 6;
            int reg = common.get_bits(4, 3, instruction.front());
            int rm = common.get_bits(1, 3, instruction.front());

            instruction.pop();
            registers["EIP"] = registers["EIP"] + 1;

            if (mod == 0)
            {
                decoded_bytes = decode_mod_00(w, d, reg, rm);
            }
            else if (mod == 1)
            {
                decoded_bytes = decode_mod_01(w, d, reg, rm);
            }
            else if (mod == 2)
            {
                decoded_bytes = decode_mod_10(w, d, reg, rm);
            }
            else
            {
                decoded_bytes = decode_mod_11(w, d, reg, rm);
            }
        }
    }

    cout << "mov " << decoded_bytes;

    cout << "EAX: " << hex << registers["EAX"] << "\n";
    cout << "ECX: " << hex << registers["ECX"] << "\n";
    cout << "EDX: " << hex << registers["EDX"] << "\n";
    cout << "EBX: " << hex << registers["EBX"] << "\n";
    cout << "ESP: " << hex << registers["ESP"] << "\n";
    cout << "EBP: " << hex << registers["EBP"] << "\n";
    cout << "ESI: " << hex << registers["ESI"] << "\n";
    cout << "EDI: " << hex << registers["EDI"] << "\n";
    cout << "EFLAGS: " << hex << registers["EFLAGS"] << "\n \n";
    return "Mov instantiated and done";
}