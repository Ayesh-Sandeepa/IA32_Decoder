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

#include "adder.h"

using namespace std;

short enc[] = {0x03, 0x44, 0xcd, 0x80, 0x00, 0x00, 0x00};

/*
void convert_binary(short *encodings)
{
    string num1 = bitset<8>(encodings[1]).to_string();
    cout << num1 << "\n";
}
*/

Adder::Adder(Common com, queue<short> &instruction, map<string, int> &registers, map<string, int> &memories32bit, map<string, int16_t> &memories16bit, map<string, int8_t> &memories8bit, list<string> &memoryAccesses,ofstream& myoutput)
    : common(com), instruction(instruction), registers(registers), memories32bit(memories32bit), memories16bit(memories16bit), memories8bit(memories8bit), memoryAccesses(memoryAccesses), myoutput(myoutput)
{
    regs_32[0] = "EAX";
    regs_32[1] = "ECX";
    regs_32[2] = "EDX";
    regs_32[3] = "EBX";
    regs_32[4] = "ESP";
    regs_32[5] = "EBP";
    regs_32[6] = "ESI";
    regs_32[7] = "EDI";

    regs_16[0] = "AX";
    regs_16[1] = "CX";
    regs_16[2] = "DX";
    regs_16[3] = "BX";
    regs_16[4] = "SP";
    regs_16[5] = "BP";
    regs_16[6] = "SI";
    regs_16[7] = "DI";

    regs_8[0] = "AL";
    regs_8[1] = "CL";
    regs_8[2] = "DL";
    regs_8[3] = "BL";
    regs_8[4] = "AH";
    regs_8[5] = "CH";
    regs_8[6] = "DH";
    regs_8[7] = "BH";
}

string Adder::decode_displacement_with_SIB(int w, int d, int mod, int reg, int index, int scale, int base)
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
        st = common.getHex(disp, 0, 0);
    }

    if (mod == 0)
    {
        if (base == 5)
        {
            if (w == 0)
            {
                if (d == 0)
                {
                    if (index == 4)
                    {
                        memoryAccesses.push_back("add %" + regs_8[reg] + "," + st);

                        int8_t num1, num2, num3;
                        uint8_t num4;

                        if (reg < 4)
                        {
                            num1 = common.get_bits(1, 8, registers[regs_32[reg]]);
                        }
                        else
                        {
                            num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                        }
                        num2 = memories8bit[st];
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + st);

                        num3 = num1 + num2;
                        num4 = unsigned(num3);

                        memories8bit[st] = num3;

                        memoryAccesses.push_back("write " + to_string(num3) + " to " + st);

                        common.setOverflow8bit(num1, num2, num3, registers);
                        common.setCarry8bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        dispWithSIB = st + ",%" + regs_8[reg] + "\n";
                    }
                    else
                    {
                        memoryAccesses.push_back("add %" + regs_8[reg] + "," + st + "(,%" + regs_32[index] + "," + to_string(scale) + ")");

                        int8_t num1, num2, num3;
                        uint8_t num4;

                        if (reg < 4)
                        {
                            num1 = common.get_bits(1, 8, registers[regs_32[reg]]);
                        }
                        else
                        {
                            num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                        }
                        num2 = memories8bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                        num3 = num1 + num2;
                        num4 = unsigned(num3);

                        memories8bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)] = num3;
                        memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                        common.setOverflow8bit(num1, num2, num3, registers);
                        common.setCarry8bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        dispWithSIB =  st + "(,%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_8[reg] + "\n";
                    }
                }
                else if (d == 1)
                {
                    if (index == 4)
                    {
                        int8_t num1, num2, num3;
                        uint8_t num4;

                        if (reg < 4)
                        {
                            num1 = common.get_bits(1, 8, registers[regs_32[reg]]);
                            num2 = memories8bit[st];
                            memoryAccesses.push_back("Read " + to_string(num2) + " from " + st);

                            num3 = num1 + num2;
                            num4 = unsigned(num3);

                            registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (num3 & 0x000000ff);
                        }
                        else
                        {
                            num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                            num2 = memories8bit[st];
                            memoryAccesses.push_back("Read " + to_string(num2) + " from " + st);

                            num3 = num1 + num2;
                            num4 = unsigned(num3);

                            registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff00ff) | ((num3<<8)  & 0x0000ff00);
                        }

                        common.setOverflow8bit(num1, num2, num3, registers);
                        common.setCarry8bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        dispWithSIB = "%" + regs_8[reg] + "," + st + "\n";
                    }
                    else
                    {
                        int8_t num1, num2, num3;
                        uint8_t num4;

                        if (reg < 4)
                        {
                            num1 = common.get_bits(1, 8, registers[regs_32[reg]]);
                            num2 = memories8bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)];
                            memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                            num3 = num1 + num2;
                            num4 = unsigned(num3);

                            registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (num3 & 0x000000ff);
                        }
                        else
                        {
                            num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                            num2 = memories8bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)];
                            memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                            num3 = num1 + num2;
                            num4 = unsigned(num3);

                            registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff00ff) | ((num3<<8) & 0x0000ff00);
                        }

                        common.setOverflow8bit(num1, num2, num3, registers);
                        common.setCarry8bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        dispWithSIB ="%" + regs_8[reg] + "," + st + "(,%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                    }
                }
            }
            else if (w == 1)
            {
                if (opSize)
                {
                    if (d == 0)
                    {
                        if (index == 4)
                        {
                            memoryAccesses.push_back("add "+st + ",%" + regs_16[reg]);

                            int16_t num1, num2, num3;
                            uint16_t num4;

                            num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                            num2 = memories16bit[st];
                            memoryAccesses.push_back("Read " + to_string(num2) + " from " + st);

                            num3 = num1 + num2;
                            num4 = unsigned(num3);

                            memories16bit[st] = num3;
                            memoryAccesses.push_back("write " + to_string(num3) + " to " + st);

                            common.setOverflow16bit(num1, num2, num3, registers);
                            common.setCarry16bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            dispWithSIB = st + ",%" + regs_16[reg] + "\n";
                        }
                        else
                        {
                            memoryAccesses.push_back("add "+st + "(,%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_16[reg]);

                            int16_t num1, num2, num3;
                            uint16_t num4;

                            num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                            num2 = memories16bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)];
                            memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                            num3 = num1 + num2;
                            num4 = unsigned(num3);

                            memories16bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)] = num3;
                            memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                            common.setOverflow16bit(num1, num2, num3, registers);
                            common.setCarry16bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            dispWithSIB = st + "(,%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_16[reg] + "\n";
                        }
                    }
                    else if (d == 1)
                    {
                        if (index == 4)
                        {
                            int16_t num1, num2, num3;
                            uint16_t num4;

                            num1 = common.get_bits(1, 16, registers[regs_32[reg]]);
                            num2 = memories16bit[st];
                            memoryAccesses.push_back("Read " + to_string(num2) + " from " + st);

                            num3 = num1 + num2;
                            num4 = unsigned(num3);

                            registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff0000) | (num3 & 0x0000ffff);

                            common.setOverflow16bit(num1, num2, num3, registers);
                            common.setCarry16bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            dispWithSIB = "%" + regs_16[reg] + "," + st + "\n";
                        }
                        else
                        {
                            int16_t num1, num2, num3;
                            uint16_t num4;

                            num1 = common.get_bits(1, 16, registers[regs_32[reg]]);
                            num2 = memories16bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)];
                            memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                            num3 = num1 + num2;
                            num4 = unsigned(num3);

                            registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff0000) | (num3 & 0x0000ffff);

                            common.setOverflow16bit(num1, num2, num3, registers);
                            common.setCarry16bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            dispWithSIB = "%" + regs_16[reg] + "," + st + "(,%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                        }
                    }
                }
                else
                {
                    if (d == 0)
                    {
                        if (index == 4)
                        {
                            memoryAccesses.push_back("add "+st + ",%" + regs_32[reg]);

                            int num1 = memories32bit[st];
                            memoryAccesses.push_back("Read " + to_string(num1) + " from " + st);

                            int num2 = registers[regs_32[reg]];
                            int num3 = num1 + num2;
                            unsigned int num4 = unsigned(num3);

                            common.setOverflow32bit(num1, num2, num3, registers);
                            common.setCarry32bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            memories32bit[st] = num3;
                            memoryAccesses.push_back("write " + to_string(num3) + " to " + st);

                            dispWithSIB = st + ",%" + regs_32[reg] + "\n";
                        }
                        else
                        {
                            memoryAccesses.push_back("add " + st + "(,%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_32[reg] );

                            int num1 = memories32bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)];
                            memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                            int num2 = registers[regs_32[reg]];
                            int num3 = num1 + num2;
                            unsigned int num4 = unsigned(num3);

                            common.setOverflow32bit(num1, num2, num3, registers);
                            common.setCarry32bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            memories32bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)] = num3;
                            memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                            dispWithSIB = st + "(,%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_32[reg] + "\n";
                        }
                    }
                    else
                    {
                        if (index == 4)
                        {
                            int num1 = memories32bit[st];
                            memoryAccesses.push_back("Read " + to_string(num1) + " from " + st);

                            int num2 = registers[regs_32[reg]];
                            int num3 = num1 + num2;
                            unsigned int num4 = unsigned(num3);

                            common.setOverflow32bit(num1, num2, num3, registers);
                            common.setCarry32bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            registers[regs_32[reg]] = num3;

                            dispWithSIB = "%" + regs_32[reg] + "," + st + "\n";
                        }
                        else
                        {

                            int num1 = memories32bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)];
                            memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                            int num2 = registers[regs_32[reg]];
                            int num3 = num1 + num2;
                            unsigned int num4 = unsigned(num3);

                            common.setOverflow32bit(num1, num2, num3, registers);
                            common.setCarry32bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            registers[regs_32[reg]] = num3;

                            dispWithSIB = "%" + regs_32[reg] + "," + st + "(,%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                        }
                    }
                }
            }
        }
        else
        {
            if (w == 0)
            {
                if (d == 0)
                {
                    if (index == 4)
                    {
                        memoryAccesses.push_back("add "  "(%" + regs_32[base] + "),%" + regs_8[reg]);

                        int8_t num1, num2, num3;
                        uint8_t num4;

                        if (reg < 4)
                        {
                            num1 = common.get_bits(1, 8, registers[regs_32[reg]]);
                        }
                        else
                        {
                            num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                        }
                        num2 = memories8bit[common.getHex(registers[regs_32[base]], 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[base]], 0, 0));

                        num3 = num1 + num2;
                        num4 = unsigned(num3);

                        memories8bit[common.getHex(registers[regs_32[base]], 0, 0)] = num3;
                        memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[base]], 0, 0));

                        common.setOverflow8bit(num1, num2, num3, registers);
                        common.setCarry8bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        dispWithSIB =  "(%" + regs_32[base] + "),%" + regs_8[reg] + "\n";
                    }
                    else
                    {
                        memoryAccesses.push_back("add (%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_8[reg]);

                        int8_t num1, num2, num3;
                        uint8_t num4;
                        if (reg < 4)
                        {
                            num1 = common.get_bits(1, 8, registers[regs_32[reg]]);
                        }
                        else
                        {
                            num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                        }
                        num2 = memories8bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2, 0, 0));

                        num3 = num1 + num2;
                        num4 = unsigned(num3);

                        memories8bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2, 0, 0)] = num3;
                        memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2, 0, 0));

                        common.setOverflow8bit(num1, num2, num3, registers);
                        common.setCarry8bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        dispWithSIB = "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_8[reg] + "\n";
                    }
                }
                else if (d == 1)
                {
                    if (index == 4)
                    {

                        int8_t num1, num2, num3;
                        uint8_t num4;

                        if (reg < 4)
                        {
                            num1 = common.get_bits(1, 8, registers[regs_32[reg]]);
                            num2 = memories8bit[common.getHex(registers[regs_32[base]], 0, 0)];
                            memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[base]], 0, 0));

                            num3 = num1 + num2;
                            num4 = unsigned(num3);

                            registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (num3 & 0x000000ff);
                        }
                        else
                        {
                            num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                            num2 = memories8bit[common.getHex(registers[regs_32[base]], 0, 0)];
                            memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[base]], 0, 0));

                            num3 = num1 + num2;
                            num4 = unsigned(num3);

                            registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff00ff) | ((num3<<8) & 0x0000ff00);
                        }

                        common.setOverflow8bit(num1, num2, num3, registers);
                        common.setCarry8bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        dispWithSIB ="%" + regs_8[reg] + "," + "(%" + regs_32[base] + ")" + "\n";
                    }
                    else
                    {
                        int8_t num1, num2, num3;
                        uint8_t num4;

                        if (reg < 4)
                        {
                            num1 = common.get_bits(1, 8, registers[regs_32[reg]]);
                            num2 = memories8bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale , 0, 0)];
                            memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale , 0, 0));

                            num3 = num1 + num2;
                            num4 = unsigned(num3);

                            registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (num3 & 0x000000ff);
                        }
                        else
                        {
                            num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                            num2 = memories8bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale , 0, 0)];
                            memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale , 0, 0));

                            num3 = num1 + num2;
                            num4 = unsigned(num3);

                            registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff00ff) | ((num3<<8) & 0x0000ff00);
                        }

                        common.setOverflow8bit(num1, num2, num3, registers);
                        common.setCarry8bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        dispWithSIB = "%" + regs_8[reg] + "," + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                    }
                }
            }
            else if (w == 1)
            {
                if (opSize)
                {
                    if (d == 0)
                    {
                        if (index == 4)
                        {
                            memoryAccesses.push_back("add (%" + regs_32[base] + "),%" + regs_16[reg]);

                            int16_t num1, num2, num3;
                            uint16_t num4;

                            num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                            num2 = memories16bit[common.getHex(registers[regs_32[base]], 0, 0)];
                            memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[base]], 0, 0));

                            num3 = num1 + num2;
                            num4 = unsigned(num3);

                            memories16bit[common.getHex(registers[regs_32[base]], 0, 0)] = num3;
                            memoryAccesses.push_back("write " + to_string(num3) + " to " + to_string((unsigned)registers[regs_32[base]]));

                            common.setOverflow16bit(num1, num2, num3, registers);
                            common.setCarry16bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            dispWithSIB = "(%" + regs_32[base] + "),%" + regs_16[reg] + "\n";
                        }
                        else
                        {
                            memoryAccesses.push_back("add (%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_16[reg]);

                            int16_t num1, num2, num3;
                            uint16_t num4;

                            num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                            num2 = memories16bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0)];
                            memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0));

                            num3 = num1 + num2;
                            num4 = unsigned(num3);

                            memories16bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0)] = num3;
                            memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0));

                            common.setOverflow16bit(num1, num2, num3, registers);
                            common.setCarry16bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            dispWithSIB = "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_16[reg] + "\n";
                        }
                    }
                    else if (d == 1)
                    {
                        if (index == 4)
                        {

                            int16_t num1, num2, num3;
                            uint16_t num4;

                            num1 = common.get_bits(1, 16, registers[regs_32[reg]]);
                            num2 = memories16bit[common.getHex(registers[regs_32[base]], 0, 0)];
                            memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[base]], 0, 0));

                            num3 = num1 + num2;
                            num4 = unsigned(num3);

                            registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff0000) | (num3 & 0x0000ffff);

                            common.setOverflow16bit(num1, num2, num3, registers);
                            common.setCarry16bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            dispWithSIB = "%" + regs_16[reg] + "," + "(%" + regs_32[base] + ")" + "\n";
                        }
                        else
                        {

                            int16_t num1, num2, num3;
                            uint16_t num4;

                            num1 = common.get_bits(1, 16, registers[regs_32[reg]]);
                            num2 = memories16bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)];
                            memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                            num3 = num1 + num2;
                            num4 = unsigned(num3);

                            registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff0000) | (num3 & 0x0000ffff);

                            common.setOverflow16bit(num1, num2, num3, registers);
                            common.setCarry16bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            dispWithSIB = "%" + regs_16[reg] + "," + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                        }
                    }
                }
                else
                {
                    if (d == 0)
                    {
                        if (index == 4)
                        {
                            memoryAccesses.push_back("add (%" + regs_32[base] + ")" + ",%" + regs_32[reg]);

                            int num1 = memories32bit[common.getHex(registers[regs_32[base]], 0, 0)];
                            memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]], 0, 0));

                            int num2 = registers[regs_32[reg]];
                            int num3 = num1 + num2;
                            unsigned int num4 = unsigned(num3);

                            common.setOverflow32bit(num1, num2, num3, registers);
                            common.setCarry32bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            memories32bit[common.getHex(registers[regs_32[base]], 0, 0)] = num3;
                            memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[base]], 0, 0));

                            dispWithSIB = "(%" + regs_32[base] + ")" + ",%" + regs_32[reg] + "\n";
                        }
                        else
                        {
                            memoryAccesses.push_back("add (%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_32[reg]);

                            int num1 = memories32bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)];
                            memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                            int num2 = registers[regs_32[reg]];
                            int num3 = num1 + num2;
                            unsigned int num4 = unsigned(num3);

                            common.setOverflow32bit(num1, num2, num3, registers);
                            common.setCarry32bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            memories32bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)] = num3;
                            memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                            dispWithSIB = "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_32[reg] + "\n";
                        }
                    }
                    else
                    {
                        if (index == 4)
                        {
                            int num1 = memories32bit[common.getHex(registers[regs_32[base]], 0, 0)];
                            memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]], 0, 0));

                            int num2 = registers[regs_32[reg]];
                            int num3 = num1 + num2;
                            unsigned int num4 = unsigned(num3);

                            common.setOverflow32bit(num1, num2, num3, registers);
                            common.setCarry32bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            registers[regs_32[reg]] = num3;

                            dispWithSIB = "%" + regs_32[reg] + "," + "(%" + regs_32[base] + ")" + "\n";
                        }
                        else
                        {
                            printf("Mod 00 & scale 02:%d\n",scale);
                            int num1 = memories32bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)];
                            memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                            int num2 = registers[regs_32[reg]];
                            int num3 = num1 + num2;
                            unsigned int num4 = unsigned(num3);

                            common.setOverflow32bit(num1, num2, num3, registers);
                            common.setCarry32bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            registers[regs_32[reg]] = num3;

                            dispWithSIB = "%" + regs_32[reg] + "," + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                        }
                    }
                }
            }
        }
        //printf("mod equals 0");
    }
    else
    {
        //printf("mod not equals 0");

        if (w == 0)
        {
            if (d == 0)
            {
                if (index == 4)
                {
                    memoryAccesses.push_back("add " +st + "(%" + regs_32[base] + "),%" + regs_8[reg] );

                    int8_t num1, num2, num3;
                    uint8_t num4;

                    if (reg < 4)
                    {
                        num1 = common.get_bits(1, 8, registers[regs_32[reg]]);
                    }
                    else
                    {
                        num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                    }
                    num2 = memories8bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                    num3 = num1 + num2;
                    num4 = unsigned(num3);

                    memories8bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)] = num3;
                    memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                    common.setOverflow8bit(num1, num2, num3, registers);
                    common.setCarry8bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    dispWithSIB = st + "(%" + regs_32[base] + "),%" + regs_8[reg] + "\n";
                }
                else
                {
                    memoryAccesses.push_back("add " + st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_8[reg]);

                    int8_t num1, num2, num3;
                    uint8_t num4;

                    if (reg < 4)
                    {
                        num1 = common.get_bits(1, 8, registers[regs_32[reg]]);
                    }
                    else
                    {
                        num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                    }
                    num2 = memories8bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0));

                    num3 = num1 + num2;
                    num4 = unsigned(num3);

                    memories8bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0)] = num3;
                    memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0));

                    common.setOverflow8bit(num1, num2, num3, registers);
                    common.setCarry8bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    dispWithSIB = st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_8[reg] + "\n";
                }
            }
            else if (d == 1)
            {
                if (index == 4)
                {
                    int8_t num1, num2, num3;
                    uint8_t num4;

                    if (reg < 4)
                    {
                        num1 = common.get_bits(1, 8, registers[regs_32[reg]]);
                        num2 = memories8bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                        num3 = num1 + num2;
                        num4 = unsigned(num3);

                        registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (num3 & 0x000000ff);
                    }
                    else
                    {
                        num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                        num2 = memories8bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                        num3 = num1 + num2;
                        num4 = unsigned(num3);

                        registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff00ff) | ((num3<<8) & 0x0000ff00);
                    }

                    common.setOverflow8bit(num1, num2, num3, registers);
                    common.setCarry8bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    dispWithSIB = "%" + regs_8[reg] + "," + st + "(%" + regs_32[base] + ")" + "\n";
                }
                else
                {
                    int8_t num1, num2, num3;
                    uint8_t num4;

                    if (reg < 4)
                    {
                        num1 = common.get_bits(1, 8, registers[regs_32[reg]]);
                        num2 = memories8bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                        num3 = num1 + num2;
                        num4 = unsigned(num3);

                        registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (num3 & 0x000000ff);
                    }
                    else
                    {
                        num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                        num2 = memories8bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                        num3 = num1 + num2;
                        num4 = unsigned(num3);

                        registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff00ff) | ((num3<<8) & 0x0000ff00);
                    }

                    common.setOverflow8bit(num1, num2, num3, registers);
                    common.setCarry8bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    dispWithSIB = "%" + regs_8[reg] + "," + st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                }
            }
        }
        else if (w == 1)
        {
            if (opSize)
            {
                if (d == 0)
                {
                    if (index == 4)
                    {
                        memoryAccesses.push_back("add " + st + "(%" + regs_32[base] + "),%" + regs_16[reg]);

                        int16_t num1, num2, num3;
                        uint16_t num4;

                        num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                        num2 = memories16bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                        num3 = num1 + num2;
                        num4 = unsigned(num3);

                        memories16bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)] = num3;
                        memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                        common.setOverflow16bit(num1, num2, num3, registers);
                        common.setCarry16bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        dispWithSIB =  st + "(%" + regs_32[base] + "),%" + regs_16[reg] + "\n";
                    }
                    else
                    {
                        memoryAccesses.push_back("add " + st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_16[reg]);

                        int16_t num1, num2, num3;
                        uint16_t num4;

                        num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                        num2 = memories16bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0));

                        num3 = num1 + num2;
                        num4 = unsigned(num3);

                        memories16bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0)] = num3;
                        memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0));

                        common.setOverflow16bit(num1, num2, num3, registers);
                        common.setCarry16bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        dispWithSIB = st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_16[reg] + "\n";
                    }
                }
                else if (d == 1)
                {
                    if (index == 4)
                    {
                        int16_t num1, num2, num3;
                        uint16_t num4;

                        num1 = common.get_bits(1, 16, registers[regs_32[reg]]);
                        num2 = memories16bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                        num3 = num1 + num2;
                        num4 = unsigned(num3);

                        registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff0000) | (num3 & 0x0000ffff);

                        common.setOverflow16bit(num1, num2, num3, registers);
                        common.setCarry16bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        dispWithSIB ="%" + regs_16[reg] + "," + st + "(%" + regs_32[base] + ")" + "\n";
                    }
                    else
                    {
                        int16_t num1, num2, num3;
                        uint16_t num4;

                        num1 = common.get_bits(1, 16, registers[regs_32[reg]]);
                        num2 = memories16bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                        num3 = num1 + num2;
                        num4 = unsigned(num3);

                        registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff0000) | (num3 & 0x0000ffff);

                        common.setOverflow16bit(num1, num2, num3, registers);
                        common.setCarry16bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        dispWithSIB = "%" + regs_16[reg] + "," + st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                    }
                }
            }
            else
            {
                if (d == 0)
                {
                    if (index == 4)
                    {
                        memoryAccesses.push_back("add " + st + "(%" + regs_32[base] + ")" + ",%" + regs_32[reg]);

                        int num1 = memories32bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                        int num2 = registers[regs_32[reg]];
                        int num3 = num1 + num2;
                        unsigned int num4 = unsigned(num3);

                        common.setOverflow32bit(num1, num2, num3, registers);
                        common.setCarry32bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        memories32bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)] = num3;
                        memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                        dispWithSIB = st + "(%" + regs_32[base] + ")" + ",%" + regs_32[reg] + "\n";
                    }
                    else
                    {
                        memoryAccesses.push_back("add " +st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_32[reg]);

                        int num1 = memories32bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                        int num2 = registers[regs_32[reg]];
                        int num3 = num1 + num2;
                        unsigned int num4 = unsigned(num3);

                        common.setOverflow32bit(num1, num2, num3, registers);
                        common.setCarry32bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        memories32bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)] = num3;
                        memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                        dispWithSIB = st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_32[reg] + "\n";
                    }
                }
                else
                {
                    if (index == 4)
                    {
                        int num1 = memories32bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                        int num2 = registers[regs_32[reg]];
                        int num3 = num1 + num2;
                        unsigned int num4 = unsigned(num3);

                        common.setOverflow32bit(num1, num2, num3, registers);
                        common.setCarry32bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        registers[regs_32[reg]] = num3;

                        dispWithSIB = "%" + regs_32[reg] + "," + st + "(%" + regs_32[base] + ")" + "\n";
                    }
                    else
                    {
                        int num1 = memories32bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                        int num2 = registers[regs_32[reg]];
                        int num3 = num1 + num2;
                        unsigned int num4 = unsigned(num3);

                        common.setOverflow32bit(num1, num2, num3, registers);
                        common.setCarry32bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        registers[regs_32[reg]] = num3;

                        dispWithSIB = "%" + regs_32[reg] + "," + st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                    }
                }
            }
        }
    }
    return dispWithSIB;
}

string Adder::decode_displacement_without_SIB(int w, int d, int mod, int reg, int rm)
{
    string dispWithoutSIB = "";
    int disp_bytes[] = {4, 1, 4};
    int bytes = disp_bytes[mod];

    unsigned int disp = common.assemble_bits(bytes, instruction, registers);
    string st = common.getHex(disp, 0, 0);

    if (mod == 0)
    {
        if (w == 0)
        {
            if (d == 0)
            {
                memoryAccesses.push_back("add " + st + ",%" + regs_8[reg]);

                int8_t num1, num2, num3;
                uint8_t num4;

                if (reg < 4)
                {
                    num1 = common.get_bits(1, 8, registers[regs_32[reg]]);
                }
                else
                {
                    num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                }
                num2 = memories8bit[st];
                memoryAccesses.push_back("Read " + to_string(num2) + " from " + st);

                num3 = num1 + num2;
                num4 = unsigned(num3);

                memories8bit[st] = num3;
                memoryAccesses.push_back("write " + to_string(num3) + " to " + st);

                common.setOverflow8bit(num1, num2, num3, registers);
                common.setCarry8bit(num1, num4, registers);
                common.setSign(num3, registers);
                common.setZero(num3, registers);

                dispWithoutSIB = st + ",%" + regs_8[reg] + "\n";
            }
            else if (d == 1)
            {
                int8_t num1, num2, num3;
                uint8_t num4;

                if (reg < 4)
                {
                    num1 = common.get_bits(1, 8, registers[regs_32[reg]]);
                    num2 = memories8bit[st];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + st);

                    num3 = num1 + num2;
                    num4 = unsigned(num3);

                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (num3 & 0x000000ff);
                }
                else
                {
                    num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                    num2 = memories8bit[st];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + st);

                    num3 = num1 + num2;
                    num4 = unsigned(num3);

                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff00ff) | ((num3<<8) & 0x0000ff00);
                }

                common.setOverflow8bit(num1, num2, num3, registers);
                common.setCarry8bit(num1, num4, registers);
                common.setSign(num3, registers);
                common.setZero(num3, registers);

                dispWithoutSIB = "%" + regs_8[reg] + "," + st + "\n";
            }
        }
        else if (w == 1)
        {
            if (opSize)
            {
                if (d == 0)
                {
                    memoryAccesses.push_back("add " + st + ",%" + regs_16[reg]);

                    int16_t num1, num2, num3;
                    uint16_t num4;

                    num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                    num2 = memories16bit[st];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + st);

                    num3 = num1 + num2;
                    num4 = unsigned(num3);

                    memories16bit[st] = num3;
                    memoryAccesses.push_back("write " + to_string(num3) + " to " + st);

                    common.setOverflow16bit(num1, num2, num3, registers);
                    common.setCarry16bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    dispWithoutSIB = st + ",%" + regs_16[reg] + "\n";
                }
                else if (d == 1)
                {
                    int16_t num1, num2, num3;
                    uint16_t num4;

                    num1 = common.get_bits(1, 16, registers[regs_32[reg]]);
                    num2 = memories16bit[st];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + st);

                    num3 = num1 + num2;
                    num4 = unsigned(num3);

                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff0000) | (num3 & 0x0000ffff);

                    common.setOverflow16bit(num1, num2, num3, registers);
                    common.setCarry16bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    dispWithoutSIB = "%" + regs_16[reg] + "," + st + "\n";
                }
            }
            else
            {
                if (d == 0)
                {
                    memoryAccesses.push_back("add " + st + ",%" + regs_32[reg]);

                    int num1 = memories32bit[st];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + st);

                    int num2 = registers[regs_32[reg]];
                    int num3 = num1 + num2;
                    unsigned int num4 = unsigned(num3);

                    common.setOverflow32bit(num1, num2, num3, registers);
                    common.setCarry32bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    memories32bit[st] = num3;
                    memoryAccesses.push_back("write " + to_string(num3) + " to " + st);

                    dispWithoutSIB = st + ",%" + regs_32[reg] + "\n";
                }
                else
                {
                    int num1 = memories32bit[st];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + st);

                    int num2 = registers[regs_32[reg]];
                    int num3 = num1 + num2;
                    unsigned int num4 = unsigned(num3);

                    common.setOverflow32bit(num1, num2, num3, registers);
                    common.setCarry32bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    registers[regs_32[reg]] = num3;

                    dispWithoutSIB = "%" + regs_32[reg] + "," + st + "\n";
                }
            }
        }
    }
    else
    {
        if (w == 0)
        {
            if (d == 0)
            {
                memoryAccesses.push_back("add " + st + "(%" + regs_32[rm] + "),%" + regs_8[reg]);

                int8_t num1, num2, num3;
                uint8_t num4;

                if (reg < 4)
                {
                    num1 = common.get_bits(1, 8, registers[regs_32[reg]]);
                }
                else
                {
                    num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                }
                num2 = memories8bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

                num3 = num1 + num2;
                num4 = unsigned(num3);

                memories8bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)] = num3;
                memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

                common.setOverflow8bit(num1, num2, num3, registers);
                common.setCarry8bit(num1, num4, registers);
                common.setSign(num3, registers);
                common.setZero(num3, registers);

                dispWithoutSIB = st + "(%" + regs_32[rm] + "),%" + regs_8[reg] + "\n";
            }
            else if (d == 1)
            {
                int8_t num1, num2, num3;
                uint8_t num4;

                if (reg < 4)
                {
                    num1 = common.get_bits(1, 8, registers[regs_32[reg]]);
                    num2 = memories8bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

                    num3 = num1 + num2;
                    num4 = unsigned(num3);

                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (num3 & 0x000000ff);
                }
                else
                {
                    num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                    num2 = memories8bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

                    num3 = num1 + num2;
                    num4 = unsigned(num3);

                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff00ff) | ((num3<<8) & 0x0000ff00);
                }

                common.setOverflow8bit(num1, num2, num3, registers);
                common.setCarry8bit(num1, num4, registers);
                common.setSign(num3, registers);
                common.setZero(num3, registers);

                dispWithoutSIB = "%" + regs_8[reg] + "," + st + "(%" + regs_32[rm] + ") \n";
            }
        }
        else if (w == 1)
        {
            if (opSize)
            {
                if (d == 0)
                {
                    memoryAccesses.push_back("add " + st + "(%" + regs_32[rm] + "),%" + regs_16[reg]);

                    int16_t num1, num2, num3;
                    uint16_t num4;

                    num1 = common.get_bits(1, 16, registers[regs_32[reg]]);
                    num2 = memories16bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

                    num3 = num1 + num2;
                    num4 = unsigned(num3);

                    memories16bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)] = num3;
                    memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

                    common.setOverflow16bit(num1, num2, num3, registers);
                    common.setCarry16bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    dispWithoutSIB = st + "(%" + regs_32[rm] + "),%" + regs_16[reg] + "\n";
                }
                else if (d == 1)
                {
                    int16_t num1, num2, num3;
                    uint16_t num4;

                    num1 = common.get_bits(1, 16, registers[regs_32[reg]]);
                    num2 = memories16bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

                    num3 = num1 + num2;
                    num4 = unsigned(num3);

                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff0000) | (num3 & 0x0000ffff);

                    common.setOverflow16bit(num1, num2, num3, registers);
                    common.setCarry16bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    dispWithoutSIB = "%" + regs_16[reg] + "," + st + "(%" + regs_32[rm] + ") \n";
                }
            }
            else
            {
                if (d == 0)
                {
                    int num1 = memories32bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

                    int num2 = registers[regs_32[reg]];
                    int num3 = num1 + num2;
                    unsigned int num4 = unsigned(num3);

                    common.setOverflow32bit(num1, num2, num3, registers);
                    common.setCarry32bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    memories32bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)] = num3;
                    memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

                    dispWithoutSIB = st + "(%" + regs_32[rm] + "),%" + regs_32[reg] + "\n";
                }
                else
                {
                    int num1 = memories32bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

                    int num2 = registers[regs_32[reg]];
                    int num3 = num1 + num2;
                    unsigned int num4 = unsigned(num3);

                    common.setOverflow32bit(num1, num2, num3, registers);
                    common.setCarry32bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    registers[regs_32[reg]] = num3;

                    dispWithoutSIB = "%" + regs_32[reg] + "," + st + "(%" + regs_32[rm] + ") \n";
                }
            }
        }
    }

    return dispWithoutSIB;
};

string Adder::decode_SIB(int w, int d, int mod, int reg)
{
    string stringSib = "";

    int scale = common.get_bits(7, 2, instruction.front());
    int index = common.get_bits(4, 3, instruction.front());
    int base = common.get_bits(1, 3, instruction.front());

    instruction.pop();
    registers["EIP"] = registers["EIP"] + 1;

    //printf("scale:%d \n", scale);
    //printf("index:%d \n", index);
    //printf("base:%d \n", base);

    scale = pow(2, scale);

    stringSib = decode_displacement_with_SIB(w, d, mod, reg, index, scale, base);
    return stringSib;
}

string Adder::decode_mod_00(int w, int d, int reg, int rm)
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
            if (d == 0)
            {
                memoryAccesses.push_back("add (%" + regs_32[rm] + "),%"+regs_8[reg]);
                string00 = "(%" + regs_32[rm] + "),%"+regs_8[reg]+"\n";

                int8_t num1, num2, num3;
                uint8_t num4;

                if (reg < 4)
                {
                    num1 = common.get_bits(1, 8, registers[regs_32[reg]]);
                }
                else
                {
                    num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                }
                num2 = memories8bit[common.getHex(registers[regs_32[rm]], 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex((unsigned int)registers[regs_32[rm]], 0, 0));

                num3 = num1 + num2;
                num4 = unsigned(num3);

                //cout <<"num3:"<<(int)(uint8_t)num3<<"\n";

                memories8bit[common.getHex((unsigned int)registers[regs_32[rm]], 0, 0)] = (unsigned int)num3;
                memoryAccesses.push_back("write " + common.getHex((uint8_t)num3,0,0) + " to " + common.getHex(registers[regs_32[rm]], 0, 0));

                //cout<< "memories8bit["<<common.getHex((unsigned int)registers[regs_32[rm]], 0, 0)<<"]:" << (int)(uint8_t)memories8bit[common.getHex((unsigned int)registers[regs_32[rm]], 0, 0)] << "\n";

                common.setOverflow8bit(num1, num2, num3, registers);
                common.setCarry8bit(num1, num4, registers);
                common.setSign(num3, registers);
                common.setZero(num3, registers);
            }
            else if (d == 1)
            {
                string00 = "%"+regs_8[reg]+",(%" + regs_32[rm] + ")" + "\n";

                int8_t num1, num2, num3;
                uint8_t num4;

                if (reg < 4)
                {
                    num1 = common.get_bits(1, 8, registers[regs_32[reg]]);
                    num2 = memories8bit[common.getHex(registers[regs_32[rm]], 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[rm]], 0, 0));

                    num3 = num1 + num2;
                    num4 = unsigned(num3);

                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (num3 & 0x000000ff);
                }
                else
                {
                    num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                    num2 = memories8bit[common.getHex(registers[regs_32[rm]], 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[rm]], 0, 0));

                    num3 = num1 + num2;
                    num4 = unsigned(num3);

                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff00ff) | ((num3<<8) & 0x0000ff00);
                }

                common.setOverflow8bit(num1, num2, num3, registers);
                common.setCarry8bit(num1, num4, registers);
                common.setSign(num3, registers);
                common.setZero(num3, registers);
            }
        }
        else if (w == 1)
        {
            if (opSize)
            {
                if (d == 0)
                {
                    memoryAccesses.push_back("add (%" + regs_32[rm] + "),%" + regs_16[reg]);

                    string00 = "(%" + regs_32[rm] + "),%" + regs_16[reg] + "\n";

                    int16_t num1, num2, num3;
                    uint16_t num4;

                    num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                    num2 = memories16bit[common.getHex(registers[regs_32[rm]], 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[rm]], 0, 0));

                    num3 = num1 + num2;
                    num4 = unsigned(num3);

                    memories16bit[common.getHex(registers[regs_32[rm]], 0, 0)] = num3;
                    memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[rm]], 0, 0));

                    common.setOverflow16bit(num1, num2, num3, registers);
                    common.setCarry16bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);
                }
                else if (d == 1)
                {
                    string00 = "%" + regs_16[reg] + ",(%" + regs_32[rm] + ")\n";
                   

                    int16_t num1, num2, num3;
                    uint16_t num4;

                    num1 = common.get_bits(1, 16, registers[regs_32[reg]]);
                    num2 = memories16bit[common.getHex(registers[regs_32[rm]], 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[rm]], 0, 0));

                    num3 = num1 + num2;
                    num4 = unsigned(num3);

                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff0000) | (num3 & 0x0000ffff);

                    common.setOverflow16bit(num1, num2, num3, registers);
                    common.setCarry16bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);
                }
            }
            else
            {
                if (d == 0)
                {
                    memoryAccesses.push_back("add (%" + regs_32[rm] + "),%" + regs_32[reg]);

                    int num1 = memories32bit[common.getHex(registers[regs_32[rm]], 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[rm]], 0, 0));

                    int num2 = registers[regs_32[reg]]; //registers["ECX"]
                    int num3 = num1 + num2;
                    unsigned int num4 = unsigned(num3);

                    common.setOverflow32bit(num1, num2, num3, registers);
                    common.setCarry32bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    string00 = "(%" + regs_32[rm] + "),%" + regs_32[reg] + "\n";
                    memories32bit[common.getHex(registers[regs_32[rm]], 0, 0)] = num3;
                    memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[rm]], 0, 0));
                }
                else
                {
                    int num1 = memories32bit[common.getHex(registers[regs_32[rm]], 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[rm]], 0, 0));

                    int num2 = registers[regs_32[reg]];
                    int num3 = num1 + num2;
                    unsigned int num4 = unsigned(num3);

                    common.setOverflow32bit(num1, num2, num3, registers);
                    common.setCarry32bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    string00 = "%" + regs_32[reg] + ",(%" + regs_32[rm] + ")\n";
                    registers[regs_32[reg]] = num3;
                }
            }
        }
    }

    return string00;
}

string Adder::decode_mod_01(int w, int d, int reg, int rm)
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

string Adder::decode_mod_10(int w, int d, int reg, int rm)
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

string Adder::decode_mod_11(int w, int d, int reg, int rm)
{
    string string11 = "";
    if (w == 0)
    {
        if (d == 0)
        {
            //printf("w:0 and d:0 \n");

            int8_t num1, num2, num3;
            uint8_t num4;

            if (reg < 4)
            {
                num1 = common.get_bits(1, 8, registers[regs_32[reg]]);
            }
            else
            {
                num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
            }

            if (rm < 4)
            {
                num2 = common.get_bits(1, 8, registers[regs_32[rm]]);
                num3 = num1 + num2;
                num4 = unsigned(num3);
                registers[regs_32[rm]] = ((registers[regs_32[rm]]) & 0xffffff00) | (num3 & 0x000000ff);
            }
            else
            {
                num2 = common.get_bits(9, 8, registers[regs_32[rm % 4]]);
                num3 = num1 + num2;
                num4 = unsigned(num3);
                registers[regs_32[rm]] = ((registers[regs_32[rm]]) & 0xffff00ff) | ((num3 << 8) & 0x0000ff00);
            }

             //cout << "Num1:"<<common.getHex(num1,0,0)<<" ; Num2:"<<common.getHex(num2,0,0) << " ; Num3:"<<common.getHex(num3,0,0)<<"\n";

            //cout << "num1: " << dec << signed(num1) << ", num2: " << dec << signed(num2) << ", num3: " << dec << signed(num3) << "\n";

            common.setOverflow8bit(num1, num2, num3, registers);
            common.setCarry8bit(num1, num4, registers);
            common.setSign(num3, registers);
            common.setZero(num3, registers);

            string11 = "%" + regs_8[rm] + ",%" + regs_8[reg] + "\n";
        }
        else if (d == 1)
        {
            //printf("w:0 and d:1 \n");

            int8_t num1, num2, num3;
            uint8_t num4;

            if (rm < 4)
            {
                num2 = common.get_bits(1, 8, registers[regs_32[rm]]);
            }
            else
            {
                num2 = common.get_bits(9, 8, registers[regs_32[rm % 4]]);
            }

            if (reg < 4)
            {
                num1 = common.get_bits(1, 8, registers[regs_32[reg]]);
                num3 = num1 + num2;
                num4 = unsigned(num3);
                registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (num3 & 0x000000ff);
            }
            else
            {
                num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                num3 = num1 + num2;
                num4 = unsigned(num3);
                registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff00ff) | ((num3 << 8) & 0x0000ff00);
            }
            //cout << "num1: " << dec << signed(num1) << ", num2: " << dec << signed(num2) << ", num3: " << dec << signed(num3) << "\n";

            common.setOverflow8bit(num1, num2, num3, registers);
            common.setCarry8bit(num1, num4, registers);
            common.setSign(num3, registers);
            common.setZero(num3, registers);

            string11 = "%" + regs_8[reg] + ",%" + regs_8[rm] + "\n";
        }
    }
    else if (w == 1)
    {
        if (opSize)
        {
            if (d == 0)
            {
                //printf("w:0 and d:0 \n");

                int16_t num1, num2, num3;
                uint16_t num4;

                num1 = common.get_bits(1, 16, registers[regs_32[reg]]);
                num2 = common.get_bits(1, 16, registers[regs_32[rm]]);
                num3 = num1 + num2;
                num4 = unsigned(num3);
                registers[regs_32[rm]] = ((registers[regs_32[rm]]) & 0xffff0000) | (num3 & 0x0000ffff);

                //cout << sizeof(num1) << "\n";
                //cout << "Num1:"<<common.getHex((uint16_t)num1,0,0)<<" ; Num2:"<<common.getHex(num2,0,0) << " ; Num3:"<<common.getHex(num3,0,0)<<"\n";
                //cout << "num1: " << dec << signed(num1) << ", num2: " << dec << signed(num2) << ", num3: " << dec << signed(num3) << "\n";

                common.setOverflow16bit(num1, num2, num3, registers);
                common.setCarry16bit(num1, num4, registers);
                common.setSign(num3, registers);
                common.setZero(num3, registers);

                string11 = "%" + regs_16[rm] + ",%" + regs_16[reg] + "\n";
            }
            else if (d == 1)
            {
                //printf("w:0 and d:1 \n");

                int16_t num1, num2, num3;
                uint16_t num4;

                num1 = common.get_bits(1, 16, registers[regs_32[reg]]);
                num2 = common.get_bits(1, 16, registers[regs_32[rm]]);
                num3 = num1 + num2;
                num4 = unsigned(num3);
                registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff0000) | (num3 & 0x0000ffff);

                //cout << "num1: " << dec << signed(num1) << ", num2: " << dec << signed(num2) << ", num3: " << dec << signed(num3) << "\n";

                common.setOverflow16bit(num1, num2, num3, registers);
                common.setCarry16bit(num1, num4, registers);
                common.setSign(num3, registers);
                common.setZero(num3, registers);

                string11 = "%" + regs_16[reg] + ",%" + regs_16[rm] + "\n";
            }
        }
        else
        {
            if (d == 0)
            {
                //printf("w:1 and d:0 \n");
                int num1 = registers[regs_32[reg]];
                int num2 = registers[regs_32[rm]];
                int num3 = num1 + num2;
                unsigned int num4 = unsigned(num3);
                registers[regs_32[rm]] = num3;

                //cout << "num1: " << num1 << ", num2: " << num2 << ", num3: " << num3 << ", num4: " << num4 << "\n";

                common.setOverflow32bit(num1, num2, num3, registers);
                common.setCarry32bit(num1, num4, registers);
                common.setSign(num3, registers);
                common.setZero(num3, registers);

                string11 = "%" + regs_32[rm] + ",%" + regs_32[reg] + "\n";
            }
            else
            {
                //printf("w:1 and d:1 \n");
                int num1 = registers[regs_32[reg]];
                int num2 = registers[regs_32[rm]];
                int num3 = num1 + num2;
                unsigned int num4 = unsigned(num3);
                registers[regs_32[reg]] = num3;

                //cout << "num1: " << dec << num1 << ", num2: " << dec << num2 << ", num3: " << dec << num3 << "\n";

                common.setOverflow32bit(num1, num2, num3, registers);
                common.setCarry32bit(num1, num4, registers);
                common.setSign(num3, registers);
                common.setZero(num3, registers);

                string11 = "%" + regs_32[reg] + ",%" + regs_32[rm] + "\n";
            }
        }
    }

    return string11;
}

string Adder::decode_imm(int opCode, int w, int d)
{
    string dec_imm;
    int imm;

    if (w == 0)
    {
        int8_t num1, num2;
        uint8_t num3;
        imm = common.assemble_bits(1, instruction, registers);

        string st = common.getHex(imm, 0, 0);

        dec_imm = "%%AL, $"+st;

        num1 = common.get_bits(1, 8, registers["EAX"]);
        num2 = imm + num1;

        num3 = unsigned(num2);

        registers["EAX"] = ((registers["EAX"]) & 0xffffff00) | (num2 & 0x000000ff);

        common.setOverflow8bit(imm, num1, num2, registers);
        common.setCarry8bit(num1, num3, registers);
        common.setSign(num2, registers);
        common.setZero(num2, registers);
    }
    else if (w == 1)
    {
        if (opSize)
        {
            int16_t num1, num2;
            uint16_t num3;

            imm = common.assemble_bits(2, instruction, registers);
            string st = common.getHex(imm, 0, 0);

            dec_imm = "%%AX, $"+st;

            num1 = common.get_bits(1, 16, registers["EAX"]);
            num2 = imm + num1;

            num3 = unsigned(num2);

            registers["EAX"] = ((registers["EAX"]) & 0xffff0000) | (num2 & 0x0000ffff);

            common.setOverflow16bit(imm, num1, num2, registers);
            common.setCarry16bit(num1, num3, registers);
            common.setSign(num2, registers);
            common.setZero(num2, registers);
        }
        else
        {
            int num1, num2;
            unsigned int num3;

            imm = common.assemble_bits(4, instruction, registers);
            string st = common.getHex(imm, 0, 0);

            dec_imm = "%%EAX, $"+st;

            num1 = registers["EAX"];
            num2 = imm + num1;

            num3 = unsigned(num2);

            registers["EAX"] = num2;

            common.setOverflow32bit(imm, num1, num2, registers);
            common.setCarry32bit(num1, num3, registers);
            common.setSign(num2, registers);
            common.setZero(num2, registers);
        }
    }

    return dec_imm;
}
/*
int main()
{
    //printf ("Typical Hello World!");
    cout << enc[1] << " " << enc[0] << "\n";

    bool d = common.get_bits(2, 1, enc[0]);
    bool w = common.get_bits(1, 1, enc[0]);

    int mod = enc[1] >> 6;
    int reg = common.get_bits(4, 3, enc[1]);
    int rm = common.get_bits(1, 3, enc[1]);

    printf("d:%d \n", d);
    printf("w:%d \n", w);
    printf("mod:%d \n", mod);
    printf("reg:%d \n", reg);
    printf("rm:%d \n", rm);

    if (mod == 0)
    {
        decode_mod_00(w, d, reg, rm);
    }
    else if (mod == 1)
    {
        decode_mod_01(w, d, reg, rm);
    }
    else if (mod == 2)
    {
        decode_mod_10(w, d, reg, rm);
    }
    else
    {
        decode_mod_11(w, d, reg, rm);
    }

    //convert_binary(enc);

    //printf("%d \n", common.get_bits(8,enc[1]));
    return 0;
}
*/

// parity and auxiliary flag should be used .......................................................//
// Exception handling when memory address is not there, normally the addressest that are not initiailized are set to zero
// Display the memory accesses at final
// Decode instructions with prefixes
// Add other opcodes 0x81 etc
// Use of SS in using reference addresses of ESP and EBP registers
string Adder::decode_add(short prefixes[4])
{
    string decoded_bytes;

    if (prefixes[3] == 0x67)
    {
        Adder_addOverride adder_addOverride(common, instruction, registers, memories32bit, memories16bit, memories8bit, memoryAccesses);
        decoded_bytes = adder_addOverride.decode_add(prefixes);
    }
    else
    {
        short opCode = instruction.front();
        bool d = common.get_bits(2, 1, opCode);
        bool w = common.get_bits(1, 1, opCode);

        instruction.pop();
        registers["EIP"] = registers["EIP"] + 1;

        //printf("d:%d \n", d);
        //printf("w:%d \n", w);
        //printf("mod:%d \n", mod);
        //printf("reg:%d \n", reg);
        //printf("rm:%d \n", rm);

        if (prefixes[2] == 0x66)
        {
            opSize = true;
        }
        else
        {
            opSize = false;
        }

        if (opCode == 0x04 or opCode == 0x05)
        {
            decoded_bytes = decode_imm(opCode, w, d);
        }
        else
        {
            short modrm = instruction.front();
            int mod = modrm >> 6;
            int reg = common.get_bits(4, 3, modrm);
            int rm = common.get_bits(1, 3, modrm);

            bool immediate = common.get_bits(8, 1, opCode);

            instruction.pop();
            registers["EIP"] = registers["EIP"] + 1;

            if (immediate)
            {
                Immediate immediate(common, instruction, registers, memories32bit, memories16bit, memories8bit, memoryAccesses);
                decoded_bytes = immediate.decode_imm(prefixes, w, d, mod, rm);
            }
            else
            {
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
    }

    cout << "add " << decoded_bytes;

    cout << "EAX: " << hex << registers["EAX"] << "\n";
    cout << "ECX: " << hex << registers["ECX"] << "\n";
    cout << "EDX: " << hex << registers["EDX"] << "\n";
    cout << "EBX: " << hex << registers["EBX"] << "\n";
    cout << "ESP: " << hex << registers["ESP"] << "\n";
    cout << "EBP: " << hex << registers["EBP"] << "\n";
    cout << "ESI: " << hex << registers["ESI"] << "\n";
    cout << "EDI: " << hex << registers["EDI"] << "\n";
    cout << "EFLAGS: " << hex << registers["EFLAGS"] << "\n \n";

    myoutput << "add " << decoded_bytes;

    myoutput << "EAX: " << hex << registers["EAX"] << "\n";
    myoutput << "ECX: " << hex << registers["ECX"] << "\n";
    myoutput << "EDX: " << hex << registers["EDX"] << "\n";
    myoutput<< "EBX: " << hex << registers["EBX"] << "\n";
    myoutput << "ESP: " << hex << registers["ESP"] << "\n";
    myoutput << "EBP: " << hex << registers["EBP"] << "\n";
    myoutput << "ESI: " << hex << registers["ESI"] << "\n";
    myoutput << "EDI: " << hex << registers["EDI"] << "\n";
    myoutput << "EFLAGS: " << hex << registers["EFLAGS"] << "\n \n";

    return "Adder instantiated and done";
}
