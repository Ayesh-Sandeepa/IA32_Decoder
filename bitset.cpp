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

#include "bitset.h"

using namespace std;

Bitset::Bitset(Common com, map<string, int> cs, map<string, int> &registers, map<string, int> &memories32bit, map<string, int16_t> &memories16bit, map<string, int8_t> &memories8bit, list<string> &memoryAccesses)
    : common(com), cs(cs), registers(registers), memories32bit(memories32bit), memories16bit(memories16bit), memories8bit(memories8bit), memoryAccesses(memoryAccesses)
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

string Bitset::decode_displacement_with_SIB(int mod, int reg, int index, int scale, int base)
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
        disp = common.assemble_bits(bytes, cs, registers);
        st = common.getHex(disp, 0, 0);
    }

    if (imm)
    {
        int8_t imd = common.assemble_bits(1, cs, registers);
        if (mod == 0)
        {
            if (base == 5)
            {

                if (opSize)
                {

                    if (index == 4)
                    {
                        memoryAccesses.push_back("bt " + st + "," + common.getHex(imd, 0, 0));

                        bool bit = common.get_bits(imd % 16 + 1, 1, memories16bit[st]);
                        memoryAccesses.push_back("Read " + to_string(imd % 16) + " bit from " + st);

                        if (bit)
                        {
                            registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                        }
                        else
                        {
                            registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                        }

                        dispWithSIB = "bt " + st + "," + common.getHex(imd, 0, 0) + "\n";
                    }
                    else
                    {
                        memoryAccesses.push_back("bt " + st + "(,%" + regs_32[index] + "," + to_string(scale) + ")" + "," + common.getHex(imd, 0, 0));

                        memoryAccesses.push_back("Read " + to_string(imd % 16) + " bit from " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                        bool bit = common.get_bits(imd % 16 + 1, 1, memories16bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)]);
                        if (bit)
                        {
                            registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                        }
                        else
                        {
                            registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                        }

                        dispWithSIB = "bt " + st + "(,%" + regs_32[index] + "," + to_string(scale) + ")" + "," + common.getHex(imd, 0, 0) + "\n";
                    }
                }
                else
                {

                    if (index == 4)
                    {
                        memoryAccesses.push_back("bt " + st + "," + common.getHex(imd, 0, 0));

                        bool bit = common.get_bits(imd % 32 + 1, 1, memories32bit[st]);
                        memoryAccesses.push_back("Read " + to_string(imd % 32) + "bit from " + st);

                        if (bit)
                        {
                            registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                        }
                        else
                        {
                            registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                        }

                        dispWithSIB = "bt " + st + "," + common.getHex(imd, 0, 0) + "\n";
                    }
                    else
                    {
                        memoryAccesses.push_back("bt " + st + "(,%" + regs_32[index] + "," + to_string(scale) + ")" + "," + common.getHex(imd, 0, 0));

                        bool bit = common.get_bits(imd % 32 + 1, 1, memories32bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)]);
                        memoryAccesses.push_back("Read " + to_string(imd % 32) + "bit from " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                        if (bit)
                        {
                            registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                        }
                        else
                        {
                            registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                        }

                        dispWithSIB = "bt " + st + "(,%" + regs_32[index] + "," + to_string(scale) + ")" + "," + common.getHex(imd, 0, 0) + "\n";
                    }
                }
            }
            else
            {
                if (opSize)
                {
                    if (index == 4)
                    {
                        memoryAccesses.push_back("bt (%" + regs_32[base] + ")" + ", $" + common.getHex(imd, 0, 0));

                        bool bit = common.get_bits(imd % 16 + 1, 1, memories16bit[common.getHex(registers[regs_32[base]], 0, 0)]);

                        memoryAccesses.push_back("Read " + to_string(imd % 16) + "bits from " + common.getHex(registers[regs_32[base]], 0, 0));

                        if (bit)
                        {
                            registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                        }
                        else
                        {
                            registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                        }

                        dispWithSIB = "bt (%" + regs_32[base] + ")" + ", $" + common.getHex(imd, 0, 0) + "\n";
                    }
                    else
                    {
                        memoryAccesses.push_back("bt (%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "), $" + common.getHex(imd, 0, 0));

                        bool bit = common.get_bits(imd % 16 + 1, 1, memories16bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0)]);
                        memoryAccesses.push_back("Read " + to_string(imd % 16) + "bit from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0));

                        if (bit)
                        {
                            registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                        }
                        else
                        {
                            registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                        }

                        dispWithSIB = "bt (%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "), $" + common.getHex(imd, 0, 0) + "\n";
                    }
                }
                else
                {

                    if (index == 4)
                    {
                        memoryAccesses.push_back("bt (%" + regs_32[base] + "), $" + common.getHex(imd, 0, 0));

                        bool bit = common.get_bits(imd % 32 + 1, 1, memories32bit[common.getHex(registers[regs_32[base]], 0, 0)]);
                        memoryAccesses.push_back("Read " + to_string(imd % 32) + "bit from " + common.getHex(registers[regs_32[base]], 0, 0));

                        if (bit)
                        {
                            registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                        }
                        else
                        {
                            registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                        }

                        dispWithSIB = "bt (%" + regs_32[base] + "), $" + common.getHex(imd, 0, 0) + "\n";
                    }
                    else
                    {
                        memoryAccesses.push_back("bt (%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "), $" + common.getHex(imd, 0, 0));

                        bool bit = common.get_bits(imd % 32 + 1, 1, memories32bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)]);
                        memoryAccesses.push_back("Read " + to_string(imd % 32) + "bit from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                        if (bit)
                        {
                            registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                        }
                        else
                        {
                            registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                        }

                        dispWithSIB = "bt (%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "), $" + common.getHex(imd, 0, 0) + "\n";
                    }
                }
            }
            //printf("mod equals 0");
        }
        else
        {
            if (opSize)
            {
                if (index == 4)
                {
                    memoryAccesses.push_back("bt " + st + "(%" + regs_32[base] + "), $" + common.getHex(imd, 0, 0));

                    bool bit = common.get_bits(imd % 16 + 1, 1, memories16bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)]);
                    memoryAccesses.push_back("Read " + to_string(imd % 16) + "bit from " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                    if (bit)
                    {
                        registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                    }
                    else
                    {
                        registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                    }

                    dispWithSIB = "bt " + st + "(%" + regs_32[base] + "), $" + common.getHex(imd, 0, 0) + "\n";
                }
                else
                {
                    memoryAccesses.push_back("bt " + st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "), $" + common.getHex(imd, 0, 0));

                    bool bit = common.get_bits(imd % 16 + 1, 1, memories16bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0)]);
                    memoryAccesses.push_back("Read " + to_string(imd % 16) + "bit from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0));

                    if (bit)
                    {
                        registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                    }
                    else
                    {
                        registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                    }

                    dispWithSIB = "bt " + st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "), $" + common.getHex(imd, 0, 0) + "\n";
                }
            }
            else
            {
                if (index == 4)
                {
                    memoryAccesses.push_back("bt " + st + "(%" + regs_32[base] + "), $" + common.getHex(imd, 0, 0));

                    bool bit = common.get_bits(imd % 32 + 1, 1, memories32bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)]);
                    memoryAccesses.push_back("Read " + to_string(imd % 32) + "bit from " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                    if (bit)
                    {
                        registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                    }
                    else
                    {
                        registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                    }

                    dispWithSIB = "bt " + st + "(%" + regs_32[base] + "), $" + common.getHex(imd, 0, 0) + "\n";
                }
                else
                {
                    memoryAccesses.push_back("bt " + st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "), $" + common.getHex(imd, 0, 0));

                    bool bit = common.get_bits(imd % 32 + 1, 1, memories32bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)]);
                    memoryAccesses.push_back("Read " + to_string(imd % 32) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                    if (bit)
                    {
                        registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                    }
                    else
                    {
                        registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                    }

                    dispWithSIB = "bt " + st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "), $" + common.getHex(imd, 0, 0) + "\n";
                }
            }
        }
    }
    else
    {
        if (mod == 0)
        {
            if (base == 5)
            {

                if (opSize)
                {

                    if (index == 4)
                    {
                        memoryAccesses.push_back("bt " + st + ",%" + regs_16[reg]);

                        int16_t num1;

                        num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                        bool bit = common.get_bits(num1 % 16 + 1, 1, memories16bit[st]);
                        memoryAccesses.push_back("Read " + to_string(num1 % 16) + " bit from " + st);

                        if (bit)
                        {
                            registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                        }
                        else
                        {
                            registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                        }

                        dispWithSIB = "bt " + st + ",%" + regs_16[reg] + "\n";
                    }
                    else
                    {
                        memoryAccesses.push_back("bt " + st + "(,%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_16[reg]);

                        int16_t num1;

                        num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                        memoryAccesses.push_back("Read " + to_string(num1 % 16) + " bit from " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                        bool bit = common.get_bits(num1 % 16 + 1, 1, memories16bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)]);
                        if (bit)
                        {
                            registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                        }
                        else
                        {
                            registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                        }

                        dispWithSIB = "bt " + st + "(,%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_16[reg] + "\n";
                    }
                }
                else
                {

                    if (index == 4)
                    {
                        memoryAccesses.push_back("bt " + st + ",%" + regs_32[reg]);

                        int num2 = registers[regs_32[reg]];
                        bool bit = common.get_bits(num2 % 32 + 1, 1, memories32bit[st]);
                        memoryAccesses.push_back("Read " + to_string(num2 % 32) + "bit from " + st);

                        if (bit)
                        {
                            registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                        }
                        else
                        {
                            registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                        }

                        dispWithSIB = "bt " + st + ",%" + regs_32[reg] + "\n";
                    }
                    else
                    {
                        memoryAccesses.push_back("bt " + st + "(,%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_32[reg]);

                        int num2 = registers[regs_32[reg]];
                        bool bit = common.get_bits(num2 % 32 + 1, 1, memories32bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)]);
                        memoryAccesses.push_back("Read " + to_string(num2 % 32) + "bit from " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                        if (bit)
                        {
                            registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                        }
                        else
                        {
                            registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                        }

                        dispWithSIB = "bt " + st + "(,%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_32[reg] + "\n";
                    }
                }
            }
            else
            {
                if (opSize)
                {
                    if (index == 4)
                    {
                        memoryAccesses.push_back("bt (%" + regs_32[base] + "),%" + regs_16[reg]);

                        int16_t num1;

                        num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                        bool bit = common.get_bits(num1 % 16 + 1, 1, memories16bit[common.getHex(registers[regs_32[base]], 0, 0)]);

                        memoryAccesses.push_back("Read " + to_string(num1 % 16) + "bits from " + common.getHex(registers[regs_32[base]], 0, 0));

                        if (bit)
                        {
                            registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                        }
                        else
                        {
                            registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                        }

                        dispWithSIB = "bt (%" + regs_32[base] + "),%" + regs_16[reg] + "\n";
                    }
                    else
                    {
                        memoryAccesses.push_back("bt (%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_16[reg]);

                        int16_t num1;

                        num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                        bool bit = common.get_bits(num1 % 16 + 1, 1, memories16bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0)]);
                        memoryAccesses.push_back("Read " + to_string(num1 % 16) + "bit from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0));

                        if (bit)
                        {
                            registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                        }
                        else
                        {
                            registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                        }

                        dispWithSIB = "bt (%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_16[reg] + "\n";
                    }
                }
                else
                {

                    if (index == 4)
                    {
                        memoryAccesses.push_back("bt (%" + regs_32[base] + "),%" + regs_32[reg]);

                        int num2 = registers[regs_32[reg]];
                        bool bit = common.get_bits(num2 % 32 + 1, 1, memories32bit[common.getHex(registers[regs_32[base]], 0, 0)]);
                        memoryAccesses.push_back("Read " + to_string(num2 % 32) + "bit from " + common.getHex(registers[regs_32[base]], 0, 0));

                        if (bit)
                        {
                            registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                        }
                        else
                        {
                            registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                        }

                        dispWithSIB = "bt (%" + regs_32[base] + "),%" + regs_32[reg] + "\n";
                    }
                    else
                    {
                        memoryAccesses.push_back("bt (%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_32[reg]);

                        int num2 = registers[regs_32[reg]];
                        bool bit = common.get_bits(num2 % 32 + 1, 1, memories32bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)]);
                        memoryAccesses.push_back("Read " + to_string(num2 % 32) + "bit from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                        if (bit)
                        {
                            registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                        }
                        else
                        {
                            registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                        }

                        dispWithSIB = "bt (%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_32[reg] + "\n";
                    }
                }
            }
            //printf("mod equals 0");
        }
        else
        {
            if (opSize)
            {
                if (index == 4)
                {
                    memoryAccesses.push_back("bt " + st + "(%" + regs_32[base] + "),%" + regs_16[reg]);

                    int16_t num1;

                    num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                    bool bit = common.get_bits(num1 % 16 + 1, 1, memories16bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)]);
                    memoryAccesses.push_back("Read " + to_string(num1 % 16) + "bit from " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                    if (bit)
                    {
                        registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                    }
                    else
                    {
                        registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                    }

                    dispWithSIB = "bt " + st + "(%" + regs_32[base] + "),%" + regs_16[reg] + "\n";
                }
                else
                {
                    memoryAccesses.push_back("bt " + st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_16[reg]);

                    int16_t num1;

                    num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                    bool bit = common.get_bits(num1 % 16 + 1, 1, memories16bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0)]);
                    memoryAccesses.push_back("Read " + to_string(num1 % 16) + "bit from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0));

                    if (bit)
                    {
                        registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                    }
                    else
                    {
                        registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                    }

                    dispWithSIB = "bt " + st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_16[reg] + "\n";
                }
            }
            else
            {
                if (index == 4)
                {
                    memoryAccesses.push_back("bt" + st + "(%" + regs_32[base] + "),%" + regs_32[reg]);

                    int num2 = registers[regs_32[reg]];
                    bool bit = common.get_bits(num2 % 32 + 1, 1, memories32bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)]);
                    memoryAccesses.push_back("Read " + to_string(num2 % 32) + "bit from " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                    if (bit)
                    {
                        registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                    }
                    else
                    {
                        registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                    }

                    dispWithSIB = "bt" + st + "(%" + regs_32[base] + "),%" + regs_32[reg] + "\n";
                }
                else
                {
                    memoryAccesses.push_back("bt " + st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_32[reg]);

                    int num2 = registers[regs_32[reg]];
                    bool bit = common.get_bits(num2 % 32 + 1, 1, memories32bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)]);
                    memoryAccesses.push_back("Read " + to_string(num2 % 32) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                    if (bit)
                    {
                        registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                    }
                    else
                    {
                        registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                    }

                    dispWithSIB = "bt " + st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_32[reg] + "\n";
                }
            }
        }
    }

    return dispWithSIB;
}

string Bitset::decode_displacement_without_SIB(int mod, int reg, int rm)
{
    string dispWithoutSIB = "";
    int disp_bytes[] = {4, 1, 4};
    int bytes = disp_bytes[mod];

    unsigned int disp = common.assemble_bits(bytes, cs, registers);
    string st = common.getHex(disp, 0, 0);

    if (imm)
    {
        int8_t imd = common.assemble_bits(1, cs, registers);
        if (mod == 0)
        {
            if (opSize)
            {
                memoryAccesses.push_back("bt " + st + ",$" + common.getHex(imd, 0, 0));

                bool bit = common.get_bits(imd % 16 + 1, 1, memories16bit[st]);
                memoryAccesses.push_back("Read " + to_string(imd % 16) + "bit from " + st);

                if (bit)
                {
                    registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                }
                else
                {
                    registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                }

                dispWithoutSIB = "bt " + st + ",$" + common.getHex(imd, 0, 0) + "\n";
            }
            else
            {
                memoryAccesses.push_back("bt " + st + ",$" + common.getHex(imd, 0, 0));

                bool bit = common.get_bits(imd % 32 + 1, 1, memories32bit[st]);
                memoryAccesses.push_back("Read " + to_string(imd % 32) + "bit from " + st);

                if (bit)
                {
                    registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                }
                else
                {
                    registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                }

                dispWithoutSIB = "bt " + st + ",$" + common.getHex(imd, 0, 0) + "\n";
            }
        }
        else
        {
            if (opSize)
            {
                memoryAccesses.push_back("bt " + st + "(%" + regs_32[rm] + "),$" + common.getHex(imd, 0, 0));

                bool bit = common.get_bits(imd % 16 + 1, 1, memories16bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)]);
                memoryAccesses.push_back("Read " + to_string(imd % 16) + " from " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

                if (bit)
                {
                    registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                }
                else
                {
                    registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                }

                dispWithoutSIB = "bt " + st + "(%" + regs_32[rm] + "),$" + common.getHex(imd, 0, 0) + "\n";
            }
            else
            {
                bool bit = common.get_bits(imd % 32 + 1, 1, memories32bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)]);
                memoryAccesses.push_back("Read " + to_string(imd % 32) + " from " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

                if (bit)
                {
                    registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                }
                else
                {
                    registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                }

                dispWithoutSIB = "bt " + st + "(%" + regs_32[rm] + "),$" + common.getHex(imd, 0, 0) + " \n";
            }
        }
    }
    else
    {
        if (mod == 0)
        {
            if (opSize)
            {
                memoryAccesses.push_back("bt " + st + ",%" + regs_16[reg]);

                int16_t num1;

                num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                bool bit = common.get_bits(num1 % 16 + 1, 1, memories16bit[st]);
                memoryAccesses.push_back("Read " + to_string(num1 % 16) + "bit from " + st);

                if (bit)
                {
                    registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                }
                else
                {
                    registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                }

                dispWithoutSIB = "bt " + st + ",%" + regs_16[reg] + "\n";
            }
            else
            {
                memoryAccesses.push_back("bt " + st + ",%" + regs_32[reg]);

                int num2 = registers[regs_32[reg]];
                bool bit = common.get_bits(num2 % 32 + 1, 1, memories32bit[st]);
                memoryAccesses.push_back("Read " + to_string(num2 % 32) + "bit from " + st);

                if (bit)
                {
                    registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                }
                else
                {
                    registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                }

                dispWithoutSIB = "bt " + st + ",%" + regs_32[reg] + "\n";
            }
        }
        else
        {
            if (opSize)
            {
                memoryAccesses.push_back("bt " + st + "(%" + regs_32[rm] + "),%" + regs_16[reg]);

                int16_t num1;

                num1 = common.get_bits(1, 16, registers[regs_32[reg]]);
                bool bit = common.get_bits(num1 % 16 + 1, 1, memories16bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)]);
                memoryAccesses.push_back("Read " + to_string(num1 % 16) + " from " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

                if (bit)
                {
                    registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                }
                else
                {
                    registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                }

                dispWithoutSIB = "bt " + st + "(%" + regs_32[rm] + "),%" + regs_16[reg] + "\n";
            }
            else
            {
                int num2 = registers[regs_32[reg]];
                bool bit = common.get_bits(num2 % 32 + 1, 1, memories32bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)]);
                memoryAccesses.push_back("Read " + to_string(num2 % 32) + " from " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

                if (bit)
                {
                    registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                }
                else
                {
                    registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                }

                dispWithoutSIB = "bt " + st + "(%" + regs_32[rm] + "),%" + regs_32[reg] + " \n";
            }
        }
    }

    return dispWithoutSIB;
};

string Bitset::decode_SIB(int mod, int reg)
{
    string stringSib = "";

    int scale = common.get_bits(7, 2, cs[common.getHex(registers["EIP"],0,0)]);
    int index = common.get_bits(4, 3, cs[common.getHex(registers["EIP"],0,0)]);
    int base = common.get_bits(1, 3, cs[common.getHex(registers["EIP"],0,0)]);

    //instruction.pop();
    registers["EIP"] = registers["EIP"] + 1;

    //printf("scale:%d \n", scale);
    //printf("index:%d \n", index);
    //printf("base:%d \n", base);

    scale = pow(2, scale);

    stringSib = decode_displacement_with_SIB(mod, reg, index, scale, base);
    return stringSib;
}

string Bitset::decode_mod_00(int reg, int rm)
{
    string string00 = "";
    if (rm == 4)
    {
        string00 = decode_SIB(0, reg);
    }
    else if (rm == 5)
    {
        string00 = decode_displacement_without_SIB(0, reg, 5);
    }
    else
    {
        if (imm)
        {
            int8_t imd = common.assemble_bits(1, cs, registers);
            if (opSize)
            {
                memoryAccesses.push_back("bt (%" + regs_32[rm] + "),$" + common.getHex(imd, 0, 0));

                string00 = "bt (%" + regs_32[rm] + "),$" + common.getHex(imd, 0, 0) + "\n";

                bool bit = common.get_bits(imd % 16 + 1, 1, memories16bit[common.getHex(registers[regs_32[rm]], 0, 0)]);
                memoryAccesses.push_back("Read " + to_string(imd % 16) + " from " + common.getHex(registers[regs_32[rm]], 0, 0));

                if (bit)
                {
                    registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                }
                else
                {
                    registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                }
            }
            else
            {
                memoryAccesses.push_back("bt (%" + regs_32[rm] + "),$" + common.getHex(imd, 0, 0));

                bool bit = common.get_bits(imd % 32 + 1, 1, memories32bit[common.getHex(registers[regs_32[rm]], 0, 0)]);
                memoryAccesses.push_back("Read " + to_string(imd % 32) + " from " + common.getHex(registers[regs_32[rm]], 0, 0));

                if (bit)
                {
                    registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                }
                else
                {
                    registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                }

                string00 = "bt (%" + regs_32[rm] + "),$" + common.getHex(imd, 0, 0) + "\n";
            }
        }
        else
        {
            if (opSize)
            {
                memoryAccesses.push_back("bt (%" + regs_32[rm] + "),%" + regs_16[reg]);

                string00 = "bt (%" + regs_32[rm] + "),%" + regs_16[reg] + "\n";

                int16_t num1;

                num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                bool bit = common.get_bits(num1 % 16 + 1, 1, memories16bit[common.getHex(registers[regs_32[rm]], 0, 0)]);
                memoryAccesses.push_back("Read " + to_string(num1 % 16) + " from " + common.getHex(registers[regs_32[rm]], 0, 0));

                if (bit)
                {
                    registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                }
                else
                {
                    registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                }
            }
            else
            {
                memoryAccesses.push_back("bt (%" + regs_32[rm] + "),%" + regs_32[reg]);

                int num2 = registers[regs_32[reg]];
                bool bit = common.get_bits(num2 % 32 + 1, 1, memories32bit[common.getHex(registers[regs_32[rm]], 0, 0)]);
                memoryAccesses.push_back("Read " + to_string(num2 % 32) + " from " + common.getHex(registers[regs_32[rm]], 0, 0));

                if (bit)
                {
                    registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                }
                else
                {
                    registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                }

                string00 = "bt (%" + regs_32[rm] + "),%" + regs_32[reg] + "\n";
            }
        }
    }

    return string00;
}

string Bitset::decode_mod_01(int reg, int rm)
{
    string string01 = "";
    if (rm == 4)
    {
        string01 = decode_SIB(1, reg);
    }
    else
    {
        string01 = decode_displacement_without_SIB(1, reg, rm);
    }
    return string01;
}

string Bitset::decode_mod_10(int reg, int rm)
{
    string string10 = "";
    if (rm == 4)
    {
        string10 = decode_SIB(2, reg);
    }
    else
    {
        string10 = decode_displacement_without_SIB(2, reg, rm);
    }
    return string10;
}

string Bitset::decode_mod_11(int reg, int rm)
{
    string string11 = "";

    if (imm)
    {
        int8_t imd = common.assemble_bits(1, cs, registers);
        if (opSize)
        {

            bool bit = common.get_bits(imd % 16 + 1, 1, registers[regs_32[rm]]);

            if (bit)
            {
                registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
            }
            else
            {
                registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
            }

            string11 = "bt %" + regs_16[rm] + ",$" + common.getHex(imd, 0, 0) + "\n";
        }
        else
        {
            bool bit = common.get_bits(imd % 32 + 1, 1, registers[regs_32[rm]]);

            if (bit)
            {
                registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
            }
            else
            {
                registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
            }

            string11 = "bt %" + regs_32[rm] + ",$" + common.getHex(imd, 0, 0) + "\n";
        }
    }
    else
    {
        if (opSize)
        {
            int16_t num1;

            num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

            bool bit = common.get_bits(num1 % 16 + 1, 1, registers[regs_32[rm]]);

            if (bit)
            {
                registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
            }
            else
            {
                registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
            }

            string11 = "bt %" + regs_16[rm] + ",%" + regs_16[reg] + "\n";
        }
        else
        {
            int num1 = registers[regs_32[reg]];

            bool bit = common.get_bits(num1 % 32 + 1, 1, registers[regs_32[rm]]);

            if (bit)
            {
                registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
            }
            else
            {
                registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
            }

            string11 = "bt %" + regs_32[rm] + ",%" + regs_32[reg] + "\n";
        }
    }

    return string11;
}

string Bitset::decode_bt(short prefixes[4])
{
    string decoded_bytes;

    if (prefixes[3] == 0x67)
    {
        Bitset_addOverride bitset_addOverride(common, cs, registers, memories32bit, memories16bit, memories8bit, memoryAccesses);
        decoded_bytes = bitset_addOverride.decode_bt(prefixes);
    }
    else
    {
        short opCode = cs[common.getHex(registers["EIP"],0,0)];

        //instruction.pop();
        registers["EIP"] = registers["EIP"] + 1;

        if (prefixes[2] == 0x66)
        {
            opSize = true;
        }
        else
        {
            opSize = false;
        }

        if (opCode == 0xba)
        {
            imm = true;
        }
        else if (opCode == 0xa3)
        {
            imm = false;
        }
        short modrm = cs[common.getHex(registers["EIP"],0,0)];
        int mod = modrm >> 6;
        int reg = common.get_bits(4, 3, modrm);
        int rm = common.get_bits(1, 3, modrm);

        //instruction.pop();
        registers["EIP"] = registers["EIP"] + 1;

        if (mod == 0)
        {
            decoded_bytes = decode_mod_00(reg, rm);
        }
        else if (mod == 1)
        {
            decoded_bytes = decode_mod_01(reg, rm);
        }
        else if (mod == 2)
        {
            decoded_bytes = decode_mod_10(reg, rm);
        }
        else
        {
            decoded_bytes = decode_mod_11(reg, rm);
        }
    }

    cout << decoded_bytes;

    cout << "EAX: " << hex << registers["EAX"] << "\n";
    cout << "ECX: " << hex << registers["ECX"] << "\n";
    cout << "EDX: " << hex << registers["EDX"] << "\n";
    cout << "EBX: " << hex << registers["EBX"] << "\n";
    cout << "ESP: " << hex << registers["ESP"] << "\n";
    cout << "EBP: " << hex << registers["EBP"] << "\n";
    cout << "ESI: " << hex << registers["ESI"] << "\n";
    cout << "EDI: " << hex << registers["EDI"] << "\n";
    cout << "EFLAGS: " << hex << registers["EFLAGS"] << "\n \n";

    return "Adder instantiated and done";
}
