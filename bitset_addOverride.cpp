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

Bitset_addOverride::Bitset_addOverride(Common com, map<string, int> &cs, map<string, int> &registers, map<string, int> &memories32bit, map<string, int16_t> &memories16bit, map<string, int8_t> &memories8bit, list<string> &memoryAccesses)
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

    regs_16bitmode[0] = "(BX+SI)";
    regs_16bitmode[1] = "(BX+DI)";
    regs_16bitmode[2] = "(BP+SI)";
    regs_16bitmode[3] = "(BP+DI)";
    regs_16bitmode[4] = "(SI)";
    regs_16bitmode[5] = "(DI)";
    regs_16bitmode[6] = "(BP)";
    regs_16bitmode[7] = "(BX)";

    list1[0] = (uint16_t *)&registers["EBX"];
    list1[1] = (uint16_t *)&registers["EBP"];

    list2[0] = (uint16_t *)&registers["ESI"];
    list2[1] = (uint16_t *)&registers["EDI"];

    list3[0] = (uint16_t *)&registers["ESI"];
    list3[1] = (uint16_t *)&registers["EDI"];
    list3[2] = (uint16_t *)&registers["EBP"];
    list3[3] = (uint16_t *)&registers["EBX"];
}

string Bitset_addOverride::decode_displacement_without_SIB(int mod, int reg, int rm)
{
    string dispWithoutSIB = "";
    int disp_bytes[] = {2, 1, 2};
    int bytes = disp_bytes[mod];

    int disp = common.assemble_bits(bytes, cs, registers);

    if (imm)
    {
        int8_t imd = common.assemble_bits(1, cs, registers);
        if (mod == 0)
        {
            if (opSize)
            {
                memoryAccesses.push_back("bt " + common.getHex(disp, 0, 0) + ",$" + common.getHex(imd, 0, 0));

                bool bit = common.get_bits(imd % 16 + 1, 1, memories16bit[common.getHex(disp, 0, 0)]);
                memoryAccesses.push_back("Read " + to_string(imd % 16) + "bit from " + common.getHex(disp, 0, 0));

                if (bit)
                {
                    registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                }
                else
                {
                    registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                }

                dispWithoutSIB = "bt " + common.getHex(disp, 0, 0) + ",$" + common.getHex(imd, 0, 0) + "\n";
            }
            else
            {
                memoryAccesses.push_back("bt " + common.getHex(disp, 0, 0) + ",$" + common.getHex(imd, 0, 0));

                bool bit = common.get_bits(imd % 32 + 1, 1, memories32bit[common.getHex(disp, 0, 0)]);
                memoryAccesses.push_back("Read " + to_string(imd % 32) + "bit from " + common.getHex(disp, 0, 0));

                if (bit)
                {
                    registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                }
                else
                {
                    registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                }

                dispWithoutSIB = "bt " + common.getHex(disp, 0, 0) + ",$" + common.getHex(imd, 0, 0) + "\n";
            }
        }
        else
        {
            if (opSize)
            {
                memoryAccesses.push_back("bt " + common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + "),$" + common.getHex(imd, 0, 0));

                bool bit;

                if (rm < 4)
                {
                    bit = common.get_bits(imd % 16 + 1, 1, memories16bit[common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0)]);
                    memoryAccesses.push_back("Read " + to_string(imd % 16) + "bit from " + common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0));
                }
                else
                {
                    bit = common.get_bits(imd % 16 + 1, 1, memories16bit[common.getHex(*list3[rm % 4] + disp, 0, 0)]);
                    memoryAccesses.push_back("Read " + to_string(imd % 16) + " from " + common.getHex(*list3[rm % 4] + disp, 0, 0));
                }

                if (bit)
                {
                    registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                }
                else
                {
                    registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                }

                dispWithoutSIB = "bt " + common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + "),$" + common.getHex(imd, 0, 0) + "\n";
            }
            else
            {
                memoryAccesses.push_back("bt " + common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + "),$" + common.getHex(imd, 0, 0));

                bool bit;

                if (rm < 4)
                {
                    bit = common.get_bits(imd % 32 + 1, 1, memories32bit[common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0)]);
                    memoryAccesses.push_back("Read " + to_string(imd % 32) + "bit from " + common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0));
                }
                else
                {
                    bit = common.get_bits(imd % 32 + 1, 1, memories32bit[common.getHex(*list3[rm % 4] + disp, 0, 0)]);
                    memoryAccesses.push_back("Read " + to_string(imd % 32) + " from " + common.getHex(*list3[rm % 4] + disp, 0, 0));
                }

                if (bit)
                {
                    registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                }
                else
                {
                    registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                }

                dispWithoutSIB = "bt " + common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + "),$" + common.getHex(imd, 0, 0) + " \n";
            }
        }
    }
    else
    {
        if (mod == 0)
        {
            if (opSize)
            {
                memoryAccesses.push_back("bt " + common.getHex(disp, 0, 0) + ",%" + regs_16[reg]);

                int16_t num1;

                num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                bool bit = common.get_bits(num1 % 16 + 1, 1, memories16bit[common.getHex(disp, 0, 0)]);
                memoryAccesses.push_back("Read " + to_string(num1 % 16) + "bit from " + common.getHex(disp, 0, 0));

                if (bit)
                {
                    registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                }
                else
                {
                    registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                }

                dispWithoutSIB = "bt " + common.getHex(disp, 0, 0) + ",%" + regs_16[reg] + "\n";
            }
            else
            {
                memoryAccesses.push_back("bt " + common.getHex(disp, 0, 0) + ",%" + regs_32[reg]);

                int num2 = registers[regs_32[reg]];
                bool bit = common.get_bits(num2 % 32 + 1, 1, memories32bit[common.getHex(disp, 0, 0)]);
                memoryAccesses.push_back("Read " + to_string(num2 % 32) + "bit from " + common.getHex(disp, 0, 0));

                if (bit)
                {
                    registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                }
                else
                {
                    registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                }

                dispWithoutSIB = "bt " + common.getHex(disp, 0, 0) + ",%" + regs_32[reg] + "\n";
            }
        }
        else
        {
            if (opSize)
            {
                memoryAccesses.push_back("bt " + common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + "),%" + regs_16[reg]);

                int16_t num1;
                bool bit;

                num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                if (rm < 4)
                {
                    bit = common.get_bits(num1 % 16 + 1, 1, memories16bit[common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0)]);
                    memoryAccesses.push_back("Read " + to_string(num1 % 16) + "bit from " + common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0));
                }
                else
                {
                    bit = common.get_bits(num1 % 16 + 1, 1, memories16bit[common.getHex(*list3[rm % 4] + disp, 0, 0)]);
                    memoryAccesses.push_back("Read " + to_string(num1 % 16) + " from " + common.getHex(*list3[rm % 4] + disp, 0, 0));
                }

                if (bit)
                {
                    registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                }
                else
                {
                    registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                }

                dispWithoutSIB = "bt " + common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + "),%" + regs_16[reg] + "\n";
            }
            else
            {
                memoryAccesses.push_back("bt " + common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + "),%" + regs_32[reg]);

                int num2 = registers[regs_32[reg]];
                bool bit;

                if (rm < 4)
                {
                    bit = common.get_bits(num2 % 32 + 1, 1, memories32bit[common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0)]);
                    memoryAccesses.push_back("Read " + to_string(num2 % 32) + "bit from " + common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0));
                }
                else
                {
                    bit = common.get_bits(num2 % 32 + 1, 1, memories32bit[common.getHex(*list3[rm % 4] + disp, 0, 0)]);
                    memoryAccesses.push_back("Read " + to_string(num2 % 32) + " from " + common.getHex(*list3[rm % 4] + disp, 0, 0));
                }

                if (bit)
                {
                    registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
                }
                else
                {
                    registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
                }

                dispWithoutSIB = "bt " + common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + "),%" + regs_32[reg] + "\n";
            }
        }
    }

    return dispWithoutSIB;
};

string Bitset_addOverride::decode_mod_00(int reg, int rm)
{
    string string00 = "";
    if (rm == 6)
    {
        string00 = decode_displacement_without_SIB(0, reg, 5);
    }
    else
    {
        if (opSize)
        {
            memoryAccesses.push_back("bt (%" + regs_16bitmode[rm] + "),%" + regs_16[reg]);
            string00 = "bt (%" + regs_16bitmode[rm] + "),%" + regs_16[reg] + "\n";

            int16_t num1;
            bool bit;

            num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

            if (rm < 4)
            {
                bit = common.get_bits(num1 % 16 + 1, 1, memories16bit[common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0)]);
                memoryAccesses.push_back("Read " + to_string(num1 % 16) + "bit from " + common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0));
            }
            else
            {
                bit = common.get_bits(num1 % 16 + 1, 1, memories16bit[common.getHex(*list3[rm % 4], 0, 0)]);
                memoryAccesses.push_back("Read " + to_string(num1 % 16) + " from " + common.getHex(*list3[rm % 4], 0, 0));
            }

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
            memoryAccesses.push_back("bt (%" + regs_16bitmode[rm] + "),%" + regs_32[reg]);

            int num2 = registers[regs_32[reg]];
            bool bit;
            if (rm < 4)
            {

                bit = common.get_bits(num2 % 32 + 1, 1, memories32bit[common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0)]);
                memoryAccesses.push_back("Read " + to_string(num2 % 32) + "bit from " + common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0));
            }
            else
            {
                bit = common.get_bits(num2 % 32 + 1, 1, memories32bit[common.getHex(*list3[rm % 4], 0, 0)]);
                memoryAccesses.push_back("Read " + to_string(num2 % 32) + " from " + common.getHex(*list3[rm % 4], 0, 0));
            }
            if (bit)
            {
                registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
            }
            else
            {
                registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
            }

            string00 = "bt (%" + regs_16bitmode[rm] + "),%" + regs_32[reg] + "\n";
        }
    }

    return string00;
}

string Bitset_addOverride::decode_mod_01(int reg, int rm)
{
    string string01 = decode_displacement_without_SIB(1, reg, rm);
    return string01;
}

string Bitset_addOverride::decode_mod_10(int reg, int rm)
{
    string string10 = decode_displacement_without_SIB(2, reg, rm);
    return string10;
}

string Bitset_addOverride::decode_mod_11(int reg, int rm)
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

            string11 = "bt %" + regs_16bitmode[rm] + ",$" + common.getHex(imd, 0, 0) + "\n";
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

            string11 = "bt %" + regs_32[rm] + ",%" + regs_16bitmode[reg] + "\n";
        }
    }

    return string11;
}

string Bitset_addOverride::decode_bt(short prefixes[4])
{
    string decoded_bytes;

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

    return decoded_bytes;
}
