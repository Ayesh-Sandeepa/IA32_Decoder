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









Adder_addOverride::Adder_addOverride(Common com, queue<short> &instruction, map<string, int> &registers, map<string, int> &memories32bit, map<string, int16_t> &memories16bit, map<string, int8_t> &memories8bit, list<string> &memoryAccesses)
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
    regs_8[0]="CL";
    regs_8[0]="DL";
    regs_8[0]="BL";
    regs_8[0]="AH";
    regs_8[0]="CH";
    regs_8[0]="DH";
    regs_8[0]="BH";

   
    regs_16bitmode[0]="BX+SI";
    regs_16bitmode[1]="BX+DI";
    regs_16bitmode[2]="BP+SI";
    regs_16bitmode[3]="BP+DI";
    regs_16bitmode[4]="SI";
    regs_16bitmode[5]="DI";
    regs_16bitmode[6]="BP";
    regs_16bitmode[7]="BX";


    list1[0] = (uint16_t *)&registers["EBX"];
    list1[1] = (uint16_t *)&registers["EBP"];

    list2[0] = (uint16_t *)&registers["ESI"];
    list2[1] = (uint16_t *)&registers["EDI"];

    list3[0] = (uint16_t *)&registers["ESI"];
    list3[1] = (uint16_t *)&registers["EDI"];
    list3[2] = (uint16_t *)&registers["EBP"];
    list3[3] = (uint16_t *)&registers["EBX"];
}

/*
string Adder_addOverride::decode_displacement_with_SIB(int w, int d, int mod, int reg, int index, int scale, int base)
{
    string dispWithSIB = "";

    int disp_bytes[] = {2, 1, 2};
    int bytes = disp_bytes[mod];

    int disp;

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
                if (d == 0)
                {
                    if (index == 4)
                    {
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
                        num3 = num1 + num2;
                        num4 = unsigned(num1) + unsigned(num2);

                        memories8bit[to_string(disp)] = num3;

                        common.setOverflow8bit(num1, num2, num3, registers);
                        common.setCarry8bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        dispWithSIB = "%" + regs_8[reg] + "," + to_string(disp) + "\n";
                    }
                    else
                    {
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
                        num2 = memories8bit[to_string(registers[regs_32[index]] * 2 + disp)];
                        num3 = num1 + num2;
                        num4 = unsigned(num1) + unsigned(num2);

                        memories8bit[to_string(registers[regs_32[index]] * scale + disp)] = num3;

                        common.setOverflow8bit(num1, num2, num3, registers);
                        common.setCarry8bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        dispWithSIB = "%" + regs_8[reg] + "," + to_string(disp) + "(,%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
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
                            num2 = memories8bit[to_string(disp)];
                            num3 = num1 + num2;
                            num4 = unsigned(num1) + unsigned(num2);

                            registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (num3 & 0x000000ff);
                        }
                        else
                        {
                            num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                            num2 = memories8bit[to_string(disp)];
                            num3 = num1 + num2;
                            num4 = unsigned(num1) + unsigned(num2);

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
                            num2 = memories8bit[to_string(registers[regs_32[index]] * scale + disp)];
                            num3 = num1 + num2;
                            num4 = unsigned(num1) + unsigned(num2);

                            registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (num3 & 0x000000ff);
                        }
                        else
                        {
                            num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                            num2 = memories8bit[to_string(registers[regs_32[index]] * scale + disp)];
                            num3 = num1 + num2;
                            num4 = unsigned(num1) + unsigned(num2);

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
                    if (d == 0)
                    {
                        if (index == 4)
                        {
                            int16_t num1, num2, num3;
                            uint16_t num4;

                            num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                            num2 = memories16bit[to_string(disp)];
                            num3 = num1 + num2;
                            num4 = unsigned(num1) + unsigned(num2);

                            memories16bit[to_string(disp)] = num3;

                            common.setOverflow16bit(num1, num2, num3, registers);
                            common.setCarry16bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            dispWithSIB = "%" + regs_16[reg] + "," + to_string(disp) + "\n";
                        }
                        else
                        {
                            int16_t num1, num2, num3;
                            uint16_t num4;

                            num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                            num2 = memories16bit[to_string(registers[regs_32[index]] * 2 + disp)];
                            num3 = num1 + num2;
                            num4 = unsigned(num1) + unsigned(num2);

                            memories16bit[to_string(registers[regs_32[index]] * scale + disp)] = num3;

                            common.setOverflow16bit(num1, num2, num3, registers);
                            common.setCarry16bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            dispWithSIB = "%" + regs_16[reg] + "," + to_string(disp) + "(,%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                        }
                    }
                    else if (d == 1)
                    {
                        if (index == 4)
                        {
                            int16_t num1, num2, num3;
                            uint16_t num4;

                            num1 = common.get_bits(1, 16, registers[regs_32[reg]]);
                            num2 = memories16bit[to_string(disp)];
                            num3 = num1 + num2;
                            num4 = unsigned(num1) + unsigned(num2);

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
                            num2 = memories16bit[to_string(registers[regs_32[index]] * scale + disp)];
                            num3 = num1 + num2;
                            num4 = unsigned(num1) + unsigned(num2);

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
                    if (w == 1 and d == 0)
                    {
                        if (index == 4)
                        {
                            int num1 = memories32bit[to_string(disp)];
                            int num2 = registers[regs_32[reg]];
                            int num3 = num1 + num2;
                            unsigned int num4 = unsigned(num1) + unsigned(num2);

                            common.setOverflow32bit(num1, num2, num3, registers);
                            common.setCarry32bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            memories32bit[to_string(disp)] = num3;

                            dispWithSIB = "%" + regs_32[reg] + "," + to_string(disp) + "\n";
                        }
                        else
                        {
                            int num1 = memories32bit[to_string(registers[regs_32[index]] * scale + disp)];
                            int num2 = registers[regs_32[reg]];
                            int num3 = num1 + num2;
                            unsigned int num4 = unsigned(num1) + unsigned(num2);

                            common.setOverflow32bit(num1, num2, num3, registers);
                            common.setCarry32bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            memories32bit[to_string(registers[regs_32[index]] * scale + disp)] = num3;

                            dispWithSIB = "%" + regs_32[reg] + "," + to_string(disp) + "(,%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                        }
                    }
                    else
                    {
                        if (index == 4)
                        {
                            int num1 = memories32bit[to_string(disp)];
                            int num2 = registers[regs_32[reg]];
                            int num3 = num1 + num2;
                            unsigned int num4 = unsigned(num1) + unsigned(num2);

                            common.setOverflow32bit(num1, num2, num3, registers);
                            common.setCarry32bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            registers[regs_32[reg]] = num3;

                            dispWithSIB = to_string(disp) + ",%" + regs_32[reg] + "\n";
                        }
                        else
                        {

                            int num1 = memories32bit[to_string(registers[regs_32[index]] * scale + disp)];
                            int num2 = registers[regs_32[reg]];
                            int num3 = num1 + num2;
                            unsigned int num4 = unsigned(num1) + unsigned(num2);

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
                if (d == 0)
                {
                    if (index == 4)
                    {

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
                        num2 = memories8bit[to_string(registers[regs_32[base]])];
                        num3 = num1 + num2;
                        num4 = unsigned(num1) + unsigned(num2);

                        memories8bit[to_string(registers[regs_32[base]])] = num3;

                        common.setOverflow8bit(num1, num2, num3, registers);
                        common.setCarry8bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        dispWithSIB = "%" + regs_8[reg] + "," + "(%" + regs_32[base] + ")" + "\n";
                    }
                    else
                    {

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
                        num2 = memories8bit[to_string(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp)];
                        num3 = num1 + num2;
                        num4 = unsigned(num1) + unsigned(num2);

                        memories8bit[to_string(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp)] = num3;

                        common.setOverflow8bit(num1, num2, num3, registers);
                        common.setCarry8bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        dispWithSIB = "%" + regs_8[reg] + "," + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
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
                            num2 = memories8bit[to_string(registers[regs_32[base]])];
                            num3 = num1 + num2;
                            num4 = unsigned(num1) + unsigned(num2);

                            registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (num3 & 0x000000ff);
                        }
                        else
                        {
                            num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                            num2 = memories8bit[to_string(registers[regs_32[base]])];
                            num3 = num1 + num2;
                            num4 = unsigned(num1) + unsigned(num2);

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
                            num2 = memories8bit[to_string(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp)];
                            num3 = num1 + num2;
                            num4 = unsigned(num1) + unsigned(num2);

                            registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (num3 & 0x000000ff);
                        }
                        else
                        {
                            num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                            num2 = memories8bit[to_string(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp)];
                            num3 = num1 + num2;
                            num4 = unsigned(num1) + unsigned(num2);

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
                    if (d == 0)
                    {
                        if (index == 4)
                        {

                            int16_t num1, num2, num3;
                            uint16_t num4;

                            num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                            num2 = memories16bit[to_string(registers[regs_32[base]])];
                            num3 = num1 + num2;
                            num4 = unsigned(num1) + unsigned(num2);

                            memories16bit[to_string(registers[regs_32[base]])] = num3;

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

                            num2 = memories16bit[to_string(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp)];
                            num3 = num1 + num2;
                            num4 = unsigned(num1) + unsigned(num2);

                            memories16bit[to_string(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp)] = num3;

                            common.setOverflow16bit(num1, num2, num3, registers);
                            common.setCarry16bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            dispWithSIB = "%" + regs_16[reg] + "," + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                        }
                    }
                    else if (d == 1)
                    {
                        if (index == 4)
                        {

                            int16_t num1, num2, num3;
                            uint16_t num4;

                            num1 = common.get_bits(1, 16, registers[regs_32[reg]]);
                            num2 = memories16bit[to_string(registers[regs_32[base]])];
                            num3 = num1 + num2;
                            num4 = unsigned(num1) + unsigned(num2);

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
                            num2 = memories16bit[to_string(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp)];
                            num3 = num1 + num2;
                            num4 = unsigned(num1) + unsigned(num2);

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
                    if (d == 0)
                    {
                        if (index == 4)
                        {
                            int num1 = memories32bit[to_string(registers[regs_32[base]])];
                            int num2 = registers[regs_32[reg]];
                            int num3 = num1 + num2;
                            unsigned int num4 = unsigned(num1) + unsigned(num2);

                            common.setOverflow32bit(num1, num2, num3, registers);
                            common.setCarry32bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            memories32bit[to_string(registers[regs_32[base]])] = num3;

                            dispWithSIB = "%" + regs_32[reg] + "," + "(%" + regs_32[base] + ")" + "\n";
                        }
                        else
                        {
                            int num1 = memories32bit[to_string(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp)];
                            int num2 = registers[regs_32[reg]];
                            int num3 = num1 + num2;
                            unsigned int num4 = unsigned(num1) + unsigned(num2);

                            common.setOverflow32bit(num1, num2, num3, registers);
                            common.setCarry32bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            memories32bit[to_string(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp)] = num3;

                            dispWithSIB = "%" + regs_32[reg] + "," + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                        }
                    }
                    else
                    {
                        if (index == 4)
                        {
                            int num1 = memories32bit[to_string(registers[regs_32[base]])];
                            int num2 = registers[regs_32[reg]];
                            int num3 = num1 + num2;
                            unsigned int num4 = unsigned(num1) + unsigned(num2);

                            common.setOverflow32bit(num1, num2, num3, registers);
                            common.setCarry32bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            registers[regs_32[reg]] = num3;

                            dispWithSIB = "(%" + regs_32[base] + ")" + ",%" + regs_32[reg] + "\n";
                        }
                        else
                        {
                            int num1 = memories32bit[to_string(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp)];
                            int num2 = registers[regs_32[reg]];
                            int num3 = num1 + num2;
                            unsigned int num4 = unsigned(num1) + unsigned(num2);

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
            if (d == 0)
            {
                if (index == 4)
                {
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
                    num2 = memories8bit[to_string(registers[regs_32[base]] + disp)];
                    num3 = num1 + num2;
                    num4 = unsigned(num1) + unsigned(num2);

                    memories8bit[to_string(registers[regs_32[base]] + disp)] = num3;

                    common.setOverflow8bit(num1, num2, num3, registers);
                    common.setCarry8bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    dispWithSIB = "%" + regs_8[reg] + "," + to_string(disp) + "(%" + regs_32[base] + ")" + "\n";
                }
                else
                {
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
                    num2 = memories8bit[to_string(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp)];
                    num3 = num1 + num2;
                    num4 = unsigned(num1) + unsigned(num2);

                    memories8bit[to_string(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp)] = num3;

                    common.setOverflow8bit(num1, num2, num3, registers);
                    common.setCarry8bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    dispWithSIB = "%" + regs_8[reg] + "," + to_string(disp) + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
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
                        num2 = memories8bit[to_string(registers[regs_32[base]] + disp)];
                        num3 = num1 + num2;
                        num4 = unsigned(num1) + unsigned(num2);

                        registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (num3 & 0x000000ff);
                    }
                    else
                    {
                        num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                        num2 = memories8bit[to_string(registers[regs_32[base]] + disp)];
                        num3 = num1 + num2;
                        num4 = unsigned(num1) + unsigned(num2);

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
                        num2 = memories8bit[to_string(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp)];
                        num3 = num1 + num2;
                        num4 = unsigned(num1) + unsigned(num2);

                        registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (num3 & 0x000000ff);
                    }
                    else
                    {
                        num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                        num2 = memories8bit[to_string(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp)];
                        num3 = num1 + num2;
                        num4 = unsigned(num1) + unsigned(num2);

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
                if (d == 0)
                {
                    if (index == 4)
                    {
                        int16_t num1, num2, num3;
                        uint16_t num4;

                        num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                        num2 = memories16bit[to_string(registers[regs_32[base]] + disp)];
                        num3 = num1 + num2;
                        num4 = unsigned(num1) + unsigned(num2);

                        memories16bit[to_string(registers[regs_32[base]] + disp)] = num3;

                        common.setOverflow16bit(num1, num2, num3, registers);
                        common.setCarry16bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        dispWithSIB = "%" + regs_16[reg] + "," + to_string(disp) + "(%" + regs_32[base] + ")" + "\n";
                    }
                    else
                    {
                        int16_t num1, num2, num3;
                        uint16_t num4;

                        num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                        num2 = memories16bit[to_string(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp)];
                        num3 = num1 + num2;
                        num4 = unsigned(num1) + unsigned(num2);

                        memories16bit[to_string(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp)] = num3;

                        common.setOverflow16bit(num1, num2, num3, registers);
                        common.setCarry16bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        dispWithSIB = "%" + regs_16[reg] + "," + to_string(disp) + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                    }
                }
                else if (d == 1)
                {
                    if (index == 4)
                    {
                        int16_t num1, num2, num3;
                        uint16_t num4;

                        num1 = common.get_bits(1, 16, registers[regs_32[reg]]);
                        num2 = memories16bit[to_string(registers[regs_32[base]] + disp)];
                        num3 = num1 + num2;
                        num4 = unsigned(num1) + unsigned(num2);

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
                        num2 = memories16bit[to_string(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp)];
                        num3 = num1 + num2;
                        num4 = unsigned(num1) + unsigned(num2);

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
                if (d == 0)
                {
                    if (index == 4)
                    {
                        int num1 = memories32bit[to_string(registers[regs_32[base]] + disp)];
                        int num2 = registers[regs_32[reg]];
                        int num3 = num1 + num2;
                        unsigned int num4 = unsigned(num1) + unsigned(num2);

                        common.setOverflow32bit(num1, num2, num3, registers);
                        common.setCarry32bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        memories32bit[to_string(registers[regs_32[base]] + disp)] = num3;

                        dispWithSIB = "%" + regs_32[reg] + "," + to_string(disp) + "(%" + regs_32[base] + ")" + "\n";
                    }
                    else
                    {
                        int num1 = memories32bit[to_string(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp)];
                        int num2 = registers[regs_32[reg]];
                        int num3 = num1 + num2;
                        unsigned int num4 = unsigned(num1) + unsigned(num2);

                        common.setOverflow32bit(num1, num2, num3, registers);
                        common.setCarry32bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        memories32bit[to_string(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp)] = num3;

                        dispWithSIB = "%" + regs_32[reg] + "," + to_string(disp) + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                    }
                }
                else
                {
                    if (index == 4)
                    {
                        int num1 = memories32bit[to_string(registers[regs_32[base]] + disp)];
                        int num2 = registers[regs_32[reg]];
                        int num3 = num1 + num2;
                        unsigned int num4 = unsigned(num1) + unsigned(num2);

                        common.setOverflow32bit(num1, num2, num3, registers);
                        common.setCarry32bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        registers[regs_32[reg]] = num3;

                        dispWithSIB = to_string(disp) + "(%" + regs_32[base] + ")" + ",%" + regs_32[reg] + "\n";
                    }
                    else
                    {
                        int num1 = memories32bit[to_string(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp)];
                        int num2 = registers[regs_32[reg]];
                        int num3 = num1 + num2;
                        unsigned int num4 = unsigned(num1) + unsigned(num2);

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
*/

string Adder_addOverride::decode_displacement_without_SIB(int w, int d, int mod, int reg, int rm)
{
    string dispWithoutSIB = "";
    int disp_bytes[] = {2, 1, 2};
    int bytes = disp_bytes[mod];

    int disp = common.assemble_bits(bytes, false, instruction, registers);

    if (mod == 0)
    {
        if (w == 0)
        {
            if (d == 0)
            {
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
                num3 = num1 + num2;
                num4 = unsigned(num1) + unsigned(num2);

                memories8bit[to_string(disp)] = num3;

                common.setOverflow8bit(num1, num2, num3, registers);
                common.setCarry8bit(num1, num4, registers);
                common.setSign(num3, registers);
                common.setZero(num3, registers);

                dispWithoutSIB = "%" + regs_8[reg] + "," + to_string(disp) + "\n";
            }
            else if (d == 1)
            {
                int8_t num1, num2, num3;
                uint8_t num4;

                if (reg < 4)
                {
                    num1 = common.get_bits(1, 8, registers[regs_32[reg]]);
                    num2 = memories8bit[to_string(disp)];
                    num3 = num1 + num2;
                    num4 = unsigned(num1) + unsigned(num2);

                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (num3 & 0x000000ff);
                }
                else
                {
                    num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                    num2 = memories8bit[to_string(disp)];
                    num3 = num1 + num2;
                    num4 = unsigned(num1) + unsigned(num2);

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
                if (d == 0)
                {
                    int16_t num1, num2, num3;
                    uint16_t num4;

                    num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                    num2 = memories16bit[to_string(disp)];
                    num3 = num1 + num2;
                    num4 = unsigned(num1) + unsigned(num2);

                    memories16bit[to_string(disp)] = num3;

                    common.setOverflow16bit(num1, num2, num3, registers);
                    common.setCarry16bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    dispWithoutSIB = "%" + regs_16[reg] + "," + to_string(disp) + "\n";
                }
                else if (d == 1)
                {
                    int16_t num1, num2, num3;
                    uint16_t num4;

                    num1 = common.get_bits(1, 16, registers[regs_32[reg]]);
                    num2 = memories16bit[to_string(disp)];
                    num3 = num1 + num2;
                    num4 = unsigned(num1) + unsigned(num2);

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
                if (d == 0)
                {
                    int num1 = memories32bit[to_string(disp)];
                    int num2 = registers[regs_32[reg]];
                    int num3 = num1 + num2;
                    unsigned int num4 = unsigned(num1) + unsigned(num2);

                    common.setOverflow32bit(num1, num2, num3, registers);
                    common.setCarry32bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    memories32bit[to_string(disp)] = num3;

                    dispWithoutSIB = "%" + regs_32[reg] + "," + to_string(disp) + "\n";
                }
                else
                {
                    int num1 = memories32bit[to_string(disp)];
                    int num2 = registers[regs_32[reg]];
                    int num3 = num1 + num2;
                    unsigned int num4 = unsigned(num1) + unsigned(num2);

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
            if (d == 0)
            {
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
                    num2 = memories8bit[to_string(*list1[rm / 2] + *list2[rm % 2] + disp)];
                    num3 = num1 + num2;
                    num4 = unsigned(num1) + unsigned(num2);

                    memories8bit[to_string(*list1[rm / 2] + *list2[rm % 2] + disp)] = num3;
                }
                else
                {
                    num2 = memories8bit[to_string(*list3[rm % 4] + disp)];
                    num3 = num1 + num2;
                    num4 = unsigned(num1) + unsigned(num2);

                    memories8bit[to_string(*list3[rm % 4] + disp)] = num3;
                }

                common.setOverflow8bit(num1, num2, num3, registers);
                common.setCarry8bit(num1, num4, registers);
                common.setSign(num3, registers);
                common.setZero(num3, registers);

                dispWithoutSIB = "%" + regs_8[reg] + "," + to_string(disp) + "(%" + regs_16bitmode[rm] + ") \n";
            }
            else if (d == 1)
            {
                int8_t num1, num2, num3;
                uint8_t num4;

                if (rm < 4)
                {
                    num2 = memories8bit[to_string(*list1[rm / 2] + *list2[rm % 2] + disp)];
                }
                else
                {
                    num2 = memories8bit[to_string(*list3[rm % 4] + disp)];
                }

                if (reg < 4)
                {
                    num1 = common.get_bits(1, 8, registers[regs_32[reg]]);
                    num3 = num1 + num2;
                    num4 = unsigned(num1) + unsigned(num2);

                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (num3 & 0x000000ff);
                }
                else
                {
                    num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                    num3 = num1 + num2;
                    num4 = unsigned(num1) + unsigned(num2);

                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff00ff) | (num3 & 0x0000ff00);
                }

                common.setOverflow8bit(num1, num2, num3, registers);
                common.setCarry8bit(num1, num4, registers);
                common.setSign(num3, registers);
                common.setZero(num3, registers);

                dispWithoutSIB = to_string(disp) + "(%" + regs_16bitmode[rm] + "),%" + regs_8[reg] + "\n";
            }
        }
        else if (w == 1)
        {
            if (opSize)
            {
                if (d == 0)
                {
                    int16_t num1, num2, num3;
                    uint16_t num4;

                    num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                    if (rm < 4)
                    {
                        num2 = memories16bit[to_string(*list1[rm / 2] + *list2[rm % 2] + disp)];
                        num3 = num1 + num2;
                        num4 = unsigned(num1) + unsigned(num2);

                        memories16bit[to_string(*list1[rm / 2] + *list2[rm % 2] + disp)] = num3;
                    }
                    else
                    {
                        num2 = memories16bit[to_string(*list3[rm % 4] + disp)];
                        num3 = num1 + num2;
                        num4 = unsigned(num1) + unsigned(num2);

                        memories16bit[to_string(*list3[rm % 4] + disp)] = num3;
                    }

                    common.setOverflow16bit(num1, num2, num3, registers);
                    common.setCarry16bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    dispWithoutSIB = "%" + regs_16[reg] + "," + to_string(disp) + "(%" + regs_16bitmode[rm] + ") \n";
                }
                else if (d == 1)
                {
                    int16_t num1, num2, num3;
                    uint16_t num4;

                    if (rm < 4)
                    {
                        num2 = memories16bit[to_string(*list1[rm / 2] + *list2[rm % 2] + disp)];
                    }
                    else
                    {
                        num2 = memories16bit[to_string(*list3[rm % 4] + disp)];
                    }

                    num1 = common.get_bits(1, 16, registers[regs_32[reg]]);
                    num3 = num1 + num2;
                    num4 = unsigned(num1) + unsigned(num2);

                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff0000) | (num3 & 0x0000ffff);

                    common.setOverflow16bit(num1, num2, num3, registers);
                    common.setCarry16bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    dispWithoutSIB = to_string(disp) + "(%" + regs_16bitmode[rm] + "),%" + regs_16[reg] + "\n";
                }
            }
            else
            {
                if (d == 0)
                {
                    int num1,num2,num3;
                    unsigned int num4;

                    if (rm < 4)
                    {
                        num1 = memories32bit[to_string(*list1[rm / 2] + *list2[rm % 2] + disp)];
                        num2 = registers[regs_32[reg]];
                        num3 = num1 + num2;
                        num4 = unsigned(num1) + unsigned(num2);

                        memories32bit[to_string(*list1[rm / 2] + *list2[rm % 2] + disp)] = num3;
                    }
                    else
                    {
                        num1 = memories32bit[to_string(*list3[rm % 4] + disp)];
                        num2 = registers[regs_32[reg]];
                        num3 = num1 + num2;
                        num4 = unsigned(num1) + unsigned(num2);

                        memories32bit[to_string(*list3[rm % 4] + disp)] = num3;
                    }

                    

                    common.setOverflow32bit(num1, num2, num3, registers);
                    common.setCarry32bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    dispWithoutSIB = "%" + regs_32[reg] + "," + to_string(disp) + "(%" + regs_16bitmode[rm] + ") \n";
                }
                else
                {
                    int num1,num2,num3;
                    unsigned int num4;
                     if (rm < 4)
                    {
                        num1 = memories32bit[to_string(*list1[rm / 2] + *list2[rm % 2] + disp)];
                    }
                    else
                    {
                        num1 = memories32bit[to_string(*list3[rm % 4] + disp)];
                    }
                    num2 = registers[regs_32[reg]];
                    num3 = num1 + num2;
                    num4 = unsigned(num1) + unsigned(num2);

                    common.setOverflow32bit(num1, num2, num3, registers);
                    common.setCarry32bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    registers[regs_32[reg]] = num3;

                    dispWithoutSIB = to_string(disp) + "(%" + regs_16bitmode[rm] + "),%" + regs_32[reg] + "\n";
                }
            }
        }
    }

    return dispWithoutSIB;
};

/*
string Adder_addOverride::decode_SIB(int w, int d, int mod, int reg)
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
*/

string Adder_addOverride::decode_mod_00(int w, int d, int reg, int rm)
{
    string string00 = "";
    if (rm == 6)
    {
        string00 = decode_displacement_without_SIB(w, d, 0, reg, 5);
    }
    else
    {
        if (w == 0)
        {
            if (d == 0)
            {
                string00 = "%" + regs_8[reg] + ",(%" + regs_16bitmode[rm] + ")\n";

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
                    num2 = memories8bit[to_string(*list1[rm / 2] + *list2[rm % 2])];
                    num3 = num1 + num2;
                    num4 = unsigned(num1) + unsigned(num2);

                    memories8bit[to_string(*list1[rm / 2] + *list2[rm % 2])] = num3;
                }
                else
                {
                    num2 = memories8bit[to_string(*list3[rm % 4])];
                    num3 = num1 + num2;
                    num4 = unsigned(num1) + unsigned(num2);

                    memories8bit[to_string(*list3[rm % 4])] = num3;
                }

                common.setOverflow8bit(num1, num2, num3, registers);
                common.setCarry8bit(num1, num4, registers);
                common.setSign(num3, registers);
                common.setZero(num3, registers);
            }
            else if (d == 1)
            {
                string00 = "(%" + regs_16bitmode[rm] + "),%" + regs_8[reg] + "\n";

                int8_t num1, num2, num3;
                uint8_t num4;

                if (rm < 4)
                {
                    num2 = memories8bit[to_string(*list1[rm / 2] + *list2[rm % 2])];
                }
                else
                {
                    num2 = memories8bit[to_string(*list3[rm % 4])];
                }

                if (reg < 4)
                {
                    num1 = common.get_bits(1, 8, registers[regs_32[reg]]);
                    num3 = num1 + num2;
                    num4 = unsigned(num1) + unsigned(num2);

                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (num3 & 0x000000ff);
                }
                else
                {
                    num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                    num3 = num1 + num2;
                    num4 = unsigned(num1) + unsigned(num2);

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
                if (d == 0)
                {
                    string00 = "%" + regs_16[reg] + ",(%" + regs_16bitmode[rm] + ")\n";

                    int16_t num1, num2, num3;
                    uint16_t num4;

                    num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                    if (rm < 4)
                    {
                        num2 = memories16bit[to_string(*list1[rm / 2] + *list2[rm % 2])];
                        num3 = num1 + num2;
                        num4 = unsigned(num1) + unsigned(num2);

                        memories16bit[to_string(*list1[rm / 2] + *list2[rm % 2])] = num3;
                    }
                    else
                    {
                        num2 = memories16bit[to_string(*list3[rm % 4])];
                        num3 = num1 + num2;
                        num4 = unsigned(num1) + unsigned(num2);

                        memories16bit[to_string(*list3[rm % 4])] = num3;
                    }

                    common.setOverflow16bit(num1, num2, num3, registers);
                    common.setCarry16bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);
                }
                else if (d == 1)
                {
                    string00 = "(%" + regs_16bitmode[rm] + "),%" + regs_16[reg] + "\n";

                    int16_t num1, num2, num3;
                    uint16_t num4;

                    num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                    if (rm < 4)
                    {
                        num2 = memories16bit[to_string(*list1[rm / 2] + *list2[rm % 2])];
                    }
                    else
                    {
                        num2 = memories16bit[to_string(*list3[rm % 4])];
                    }

                    num3 = num1 + num2;
                    num4 = unsigned(num1) + unsigned(num2);

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
                    int num1, num2, num3;
                    unsigned int num4;
                    if (rm < 4)
                    {
                        num1 = memories32bit[to_string(*list1[rm / 2] + *list2[rm % 2])];
                        num2 = registers[regs_32[reg]]; //registers["ECX"]
                        num3 = num1 + num2;
                        num4 = unsigned(num1) + unsigned(num2);

                        memories32bit[to_string(*list1[rm / 2] + *list2[rm % 2])] = num3;
                    }
                    else
                    {
                        num1 = memories32bit[to_string(*list3[rm % 4])];
                        num2 = registers[regs_32[reg]]; //registers["ECX"]
                        num3 = num1 + num2;
                        num4 = unsigned(num1) + unsigned(num2);

                        memories32bit[to_string(*list3[rm % 4])] = num3;
                    }

                    common.setOverflow32bit(num1, num2, num3, registers);
                    common.setCarry32bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    string00 = "%" + regs_32[reg] + ",(%" + regs_16bitmode[rm] + ")\n";
                }
                else
                {
                    int num1, num2, num3;
                    unsigned int num4;

                    if (rm < 4)
                    {
                        num1 = memories32bit[to_string(*list1[rm / 2] + *list2[rm % 2])];
                    }
                    else
                    {
                        num1 = memories32bit[to_string(*list3[rm % 4])];
                    }

                    num2 = registers[regs_32[reg]];
                    num3 = num1 + num2;
                    num4 = unsigned(num1) + unsigned(num2);

                    common.setOverflow32bit(num1, num2, num3, registers);
                    common.setCarry32bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    string00 = "(%" + regs_16bitmode[rm] + "),%" + regs_32[reg] + "\n";
                    registers[regs_32[reg]] = num3;
                }
            }
        }
    }

    return string00;
}

string Adder_addOverride::decode_mod_01(int w, int d, int reg, int rm)
{
    string string01 = decode_displacement_without_SIB(w, d, 1, reg, rm);
    return string01;
}

string Adder_addOverride::decode_mod_10(int w, int d, int reg, int rm)
{
    string string10 = decode_displacement_without_SIB(w, d, 2, reg, rm);
    return string10;
}

string Adder_addOverride::decode_mod_11(int w, int d, int reg, int rm)
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
                num2 = common.get_bits(1, 8, registers[regs_32[rm]]);
                num3 = num1 + num2;
                num4 = unsigned(num1) + unsigned(num2);
                registers[regs_32[rm]] = ((registers[regs_32[rm]]) & 0xffffff00) | (num3 & 0x000000ff);
            }
            else
            {
                num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                num2 = common.get_bits(9, 8, registers[regs_32[rm % 4]]);
                num3 = num1 + num2;
                num4 = unsigned(num1) + unsigned(num2);
                registers[regs_32[rm]] = ((registers[regs_32[rm]]) & 0xffff00ff) | ((num3 << 8) & 0x0000ff00);
            }

            //cout << "num1: " << dec << signed(num1) << ", num2: " << dec << signed(num2) << ", num3: " << dec << signed(num3) << "\n";

            common.setOverflow8bit(num1, num2, num3, registers);
            common.setCarry8bit(num1, num4, registers);
            common.setSign(num3, registers);
            common.setZero(num3, registers);

            string11 = "%" + regs_8[reg] + ",%" + regs_8[rm] + "\n";
        }
        else if (d == 1)
        {
            //printf("w:0 and d:1 \n");

            int8_t num1, num2, num3;
            uint8_t num4;
            if (reg < 4)
            {
                num1 = common.get_bits(1, 8, registers[regs_32[reg]]);
                num2 = common.get_bits(1, 8, registers[regs_32[rm]]);
                num3 = num1 + num2;
                num4 = unsigned(num1) + unsigned(num2);
                registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (num3 & 0x000000ff);
            }
            else
            {
                num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                num2 = common.get_bits(9, 8, registers[regs_32[rm % 4]]);
                num3 = num1 + num2;
                num4 = unsigned(num1) + unsigned(num2);
                registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff00ff) | ((num3 << 8) & 0x0000ff00);
            }
            //cout << "num1: " << dec << signed(num1) << ", num2: " << dec << signed(num2) << ", num3: " << dec << signed(num3) << "\n";

            common.setOverflow8bit(num1, num2, num3, registers);
            common.setCarry8bit(num1, num4, registers);
            common.setSign(num3, registers);
            common.setZero(num3, registers);

            string11 = "%" + regs_8[rm] + ",%" + regs_8[reg] + "\n";
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
                num4 = unsigned(num1) + unsigned(num2);
                registers[regs_32[rm]] = ((registers[regs_32[rm]]) & 0xffff0000) | (num3 & 0x0000ffff);

                //cout << "num1: " << dec << signed(num1) << ", num2: " << dec << signed(num2) << ", num3: " << dec << signed(num3) << "\n";

                common.setOverflow16bit(num1, num2, num3, registers);
                common.setCarry16bit(num1, num4, registers);
                common.setSign(num3, registers);
                common.setZero(num3, registers);

                string11 = "%" + regs_16[reg] + ",%" + regs_16[rm] + "\n";
            }
            else if (d == 1)
            {
                //printf("w:0 and d:1 \n");

                int16_t num1, num2, num3;
                uint16_t num4;

                num1 = common.get_bits(1, 16, registers[regs_32[reg]]);
                num2 = common.get_bits(1, 16, registers[regs_32[rm]]);
                num3 = num1 + num2;
                num4 = unsigned(num1) + unsigned(num2);
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
            if (d == 0)
            {
                //printf("w:1 and d:0 \n");
                int num1 = registers[regs_32[reg]];
                int num2 = registers[regs_32[rm]];
                int num3 = num1 + num2;
                unsigned int num4 = unsigned(num1) + unsigned(num2);
                registers[regs_32[rm]] = num3;

                //cout << "num1: " << num1 << ", num2: " << num2 << ", num3: " << num3 << ", num4: " << num4 << "\n";

                common.setOverflow32bit(num1, num2, num3, registers);
                common.setCarry32bit(num1, num4, registers);
                common.setSign(num3, registers);
                common.setZero(num3, registers);

                string11 = "%" + regs_32[reg] + ",%" + regs_32[rm] + "\n";
            }
            else
            {
                //printf("w:1 and d:1 \n");
                int num1 = registers[regs_32[reg]];
                int num2 = registers[regs_32[rm]];
                int num3 = num1 + num2;
                unsigned int num4 = unsigned(num1) + unsigned(num2);
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

string Adder_addOverride::decode_add(short prefixes[4])
{
    bool d = common.get_bits(2, 1, instruction.front());
    bool w = common.get_bits(1, 1, instruction.front());

    instruction.pop();
    registers["EIP"] = registers["EIP"] + 1;

    int mod = instruction.front() >> 6;
    int reg = common.get_bits(4, 3, instruction.front());
    int rm = common.get_bits(1, 3, instruction.front());

    instruction.pop();
    registers["EIP"] = registers["EIP"] + 1;

    //printf("d:%d \n", d);
    //printf("w:%d \n", w);
    //printf("mod:%d \n", mod);
    //printf("reg:%d \n", reg);
    //printf("rm:%d \n", rm);

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

    return "Adder instantiated and done";
}