#include <stdio.h>
#include <string>
#include <sstream>
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

#include "immediate.h"

using namespace std;

short enc[] = {0x03, 0x44, 0xcd, 0x80, 0x00, 0x00, 0x00};

/*
void convert_binary(short *encodings)
{
    string num1 = bitset<8>(encodings[1]).to_string();
    cout << num1 << "\n";
}
*/
/*
Immediate::Immediate(Common com, queue<short> &instruction, map<string, int> &registers, map<string, int> &memories32bit, map<string, int16_t> &memories16bit, map<string, int8_t> &memories8bit, list<string> &memoryAccesses)
    : common(com), instruction(instruction), registers(registers), memories32bit(memories32bit), memories16bit(memories16bit), memories8bit(memories8bit), memoryAccesses(memoryAccesses)
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

string Immediate::decode_displacement_with_SIB(int w, int s, int mod, int reg, int index, int wcale, int base)
{
    string dispWithSIB = "";

    int sisp_bytes[] = {4, 1, 4};
    int bytes = disp_bytes[mod];

    unsigned int disp;

    if (mod == 0 and base != 5)
    {
    }
    else
    {
        disp = common.assemble_bits(bytes, true, instruction, registers);
    }

    if (mod == 0)
    {
        if (base == 5)
        {
            if (w == 0)
            {
                if (s == 0)
                {
                    if (index == 4)
                    {
                        memoryAccesses.push_back("add %" + regs_8[reg] + "," + to_string(disp));

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
                        num2 = memories8bit[to_string(disp)];
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string(disp));

                        num3 = num1 + num2;
                        num4 = unsigned(num3);

                        memories8bit[to_string(disp)] = num3;

                        memoryAccesses.push_back("write " + to_string(num3) + " to " + to_string(disp));

                        common.setOverflow8bit(num1, num2, num3, registers);
                        common.setCarry8bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        dispWithSIB = "%" + regs_8[reg] + "," + to_string(disp) + "\n";
                    }
                    else
                    {
                        memoryAccesses.push_back("add %" + regs_8[reg] + "," + to_string(disp) + "(,%" + regs_32[index] + "," + to_string(scale) + ")");

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
                        num2 = memories8bit[to_string((unsigned)(registers[regs_32[index]] * 2 + disp))];
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string((unsigned)(registers[regs_32[index]] * 2 + disp)));

                        num3 = num1 + num2;
                        num4 = unsigned(num3);

                        memories8bit[to_string((unsigned)(registers[regs_32[index]] * scale + disp))] = num3;
                        memoryAccesses.push_back("write " + to_string(num3) + " to " + to_string((unsigned)(registers[regs_32[index]] * scale + disp)));

                        common.setOverflow8bit(num1, num2, num3, registers);
                        common.setCarry8bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        dispWithSIB = "%" + regs_8[reg] + "," + to_string(disp) + "(,%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                    }
                }
                else if (s == 1)
                {
                    if (index == 4)
                    {
                        int8_t num1, num2, num3;
                        uint8_t num4;

                        if (reg < 4)
                        {
                            num1 = common.get_bits(1, 8, registers[regs_32[reg]]);
                            num2 = memories8bit[to_string(disp)];
                            memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string(disp));

                            num3 = num1 + num2;
                            num4 = unsigned(num3);

                            registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (num3 & 0x000000ff);
                        }
                        else
                        {
                            num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                            num2 = memories8bit[to_string(disp)];
                            memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string(disp));

                            num3 = num1 + num2;
                            num4 = unsigned(num3);

                            registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff00ff) | (num3 & 0x0000ff00);
                        }

                        common.setOverflow8bit(num1, num2, num3, registers);
                        common.setCarry8bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        dispWithSIB = to_string(disp) + ",%" + regs_8[reg] + "\n";
                    }
                    else
                    {
                        int8_t num1, num2, num3;
                        uint8_t num4;

                        if (reg < 4)
                        {
                            num1 = common.get_bits(1, 8, registers[regs_32[reg]]);
                            num2 = memories8bit[to_string((unsigned)(registers[regs_32[index]] * scale + disp))];
                            memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string((unsigned)(registers[regs_32[index]] * scale + disp)));

                            num3 = num1 + num2;
                            num4 = unsigned(num3);

                            registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (num3 & 0x000000ff);
                        }
                        else
                        {
                            num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                            num2 = memories8bit[to_string((unsigned)(registers[regs_32[index]] * scale + disp))];
                            memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string((unsigned)(registers[regs_32[index]] * scale + disp)));

                            num3 = num1 + num2;
                            num4 = unsigned(num3);

                            registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff00ff) | (num3 & 0x0000ff00);
                        }

                        common.setOverflow8bit(num1, num2, num3, registers);
                        common.setCarry8bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        dispWithSIB = to_string(disp) + "(,%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_8[reg] + "\n";
                    }
                }
            }
            else if (w == 1)
            {
                if (opSize)
                {
                    if (s == 0)
                    {
                        if (index == 4)
                        {
                            memoryAccesses.push_back("add %" + regs_16[reg] + "," + to_string(disp));

                            int16_t num1, num2, num3;
                            uint16_t num4;

                            num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                            num2 = memories16bit[to_string(disp)];
                            memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string(disp));

                            num3 = num1 + num2;
                            num4 = unsigned(num3);

                            memories16bit[to_string(disp)] = num3;
                            memoryAccesses.push_back("write " + to_string(num3) + " to " + to_string(disp));

                            common.setOverflow16bit(num1, num2, num3, registers);
                            common.setCarry16bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            dispWithSIB = "%" + regs_16[reg] + "," + to_string(disp) + "\n";
                        }
                        else
                        {
                            memoryAccesses.push_back("add %" + regs_16[reg] + "," + to_string(disp) + "(,%" + regs_32[index] + "," + to_string(scale) + ")");

                            int16_t num1, num2, num3;
                            uint16_t num4;

                            num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                            num2 = memories16bit[to_string((unsigned)(registers[regs_32[index]] * 2 + disp))];
                            memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string((unsigned)(registers[regs_32[index]] * 2 + disp)));

                            num3 = num1 + num2;
                            num4 = unsigned(num3);

                            memories16bit[to_string((unsigned)(registers[regs_32[index]] * scale + disp))] = num3;
                            memoryAccesses.push_back("write " + to_string(num3) + " to " + to_string((unsigned)(registers[regs_32[index]] * scale + disp)));

                            common.setOverflow16bit(num1, num2, num3, registers);
                            common.setCarry16bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            dispWithSIB = "%" + regs_16[reg] + "," + to_string(disp) + "(,%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                        }
                    }
                    else if (s == 1)
                    {
                        if (index == 4)
                        {
                            int16_t num1, num2, num3;
                            uint16_t num4;

                            num1 = common.get_bits(1, 16, registers[regs_32[reg]]);
                            num2 = memories16bit[to_string(disp)];
                            memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string(disp));

                            num3 = num1 + num2;
                            num4 = unsigned(num3);

                            registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff0000) | (num3 & 0x0000ffff);

                            common.setOverflow16bit(num1, num2, num3, registers);
                            common.setCarry16bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            dispWithSIB = to_string(disp) + ",%" + regs_16[reg] + "\n";
                        }
                        else
                        {
                            int16_t num1, num2, num3;
                            uint16_t num4;

                            num1 = common.get_bits(1, 16, registers[regs_32[reg]]);
                            num2 = memories16bit[to_string((unsigned)(registers[regs_32[index]] * scale + disp))];
                            memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string((unsigned)(registers[regs_32[index]] * scale + disp)));

                            num3 = num1 + num2;
                            num4 = unsigned(num3);

                            registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff0000) | (num3 & 0x0000ffff);

                            common.setOverflow16bit(num1, num2, num3, registers);
                            common.setCarry16bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            dispWithSIB = to_string(disp) + "(,%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_16[reg] + "\n";
                        }
                    }
                }
                else
                {
                    if (s == 1 and d == 0)
                    {
                        if (index == 4)
                        {
                            memoryAccesses.push_back("add %" + regs_32[reg] + "," + to_string(disp));

                            int num1 = memories32bit[to_string(disp)];
                            memoryAccesses.push_back("Read " + to_string(num1) + " from " + to_string(disp));

                            int num2 = registers[regs_32[reg]];
                            int num3 = num1 + num2;
                            unsigned int num4 = unsigned(num3);

                            common.setOverflow32bit(num1, num2, num3, registers);
                            common.setCarry32bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            memories32bit[to_string(disp)] = num3;
                            memoryAccesses.push_back("write " + to_string(num3) + " to " + to_string(disp));

                            dispWithSIB = "%" + regs_32[reg] + "," + to_string(disp) + "\n";
                        }
                        else
                        {
                            memoryAccesses.push_back("add %" + regs_32[reg] + "," + to_string(disp) + "(,%" + regs_32[index] + "," + to_string(scale) + ")");

                            int num1 = memories32bit[to_string((unsigned)(registers[regs_32[index]] * scale + disp))];
                            memoryAccesses.push_back("Read " + to_string(num1) + " from " + to_string((unsigned)(registers[regs_32[index]] * scale + disp)));

                            int num2 = registers[regs_32[reg]];
                            int num3 = num1 + num2;
                            unsigned int num4 = unsigned(num3);

                            common.setOverflow32bit(num1, num2, num3, registers);
                            common.setCarry32bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            memories32bit[to_string((unsigned)(registers[regs_32[index]] * scale + disp))] = num3;
                            memoryAccesses.push_back("write " + to_string(num3) + " to " + to_string((unsigned)(registers[regs_32[index]] * scale + disp)));

                            dispWithSIB = "%" + regs_32[reg] + "," + to_string(disp) + "(,%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                        }
                    }
                    else
                    {
                        if (index == 4)
                        {
                            int num1 = memories32bit[to_string(disp)];
                            memoryAccesses.push_back("Read " + to_string(num1) + " from " + to_string(disp));

                            int num2 = registers[regs_32[reg]];
                            int num3 = num1 + num2;
                            unsigned int num4 = unsigned(num3);

                            common.setOverflow32bit(num1, num2, num3, registers);
                            common.setCarry32bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            registers[regs_32[reg]] = num3;

                            dispWithSIB = to_string(disp) + ",%" + regs_32[reg] + "\n";
                        }
                        else
                        {

                            int num1 = memories32bit[to_string((unsigned)(registers[regs_32[index]] * scale + disp))];
                            memoryAccesses.push_back("Read " + to_string(num1) + " from " + to_string((unsigned)(registers[regs_32[index]] * scale + disp)));

                            int num2 = registers[regs_32[reg]];
                            int num3 = num1 + num2;
                            unsigned int num4 = unsigned(num3);

                            common.setOverflow32bit(num1, num2, num3, registers);
                            common.setCarry32bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            registers[regs_32[reg]] = num3;

                            dispWithSIB = to_string(disp) + "(,%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_32[reg] + "\n";
                        }
                    }
                }
            }
        }
        else
        {
            if (w == 0)
            {
                if (s == 0)
                {
                    if (index == 4)
                    {
                        memoryAccesses.push_back("add %" + regs_8[reg] + "," + "(%" + regs_32[base] + ")");

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
                        num2 = memories8bit[to_string((unsigned)registers[regs_32[base]])];
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string((unsigned)registers[regs_32[base]]));

                        num3 = num1 + num2;
                        num4 = unsigned(num3);

                        memories8bit[to_string((unsigned)registers[regs_32[base]])] = num3;
                        memoryAccesses.push_back("write " + to_string(num3) + " to " + to_string((unsigned)registers[regs_32[base]]));

                        common.setOverflow8bit(num1, num2, num3, registers);
                        common.setCarry8bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        dispWithSIB = "%" + regs_8[reg] + "," + "(%" + regs_32[base] + ")" + "\n";
                    }
                    else
                    {
                        memoryAccesses.push_back("add %" + regs_8[reg] + "," + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")");

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
                        num2 = memories8bit[to_string((unsigned)(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp))];
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string((unsigned)(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp)));

                        num3 = num1 + num2;
                        num4 = unsigned(num3);

                        memories8bit[to_string((unsigned)(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp))] = num3;
                        memoryAccesses.push_back("write " + to_string(num3) + " to " + to_string((unsigned)(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp)));

                        common.setOverflow8bit(num1, num2, num3, registers);
                        common.setCarry8bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        dispWithSIB = "%" + regs_8[reg] + "," + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                    }
                }
                else if (s == 1)
                {
                    if (index == 4)
                    {

                        int8_t num1, num2, num3;
                        uint8_t num4;

                        if (reg < 4)
                        {
                            num1 = common.get_bits(1, 8, registers[regs_32[reg]]);
                            num2 = memories8bit[to_string((unsigned)registers[regs_32[base]])];
                            memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string((unsigned)registers[regs_32[base]]));

                            num3 = num1 + num2;
                            num4 = unsigned(num3);

                            registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (num3 & 0x000000ff);
                        }
                        else
                        {
                            num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                            num2 = memories8bit[to_string((unsigned)registers[regs_32[base]])];
                            memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string((unsigned)registers[regs_32[base]]));

                            num3 = num1 + num2;
                            num4 = unsigned(num3);

                            registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff00ff) | (num3 & 0x0000ff00);
                        }

                        common.setOverflow8bit(num1, num2, num3, registers);
                        common.setCarry8bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        dispWithSIB = "(%" + regs_32[base] + "),%" + regs_8[reg] + "\n";
                    }
                    else
                    {

                        int8_t num1, num2, num3;
                        uint8_t num4;

                        if (reg < 4)
                        {
                            num1 = common.get_bits(1, 8, registers[regs_32[reg]]);
                            num2 = memories8bit[to_string((unsigned)(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp))];
                            memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string((unsigned)(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp)));

                            num3 = num1 + num2;
                            num4 = unsigned(num3);

                            registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (num3 & 0x000000ff);
                        }
                        else
                        {
                            num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                            num2 = memories8bit[to_string((unsigned)(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp))];
                            memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string((unsigned)(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp)));

                            num3 = num1 + num2;
                            num4 = unsigned(num3);

                            registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff00ff) | (num3 & 0x0000ff00);
                        }

                        common.setOverflow8bit(num1, num2, num3, registers);
                        common.setCarry8bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        dispWithSIB = "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_8[reg] + "\n";
                    }
                }
            }
            else if (w == 1)
            {
                if (opSize)
                {
                    if (s == 0)
                    {
                        if (index == 4)
                        {
                            memoryAccesses.push_back("add %" + regs_16[reg] + "," + "(%" + regs_32[base] + ")");

                            int16_t num1, num2, num3;
                            uint16_t num4;

                            num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                            num2 = memories16bit[to_string((unsigned)registers[regs_32[base]])];
                            memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string((unsigned)registers[regs_32[base]]));

                            num3 = num1 + num2;
                            num4 = unsigned(num3);

                            memories16bit[to_string((unsigned)registers[regs_32[base]])] = num3;
                            memoryAccesses.push_back("write " + to_string(num3) + " to " + to_string((unsigned)registers[regs_32[base]]));

                            common.setOverflow16bit(num1, num2, num3, registers);
                            common.setCarry16bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            dispWithSIB = "%" + regs_16[reg] + "," + "(%" + regs_32[base] + ")" + "\n";
                        }
                        else
                        {
                            memoryAccesses.push_back("add %" + regs_16[reg] + "," + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")");

                            int16_t num1, num2, num3;
                            uint16_t num4;

                            num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                            num2 = memories16bit[to_string((unsigned)(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp))];
                            memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string((unsigned)(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp)));

                            num3 = num1 + num2;
                            num4 = unsigned(num3);

                            memories16bit[to_string((unsigned)(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp))] = num3;
                            memoryAccesses.push_back("write " + to_string(num3) + " to " + to_string((unsigned)(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp)));

                            common.setOverflow16bit(num1, num2, num3, registers);
                            common.setCarry16bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            dispWithSIB = "%" + regs_16[reg] + "," + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                        }
                    }
                    else if (s == 1)
                    {
                        if (index == 4)
                        {

                            int16_t num1, num2, num3;
                            uint16_t num4;

                            num1 = common.get_bits(1, 16, registers[regs_32[reg]]);
                            num2 = memories16bit[to_string((unsigned)registers[regs_32[base]])];
                            memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string((unsigned)registers[regs_32[base]]));

                            num3 = num1 + num2;
                            num4 = unsigned(num3);

                            registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff0000) | (num3 & 0x0000ffff);

                            common.setOverflow16bit(num1, num2, num3, registers);
                            common.setCarry16bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            dispWithSIB = "(%" + regs_32[base] + "),%" + regs_16[reg] + "\n";
                        }
                        else
                        {

                            int16_t num1, num2, num3;
                            uint16_t num4;

                            num1 = common.get_bits(1, 16, registers[regs_32[reg]]);
                            num2 = memories16bit[to_string((unsigned)(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp))];
                            memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string((unsigned)(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp)));

                            num3 = num1 + num2;
                            num4 = unsigned(num3);

                            registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff0000) | (num3 & 0x0000ffff);

                            common.setOverflow16bit(num1, num2, num3, registers);
                            common.setCarry16bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            dispWithSIB = "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_16[reg] + "\n";
                        }
                    }
                }
                else
                {
                    if (s == 0)
                    {
                        if (index == 4)
                        {
                            memoryAccesses.push_back("add %" + regs_32[reg] + "," + "(%" + regs_32[base] + ")");

                            int num1 = memories32bit[to_string((unsigned)registers[regs_32[base]])];
                            memoryAccesses.push_back("Read " + to_string(num1) + " from " + to_string((unsigned)registers[regs_32[base]]));

                            int num2 = registers[regs_32[reg]];
                            int num3 = num1 + num2;
                            unsigned int num4 = unsigned(num3);

                            common.setOverflow32bit(num1, num2, num3, registers);
                            common.setCarry32bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            memories32bit[to_string((unsigned)registers[regs_32[base]])] = num3;
                            memoryAccesses.push_back("write " + to_string(num3) + " to " + to_string((unsigned)registers[regs_32[base]]));

                            dispWithSIB = "%" + regs_32[reg] + "," + "(%" + regs_32[base] + ")" + "\n";
                        }
                        else
                        {
                            memoryAccesses.push_back("add %" + regs_32[reg] + "," + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")");

                            int num1 = memories32bit[to_string((unsigned)(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp))];
                            memoryAccesses.push_back("Read " + to_string(num1) + " from " + to_string((unsigned)(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp)));

                            int num2 = registers[regs_32[reg]];
                            int num3 = num1 + num2;
                            unsigned int num4 = unsigned(num3);

                            common.setOverflow32bit(num1, num2, num3, registers);
                            common.setCarry32bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            memories32bit[to_string((unsigned)(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp))] = num3;
                            memoryAccesses.push_back("write " + to_string(num3) + " to " + to_string((unsigned)(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp)));

                            dispWithSIB = "%" + regs_32[reg] + "," + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                        }
                    }
                    else
                    {
                        if (index == 4)
                        {
                            int num1 = memories32bit[to_string((unsigned)registers[regs_32[base]])];
                            memoryAccesses.push_back("Read " + to_string(num1) + " from " + to_string((unsigned)registers[regs_32[base]]));

                            int num2 = registers[regs_32[reg]];
                            int num3 = num1 + num2;
                            unsigned int num4 = unsigned(num3);

                            common.setOverflow32bit(num1, num2, num3, registers);
                            common.setCarry32bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            registers[regs_32[reg]] = num3;

                            dispWithSIB = "(%" + regs_32[base] + ")" + ",%" + regs_32[reg] + "\n";
                        }
                        else
                        {
                            int num1 = memories32bit[to_string((unsigned)(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp))];
                            memoryAccesses.push_back("Read " + to_string(num1) + " from " + to_string((unsigned)(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp)));

                            int num2 = registers[regs_32[reg]];
                            int num3 = num1 + num2;
                            unsigned int num4 = unsigned(num3);

                            common.setOverflow32bit(num1, num2, num3, registers);
                            common.setCarry32bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            registers[regs_32[reg]] = num3;

                            dispWithSIB = "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_32[reg] + "\n";
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
            if (s == 0)
            {
                if (index == 4)
                {
                    memoryAccesses.push_back("add %" + regs_8[reg] + "," + to_string(disp) + "(%" + regs_32[base] + ")");

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
                    num2 = memories8bit[to_string((unsigned)(registers[regs_32[base]] + disp))];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string((unsigned)(registers[regs_32[base]] + disp)));

                    num3 = num1 + num2;
                    num4 = unsigned(num3);

                    memories8bit[to_string((unsigned)(registers[regs_32[base]] + disp))] = num3;
                    memoryAccesses.push_back("write " + to_string(num3) + " to " + to_string((unsigned)(registers[regs_32[base]] + disp)));

                    common.setOverflow8bit(num1, num2, num3, registers);
                    common.setCarry8bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    dispWithSIB = "%" + regs_8[reg] + "," + to_string(disp) + "(%" + regs_32[base] + ")" + "\n";
                }
                else
                {
                    memoryAccesses.push_back("add %" + regs_8[reg] + "," + to_string(disp) + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")");

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
                    num2 = memories8bit[to_string((unsigned)(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp))];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string((unsigned)(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp)));

                    num3 = num1 + num2;
                    num4 = unsigned(num3);

                    memories8bit[to_string((unsigned)(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp))] = num3;
                    memoryAccesses.push_back("write " + to_string(num3) + " to " + to_string((unsigned)(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp)));

                    common.setOverflow8bit(num1, num2, num3, registers);
                    common.setCarry8bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    dispWithSIB = "%" + regs_8[reg] + "," + to_string(disp) + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                }
            }
            else if (s == 1)
            {
                if (index == 4)
                {
                    int8_t num1, num2, num3;
                    uint8_t num4;

                    if (reg < 4)
                    {
                        num1 = common.get_bits(1, 8, registers[regs_32[reg]]);
                        num2 = memories8bit[to_string((unsigned)(registers[regs_32[base]] + disp))];
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string((unsigned)(registers[regs_32[base]] + disp)));

                        num3 = num1 + num2;
                        num4 = unsigned(num3);

                        registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (num3 & 0x000000ff);
                    }
                    else
                    {
                        num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                        num2 = memories8bit[to_string((unsigned)(registers[regs_32[base]] + disp))];
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string((unsigned)(registers[regs_32[base]] + disp)));

                        num3 = num1 + num2;
                        num4 = unsigned(num3);

                        registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff00ff) | (num3 & 0x0000ff00);
                    }

                    common.setOverflow8bit(num1, num2, num3, registers);
                    common.setCarry8bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    dispWithSIB = to_string(disp) + "(%" + regs_32[base] + "),%" + regs_8[reg] + "\n";
                }
                else
                {
                    int8_t num1, num2, num3;
                    uint8_t num4;

                    if (reg < 4)
                    {
                        num1 = common.get_bits(1, 8, registers[regs_32[reg]]);
                        num2 = memories8bit[to_string((unsigned)(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp))];
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string((unsigned)(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp)));

                        num3 = num1 + num2;
                        num4 = unsigned(num3);

                        registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (num3 & 0x000000ff);
                    }
                    else
                    {
                        num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                        num2 = memories8bit[to_string((unsigned)(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp))];
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string((unsigned)(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp)));

                        num3 = num1 + num2;
                        num4 = unsigned(num3);

                        registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff00ff) | (num3 & 0x0000ff00);
                    }

                    common.setOverflow8bit(num1, num2, num3, registers);
                    common.setCarry8bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    dispWithSIB = to_string(disp) + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_8[reg] + "\n";
                }
            }
        }
        else if (w == 1)
        {
            if (opSize)
            {
                if (s == 0)
                {
                    if (index == 4)
                    {
                        memoryAccesses.push_back("add %" + regs_16[reg] + "," + to_string(disp) + "(%" + regs_32[base] + ")");

                        int16_t num1, num2, num3;
                        uint16_t num4;

                        num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                        num2 = memories16bit[to_string((unsigned)(registers[regs_32[base]] + disp))];
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string((unsigned)(registers[regs_32[base]] + disp)));

                        num3 = num1 + num2;
                        num4 = unsigned(num3);

                        memories16bit[to_string((unsigned)(registers[regs_32[base]] + disp))] = num3;
                        memoryAccesses.push_back("write " + to_string(num3) + " to " + to_string((unsigned)(registers[regs_32[base]] + disp)));

                        common.setOverflow16bit(num1, num2, num3, registers);
                        common.setCarry16bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        dispWithSIB = "%" + regs_16[reg] + "," + to_string(disp) + "(%" + regs_32[base] + ")" + "\n";
                    }
                    else
                    {
                        memoryAccesses.push_back("add %" + regs_16[reg] + "," + to_string(disp) + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")");

                        int16_t num1, num2, num3;
                        uint16_t num4;

                        num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                        num2 = memories16bit[to_string((unsigned)(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp))];
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string((unsigned)(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp)));

                        num3 = num1 + num2;
                        num4 = unsigned(num3);

                        memories16bit[to_string((unsigned)(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp))] = num3;
                        memoryAccesses.push_back("write " + to_string(num3) + " to " + to_string((unsigned)(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp)));

                        common.setOverflow16bit(num1, num2, num3, registers);
                        common.setCarry16bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        dispWithSIB = "%" + regs_16[reg] + "," + to_string(disp) + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                    }
                }
                else if (s == 1)
                {
                    if (index == 4)
                    {
                        int16_t num1, num2, num3;
                        uint16_t num4;

                        num1 = common.get_bits(1, 16, registers[regs_32[reg]]);
                        num2 = memories16bit[to_string((unsigned)(registers[regs_32[base]] + disp))];
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string((unsigned)(registers[regs_32[base]] + disp)));

                        num3 = num1 + num2;
                        num4 = unsigned(num3);

                        registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff0000) | (num3 & 0x0000ffff);

                        common.setOverflow16bit(num1, num2, num3, registers);
                        common.setCarry16bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        dispWithSIB = to_string(disp) + "(%" + regs_32[base] + "),%" + regs_16[reg] + "\n";
                    }
                    else
                    {
                        int16_t num1, num2, num3;
                        uint16_t num4;

                        num1 = common.get_bits(1, 16, registers[regs_32[reg]]);
                        num2 = memories16bit[to_string((unsigned)(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp))];
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string((unsigned)(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp)));

                        num3 = num1 + num2;
                        num4 = unsigned(num3);

                        registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff0000) | (num3 & 0x0000ffff);

                        common.setOverflow16bit(num1, num2, num3, registers);
                        common.setCarry16bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        dispWithSIB = to_string(disp) + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_16[reg] + "\n";
                    }
                }
            }
            else
            {
                if (s == 0)
                {
                    if (index == 4)
                    {
                        memoryAccesses.push_back("add %" + regs_32[reg] + "," + to_string(disp) + "(%" + regs_32[base] + ")");

                        int num1 = memories32bit[to_string((unsigned)(registers[regs_32[base]] + disp))];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + to_string((unsigned)(registers[regs_32[base]] + disp)));

                        int num2 = registers[regs_32[reg]];
                        int num3 = num1 + num2;
                        unsigned int num4 = unsigned(num3);

                        common.setOverflow32bit(num1, num2, num3, registers);
                        common.setCarry32bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        memories32bit[to_string((unsigned)(registers[regs_32[base]] + disp))] = num3;
                        memoryAccesses.push_back("write " + to_string(num3) + " to " + to_string((unsigned)(registers[regs_32[base]] + disp)));

                        dispWithSIB = "%" + regs_32[reg] + "," + to_string(disp) + "(%" + regs_32[base] + ")" + "\n";
                    }
                    else
                    {
                        memoryAccesses.push_back("add %" + regs_32[reg] + "," + to_string(disp) + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")");

                        int num1 = memories32bit[to_string((unsigned)(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp))];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + to_string((unsigned)(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp)));

                        int num2 = registers[regs_32[reg]];
                        int num3 = num1 + num2;
                        unsigned int num4 = unsigned(num3);

                        common.setOverflow32bit(num1, num2, num3, registers);
                        common.setCarry32bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        memories32bit[to_string((unsigned)(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp))] = num3;
                        memoryAccesses.push_back("write " + to_string(num3) + " to " + to_string((unsigned)(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp)));

                        dispWithSIB = "%" + regs_32[reg] + "," + to_string(disp) + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                    }
                }
                else
                {
                    if (index == 4)
                    {
                        int num1 = memories32bit[to_string((unsigned)(registers[regs_32[base]] + disp))];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + to_string((unsigned)(registers[regs_32[base]] + disp)));

                        int num2 = registers[regs_32[reg]];
                        int num3 = num1 + num2;
                        unsigned int num4 = unsigned(num3);

                        common.setOverflow32bit(num1, num2, num3, registers);
                        common.setCarry32bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        registers[regs_32[reg]] = num3;

                        dispWithSIB = to_string(disp) + "(%" + regs_32[base] + ")" + ",%" + regs_32[reg] + "\n";
                    }
                    else
                    {
                        int num1 = memories32bit[to_string((unsigned)(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp))];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + to_string((unsigned)(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp)));

                        int num2 = registers[regs_32[reg]];
                        int num3 = num1 + num2;
                        unsigned int num4 = unsigned(num3);

                        common.setOverflow32bit(num1, num2, num3, registers);
                        common.setCarry32bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        registers[regs_32[reg]] = num3;

                        dispWithSIB = to_string(disp) + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_32[reg] + "\n";
                    }
                }
            }
        }
    }
    return dispWithSIB;
}

string Immediate::decode_displacement_without_SIB(int w, int s, int mod, int reg, int rm)
{
    string dispWithoutSIB = "";
    int disp_bytes[] = {4, 1, 4};
    int bytes = disp_bytes[mod];

    unsigned int disp = common.assemble_bits(bytes, false, instruction, registers);

    if (mod == 0)
    {
        if (w == 0)
        {
            if (s == 0)
            {
                memoryAccesses.push_back("add %" + regs_8[reg] + "," + to_string(disp));

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
                num2 = memories8bit[to_string(disp)];
                memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string(disp));

                num3 = num1 + num2;
                num4 = unsigned(num3);

                memories8bit[to_string(disp)] = num3;
                memoryAccesses.push_back("write " + to_string(num3) + " to " + to_string(disp));

                common.setOverflow8bit(num1, num2, num3, registers);
                common.setCarry8bit(num1, num4, registers);
                common.setSign(num3, registers);
                common.setZero(num3, registers);

                dispWithoutSIB = "%" + regs_8[reg] + "," + to_string(disp) + "\n";
            }
            else if (s == 1)
            {
                int8_t num1, num2, num3;
                uint8_t num4;

                if (reg < 4)
                {
                    num1 = common.get_bits(1, 8, registers[regs_32[reg]]);
                    num2 = memories8bit[to_string(disp)];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string(disp));

                    num3 = num1 + num2;
                    num4 = unsigned(num3);

                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (num3 & 0x000000ff);
                }
                else
                {
                    num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                    num2 = memories8bit[to_string(disp)];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string(disp));

                    num3 = num1 + num2;
                    num4 = unsigned(num3);

                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff00ff) | (num3 & 0x0000ff00);
                }

                common.setOverflow8bit(num1, num2, num3, registers);
                common.setCarry8bit(num1, num4, registers);
                common.setSign(num3, registers);
                common.setZero(num3, registers);

                dispWithoutSIB = to_string(disp) + ",%" + regs_8[reg] + "\n";
            }
        }
        else if (w == 1)
        {
            if (opSize)
            {
                if (s == 0)
                {
                    memoryAccesses.push_back("add %" + regs_16[reg] + "," + to_string(disp));

                    int16_t num1, num2, num3;
                    uint16_t num4;

                    num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                    num2 = memories16bit[to_string(disp)];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string(disp));

                    num3 = num1 + num2;
                    num4 = unsigned(num3);

                    memories16bit[to_string(disp)] = num3;
                    memoryAccesses.push_back("write " + to_string(num3) + " to " + to_string(disp));

                    common.setOverflow16bit(num1, num2, num3, registers);
                    common.setCarry16bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    dispWithoutSIB = "%" + regs_16[reg] + "," + to_string(disp) + "\n";
                }
                else if (s == 1)
                {
                    int16_t num1, num2, num3;
                    uint16_t num4;

                    num1 = common.get_bits(1, 16, registers[regs_32[reg]]);
                    num2 = memories16bit[to_string(disp)];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string(disp));

                    num3 = num1 + num2;
                    num4 = unsigned(num3);

                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff0000) | (num3 & 0x0000ffff);

                    common.setOverflow16bit(num1, num2, num3, registers);
                    common.setCarry16bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    dispWithoutSIB = to_string(disp) + ",%" + regs_16[reg] + "\n";
                }
            }
            else
            {
                if (s == 0)
                {
                    memoryAccesses.push_back("add %" + regs_32[reg] + "," + to_string(disp));

                    int num1 = memories32bit[to_string(disp)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + to_string(disp));

                    int num2 = registers[regs_32[reg]];
                    int num3 = num1 + num2;
                    unsigned int num4 = unsigned(num3);

                    common.setOverflow32bit(num1, num2, num3, registers);
                    common.setCarry32bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    memories32bit[to_string(disp)] = num3;
                    memoryAccesses.push_back("write " + to_string(num3) + " to " + to_string(disp));

                    dispWithoutSIB = "%" + regs_32[reg] + "," + to_string(disp) + "\n";
                }
                else
                {
                    int num1 = memories32bit[to_string(disp)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + to_string(disp));

                    int num2 = registers[regs_32[reg]];
                    int num3 = num1 + num2;
                    unsigned int num4 = unsigned(num3);

                    common.setOverflow32bit(num1, num2, num3, registers);
                    common.setCarry32bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    registers[regs_32[reg]] = num3;

                    dispWithoutSIB = to_string(disp) + ",%" + regs_32[reg] + "\n";
                }
            }
        }
    }
    else
    {
        if (w == 0)
        {
            if (s == 0)
            {
                memoryAccesses.push_back("add %" + regs_8[reg] + "," + to_string(disp) + "(%" + regs_32[rm] + ")");

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
                num2 = memories8bit[to_string((unsigned)(registers[regs_32[rm]] + disp))];
                memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string((unsigned)(registers[regs_32[rm]] + disp)));

                num3 = num1 + num2;
                num4 = unsigned(num3);

                memories8bit[to_string((unsigned)(registers[regs_32[rm]] + disp))] = num3;
                memoryAccesses.push_back("write " + to_string(num3) + " to " + to_string((unsigned)(registers[regs_32[rm]] + disp)));

                common.setOverflow8bit(num1, num2, num3, registers);
                common.setCarry8bit(num1, num4, registers);
                common.setSign(num3, registers);
                common.setZero(num3, registers);

                dispWithoutSIB = "%" + regs_8[reg] + "," + to_string(disp) + "(%" + regs_32[rm] + ") \n";
            }
            else if (s == 1)
            {
                int8_t num1, num2, num3;
                uint8_t num4;

                if (reg < 4)
                {
                    num1 = common.get_bits(1, 8, registers[regs_32[reg]]);
                    num2 = memories8bit[to_string((unsigned)(registers[regs_32[rm]] + disp))];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string((unsigned)(registers[regs_32[rm]] + disp)));

                    num3 = num1 + num2;
                    num4 = unsigned(num3);

                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (num3 & 0x000000ff);
                }
                else
                {
                    num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                    num2 = memories8bit[to_string((unsigned)(registers[regs_32[rm]] + disp))];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string((unsigned)(registers[regs_32[rm]] + disp)));

                    num3 = num1 + num2;
                    num4 = unsigned(num3);

                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff00ff) | (num3 & 0x0000ff00);
                }

                common.setOverflow8bit(num1, num2, num3, registers);
                common.setCarry8bit(num1, num4, registers);
                common.setSign(num3, registers);
                common.setZero(num3, registers);

                dispWithoutSIB = to_string(disp) + "(%" + regs_32[rm] + "),%" + regs_8[reg] + "\n";
            }
        }
        else if (w == 1)
        {
            if (opSize)
            {
                if (s == 0)
                {
                    memoryAccesses.push_back("add %" + regs_16[reg] + "," + to_string(disp) + "(%" + regs_32[rm] + ")");

                    int16_t num1, num2, num3;
                    uint16_t num4;

                    num1 = common.get_bits(1, 16, registers[regs_32[reg]]);
                    num2 = memories16bit[to_string((unsigned)(registers[regs_32[rm]] + disp))];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string((unsigned)(registers[regs_32[rm]] + disp)));

                    num3 = num1 + num2;
                    num4 = unsigned(num3);

                    memories16bit[to_string((unsigned)(registers[regs_32[rm]] + disp))] = num3;
                    memoryAccesses.push_back("write " + to_string(num3) + " to " + to_string((unsigned)(registers[regs_32[rm]] + disp)));

                    common.setOverflow16bit(num1, num2, num3, registers);
                    common.setCarry16bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    dispWithoutSIB = "%" + regs_16[reg] + "," + to_string(disp) + "(%" + regs_32[rm] + ") \n";
                }
                else if (s == 1)
                {
                    int16_t num1, num2, num3;
                    uint16_t num4;

                    num1 = common.get_bits(1, 16, registers[regs_32[reg]]);
                    num2 = memories16bit[to_string((unsigned)(registers[regs_32[rm]] + disp))];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string((unsigned)(registers[regs_32[rm]] + disp)));

                    num3 = num1 + num2;
                    num4 = unsigned(num3);

                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff0000) | (num3 & 0x0000ffff);

                    common.setOverflow16bit(num1, num2, num3, registers);
                    common.setCarry16bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    dispWithoutSIB = to_string(disp) + "(%" + regs_32[rm] + "),%" + regs_16[reg] + "\n";
                }
            }
            else
            {
                if (s == 0)
                {
                    int num1 = memories32bit[to_string((unsigned)(registers[regs_32[rm]] + disp))];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + to_string((unsigned)(registers[regs_32[rm]] + disp)));

                    int num2 = registers[regs_32[reg]];
                    int num3 = num1 + num2;
                    unsigned int num4 = unsigned(num3);

                    common.setOverflow32bit(num1, num2, num3, registers);
                    common.setCarry32bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    memories32bit[to_string((unsigned)(registers[regs_32[rm]] + disp))] = num3;

                    dispWithoutSIB = "%" + regs_32[reg] + "," + to_string(disp) + "(%" + regs_32[rm] + ") \n";
                }
                else
                {
                    int num1 = memories32bit[to_string((unsigned)(registers[regs_32[rm]] + disp))];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + to_string((unsigned)(registers[regs_32[rm]] + disp)));

                    int num2 = registers[regs_32[reg]];
                    int num3 = num1 + num2;
                    unsigned int num4 = unsigned(num3);

                    common.setOverflow32bit(num1, num2, num3, registers);
                    common.setCarry32bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    registers[regs_32[reg]] = num3;

                    dispWithoutSIB = to_string(disp) + "(%" + regs_32[rm] + "),%" + regs_32[reg] + "\n";
                }
            }
        }
    }

    return dispWithoutSIB;
};

string Immediate::decode_SIB(int w, int s, int mod, int reg)
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

    stringSib = decode_displacement_with_SIB(s, d, mod, reg, index, scale, base);
    return stringSib;
}

string Immediate::decode_mod_00(int w, int s, int rm)
{
    string string00 = "";
    if (rm == 4)
    {
        string00 = decode_SIB(s, d, 0, reg);
    }
    else if (rm == 5)
    {
        string00 = decode_displacement_without_SIB(s, d, 0, reg, 5);
    }
    else
    {
        if (w == 0)
        {
            if (s == 0)
            {
                memoryAccesses.push_back("add %" + regs_8[reg] + ",(%" + regs_32[rm] + ")");
                string00 = "%" + regs_8[reg] + ",(%" + regs_32[rm] + ")\n";

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
                num2 = memories8bit[to_string((unsigned)registers[regs_32[rm]])];
                memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string((unsigned)registers[regs_32[rm]]));

                num3 = num1 + num2;
                num4 = unsigned(num3);

                memories8bit[to_string((unsigned)registers[regs_32[rm]])] = num3;
                memoryAccesses.push_back("write " + to_string(num3) + " to " + to_string((unsigned)registers[regs_32[rm]]));

                common.setOverflow8bit(num1, num2, num3, registers);
                common.setCarry8bit(num1, num4, registers);
                common.setSign(num3, registers);
                common.setZero(num3, registers);
            }
            else if (s == 1)
            {
                string00 = "(%" + regs_32[rm] + "),%" + regs_8[reg] + "\n";

                int8_t num1, num2, num3;
                uint8_t num4;

                if (reg < 4)
                {
                    num1 = common.get_bits(1, 8, registers[regs_32[reg]]);
                    num2 = memories8bit[to_string((unsigned)registers[regs_32[rm]])];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string((unsigned)registers[regs_32[rm]]));

                    num3 = num1 + num2;
                    num4 = unsigned(num3);

                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (num3 & 0x000000ff);
                }
                else
                {
                    num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                    num2 = memories8bit[to_string((unsigned)registers[regs_32[rm]])];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string((unsigned)registers[regs_32[rm]]));

                    num3 = num1 + num2;
                    num4 = unsigned(num3);

                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff00ff) | (num3 & 0x0000ff00);
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
                if (s == 0)
                {
                    memoryAccesses.push_back("add %" + regs_16[reg] + ",(%" + regs_32[rm] + ")");

                    string00 = "%" + regs_16[reg] + ",(%" + regs_32[rm] + ")\n";

                    int16_t num1, num2, num3;
                    uint16_t num4;

                    num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                    num2 = memories16bit[to_string((unsigned)registers[regs_32[rm]])];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string((unsigned)registers[regs_32[rm]]));

                    num3 = num1 + num2;
                    num4 = unsigned(num3);

                    memories16bit[to_string((unsigned)registers[regs_32[rm]])] = num3;
                    memoryAccesses.push_back("write " + to_string(num3) + " to " + to_string((unsigned)registers[regs_32[rm]]));

                    common.setOverflow16bit(num1, num2, num3, registers);
                    common.setCarry16bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);
                }
                else if (s == 1)
                {
                    string00 = "(%" + regs_32[rm] + "),%" + regs_16[reg] + "\n";

                    int16_t num1, num2, num3;
                    uint16_t num4;

                    num1 = common.get_bits(1, 16, registers[regs_32[reg]]);
                    num2 = memories8bit[to_string((unsigned)registers[regs_32[rm]])];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + to_string((unsigned)registers[regs_32[rm]]));

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
                if (s == 0)
                {
                    memoryAccesses.push_back("add %" + regs_32[reg] + ",(%" + regs_32[rm] + ")");

                    int num1 = memories32bit[to_string((unsigned)registers[regs_32[reg]])];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + to_string((unsigned)registers[regs_32[reg]]));

                    int num2 = registers[regs_32[reg]]; //registers["ECX"]
                    int num3 = num1 + num2;
                    unsigned int num4 = unsigned(num3);

                    common.setOverflow32bit(num1, num2, num3, registers);
                    common.setCarry32bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    string00 = "%" + regs_32[reg] + ",(%" + regs_32[rm] + ")\n";
                    memories32bit[to_string((unsigned)registers[regs_32[rm]])] = num3;
                    memoryAccesses.push_back("write " + to_string(num3) + " to " + to_string((unsigned)registers[regs_32[rm]]));
                }
                else
                {
                    int num1 = memories32bit[to_string((unsigned)registers[regs_32[rm]])];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + to_string((unsigned)registers[regs_32[rm]]));

                    int num2 = registers[regs_32[reg]];
                    int num3 = num1 + num2;
                    unsigned int num4 = unsigned(num3);

                    common.setOverflow32bit(num1, num2, num3, registers);
                    common.setCarry32bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    string00 = "(%" + regs_32[rm] + "),%" + regs_32[reg] + "\n";
                    registers[regs_32[reg]] = num3;
                }
            }
        }
    }

    return string00;
}

string Immediate::decode_mod_01(int w, int s, int rm)
{
    string string01 = "";
    if (rm == 4)
    {
        string01 = decode_SIB(s, d, 1, reg);
    }
    else
    {
        string01 = decode_displacement_without_SIB(s, d, 1, reg, rm);
    }
    return string01;
}

string Immediate::decode_mod_10(int w, int s, int rm)
{
    string string10 = "";
    if (rm == 4)
    {
        string10 = decode_SIB(s, d, 2, reg);
    }
    else
    {
        string10 = decode_displacement_without_SIB(s, d, 2, reg, rm);
    }
    return string10;
}

string Immediate::decode_mod_11(int w, int s, int rm)
{
    string string11 = "";
    if (w == 0)
    {

        //printf("w:0 and d:0 \n");

        int8_t num1, num2, num3;
        uint8_t num4;

        num1 = common.assemble_bits(1, true, instruction, registers);

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

        //cout << "num1: " << dec << signed(num1) << ", num2: " << dec << signed(num2) << ", num3: " << dec << signed(num3) << "\n";

        common.setOverflow8bit(num1, num2, num3, registers);
        common.setCarry8bit(num1, num4, registers);
        common.setSign(num3, registers);
        common.setZero(num3, registers);

        string11 = "$" + to_string(num1) + ",%" + regs_8[rm] + "\n";
    }
    else if (w == 1)
    {
        if (opSize)
        {
            if (s == 0)
            {
                //printf("w:0 and d:0 \n");

                int16_t num1, num2, num3;
                uint16_t num4;

                //num1 = common.get_bits(1, 16, registers[regs_32[reg]]);
                num2 = common.get_bits(1, 16, registers[regs_32[rm]]);
                num3 = num1 + num2;
                num4 = unsigned(num3);
                registers[regs_32[rm]] = ((registers[regs_32[rm]]) & 0xffff0000) | (num3 & 0x0000ffff);

                //cout << "num1: " << dec << signed(num1) << ", num2: " << dec << signed(num2) << ", num3: " << dec << signed(num3) << "\n";

                common.setOverflow16bit(num1, num2, num3, registers);
                common.setCarry16bit(num1, num4, registers);
                common.setSign(num3, registers);
                common.setZero(num3, registers);

                //string11 = "%" + regs_16[reg] + ",%" + regs_16[rm] + "\n";
            }
            else if (s == 1)
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

                string11 = "%" + regs_16[rm] + ",%" + regs_16[reg] + "\n";
            }
        }
        else
        {
            if (s == 0)
            {
                //printf("w:1 and d:0 \n");
                //int num1 = registers[regs_32[reg]];
                int num2 = registers[regs_32[rm]];
                int num3 = num1 + num2;
                unsigned int num4 = unsigned(num3);
                registers[regs_32[rm]] = num3;

                //cout << "num1: " << num1 << ", num2: " << num2 << ", num3: " << num3 << ", num4: " << num4 << "\n";

                common.setOverflow32bit(num1, num2, num3, registers);
                common.setCarry32bit(num1, num4, registers);
                common.setSign(num3, registers);
                common.setZero(num3, registers);

                //string11 = "%" + regs_32[reg] + ",%" + regs_32[rm] + "\n";
            }
            else
            {
                //printf("w:1 and d:1 \n");
                //int num1 = registers[regs_32[reg]];
                int num2 = registers[regs_32[rm]];
                int num3 = num1 + num2;
                unsigned int num4 = unsigned(num3);
                registers[regs_32[reg]] = num3;

                //cout << "num1: " << dec << num1 << ", num2: " << dec << num2 << ", num3: " << dec << num3 << "\n";

                common.setOverflow32bit(num1, num2, num3, registers);
                common.setCarry32bit(num1, num4, registers);
                common.setSign(num3, registers);
                common.setZero(num3, registers);

                string11 = "%" + regs_32[rm] + ",%" + regs_32[reg] + "\n";
            }
        }
    }

    return string11;
}
*/
string Immediate::decode_imm(short prefixes[4], int w, int s, int mod, int rm)
{
    string decoded_bytes;

    if (prefixes[2] == 0x66)
    {
        opSize = true;
    }
    else
    {
        opSize = false;
    }

    if (mod == 0)
    {
        decoded_bytes = decode_mod_00(w, s, rm);
    }
    else if (mod == 1)
    {
        decoded_bytes = decode_mod_01(w, s, rm);
    }
    else if (mod == 2)
    {
        decoded_bytes = decode_mod_10(w, s, rm);
    }
    else
    {
        decoded_bytes = decode_mod_11(w, s, rm);
    }

    return decoded_bytes;
}