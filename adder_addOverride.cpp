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
    regs_8[0] = "CL";
    regs_8[0] = "DL";
    regs_8[0] = "BL";
    regs_8[0] = "AH";
    regs_8[0] = "CH";
    regs_8[0] = "DH";
    regs_8[0] = "BH";

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

string Adder_addOverride::decode_displacement_without_SIB(int w, int d, int mod, int reg, int rm)
{
    string dispWithoutSIB = "";
    int disp_bytes[] = {2, 1, 2};
    int bytes = disp_bytes[mod];

    int disp = common.assemble_bits(bytes, instruction, registers);

    if (mod == 0)
    {
        if (w == 0)
        {
            if (d == 0)
            {
                memoryAccesses.push_back("add " + common.getHex(disp, 0, 0) + ",%" + regs_8[reg]);

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
                num2 = memories8bit[common.getHex(disp, 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(disp, 0, 0));

                num3 = num1 + num2;
                num4 = unsigned(num3);

                memories8bit[common.getHex(disp, 0, 0)] = num3;
                memoryAccesses.push_back("Write " + to_string(num3) + " to " + common.getHex(disp, 0, 0));

                common.setOverflow8bit(num1, num2, num3, registers);
                common.setCarry8bit(num1, num4, registers);
                common.setSign(num3, registers);
                common.setZero(num3, registers);

                dispWithoutSIB =common.getHex(disp, 0, 0) + ",%" + regs_8[reg] + "\n";
            }
            else if (d == 1)
            {
                int8_t num1, num2, num3;
                uint8_t num4;

                if (reg < 4)
                {
                    num1 = common.get_bits(1, 8, registers[regs_32[reg]]);
                    num2 = memories8bit[common.getHex(disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(disp, 0, 0));

                    num3 = num1 + num2;
                    num4 = unsigned(num3);

                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (num3 & 0x000000ff);
                }
                else
                {
                    num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                    num2 = memories8bit[common.getHex(disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(disp, 0, 0));

                    num3 = num1 + num2;
                    num4 = unsigned(num3);

                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff00ff) | ((num3<<8) & 0x0000ff00);
                }

                common.setOverflow8bit(num1, num2, num3, registers);
                common.setCarry8bit(num1, num4, registers);
                common.setSign(num3, registers);
                common.setZero(num3, registers);

                dispWithoutSIB =  "%" + regs_8[reg] + "," + common.getHex(disp, 0, 0) + "\n";
            }
        }
        else if (w == 1)
        {
            if (opSize)
            {
                if (d == 0)
                {
                    memoryAccesses.push_back("add " + common.getHex(disp, 0, 0) + ",%" + regs_16[reg]);

                    int16_t num1, num2, num3;
                    uint16_t num4;

                    num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                    num2 = memories16bit[common.getHex(disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(disp, 0, 0));

                    num3 = num1 + num2;
                    num4 = unsigned(num3);

                    memories16bit[common.getHex(disp, 0, 0)] = num3;
                    memoryAccesses.push_back("Write " + to_string(num3) + " to " + common.getHex(disp, 0, 0));

                    common.setOverflow16bit(num1, num2, num3, registers);
                    common.setCarry16bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    dispWithoutSIB = common.getHex(disp, 0, 0) + ",%" + regs_16[reg] + "\n";
                }
                else if (d == 1)
                {
                    int16_t num1, num2, num3;
                    uint16_t num4;

                    num1 = common.get_bits(1, 16, registers[regs_32[reg]]);
                    num2 = memories16bit[common.getHex(disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(disp, 0, 0));

                    num3 = num1 + num2;
                    num4 = unsigned(num3);

                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff0000) | (num3 & 0x0000ffff);

                    common.setOverflow16bit(num1, num2, num3, registers);
                    common.setCarry16bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    dispWithoutSIB = "%" + regs_16[reg] + "," + common.getHex(disp, 0, 0) + "\n";
                }
            }
            else
            {
                if (d == 0)
                {
                    memoryAccesses.push_back("add " + common.getHex(disp, 0, 0) + ",%" + regs_32[reg] );

                    int num1 = memories32bit[common.getHex(disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(disp, 0, 0));

                    int num2 = registers[regs_32[reg]];
                    int num3 = num1 + num2;
                    unsigned int num4 = unsigned(num3);

                    common.setOverflow32bit(num1, num2, num3, registers);
                    common.setCarry32bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    memories32bit[common.getHex(disp, 0, 0)] = num3;
                    memoryAccesses.push_back("Write " + to_string(num3) + " to " + common.getHex(disp, 0, 0));

                    dispWithoutSIB = common.getHex(disp, 0, 0) + ",%" + regs_32[reg] + "\n";
                }
                else
                {
                    int num1 = memories32bit[common.getHex(disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(disp, 0, 0));

                    int num2 = registers[regs_32[reg]];
                    int num3 = num1 + num2;
                    unsigned int num4 = unsigned(num3);

                    common.setOverflow32bit(num1, num2, num3, registers);
                    common.setCarry32bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    registers[regs_32[reg]] = num3;

                    dispWithoutSIB = "%" + regs_32[reg] + "," + common.getHex(disp, 0, 0) + "\n";
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
                memoryAccesses.push_back("add " +common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + "),%" + regs_8[reg]);

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
                    num2 = memories8bit[common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0));

                    num3 = num1 + num2;
                    num4 = unsigned(num3);

                    memories8bit[common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0)] = num3;
                    memoryAccesses.push_back("Write " + to_string(num3) + " to " + common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0));
                }
                else
                {
                    num2 = memories8bit[common.getHex(*list3[rm % 4] + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(*list3[rm % 4] + disp, 0, 0));

                    num3 = num1 + num2;
                    num4 = unsigned(num3);

                    memories8bit[common.getHex(*list3[rm % 4] + disp, 0, 0)] = num3;
                    memoryAccesses.push_back("Write " + to_string(num3) + " to " + common.getHex(*list3[rm % 4] + disp, 0, 0));
                }

                common.setOverflow8bit(num1, num2, num3, registers);
                common.setCarry8bit(num1, num4, registers);
                common.setSign(num3, registers);
                common.setZero(num3, registers);

                dispWithoutSIB = common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + "),%" + regs_8[reg] + "\n";
            }
            else if (d == 1)
            {
                int8_t num1, num2, num3;
                uint8_t num4;

                if (rm < 4)
                {
                    num2 = memories8bit[common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0));
                }
                else
                {
                    num2 = memories8bit[common.getHex(*list3[rm % 4] + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(*list3[rm % 4] + disp, 0, 0));
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

                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff00ff) | ((num3<<8) & 0x0000ff00);
                }

                common.setOverflow8bit(num1, num2, num3, registers);
                common.setCarry8bit(num1, num4, registers);
                common.setSign(num3, registers);
                common.setZero(num3, registers);

                dispWithoutSIB = "%" + regs_8[reg] + "," + common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + ") \n";
            }
        }
        else if (w == 1)
        {
            if (opSize)
            {
                if (d == 0)
                {
                    memoryAccesses.push_back("add " + common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + "),%" + regs_16[reg]);

                    int16_t num1, num2, num3;
                    uint16_t num4;

                    num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                    if (rm < 4)
                    {
                        num2 = memories16bit[common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0));

                        num3 = num1 + num2;
                        num4 = unsigned(num3);

                        memories16bit[common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0)] = num3;
                        memoryAccesses.push_back("Write " + to_string(num3) + " to " + common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0));
                    }
                    else
                    {
                        num2 = memories16bit[common.getHex(*list3[rm % 4] + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(*list3[rm % 4] + disp, 0, 0));

                        num3 = num1 + num2;
                        num4 = unsigned(num3);

                        memories16bit[common.getHex(*list3[rm % 4] + disp, 0, 0)] = num3;
                        memoryAccesses.push_back("Write " + to_string(num3) + " to " + common.getHex(*list3[rm % 4] + disp, 0, 0));
                    }

                    common.setOverflow16bit(num1, num2, num3, registers);
                    common.setCarry16bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    dispWithoutSIB = common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + "),%" + regs_16[reg] + "\n";
                }
                else if (d == 1)
                {
                    int16_t num1, num2, num3;
                    uint16_t num4;

                    if (rm < 4)
                    {
                        num2 = memories16bit[common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0));
                    }
                    else
                    {
                        num2 = memories16bit[common.getHex(*list3[rm % 4] + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(*list3[rm % 4] + disp, 0, 0));
                    }

                    num1 = common.get_bits(1, 16, registers[regs_32[reg]]);
                    num3 = num1 + num2;
                    num4 = unsigned(num3);

                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff0000) | (num3 & 0x0000ffff);

                    common.setOverflow16bit(num1, num2, num3, registers);
                    common.setCarry16bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    dispWithoutSIB = "%" + regs_16[reg] + "," + common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + ") \n";
                }
            }
            else
            {
                if (d == 0)
                {
                    memoryAccesses.push_back("add " + common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + "),%" + regs_32[reg] );

                    int num1, num2, num3;
                    unsigned int num4;

                    if (rm < 4)
                    {
                        num1 = memories32bit[common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0));

                        num2 = registers[regs_32[reg]];
                        num3 = num1 + num2;
                        num4 = unsigned(num3);

                        memories32bit[common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0)] = num3;
                        memoryAccesses.push_back("Write " + to_string(num3) + " to " + common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0));
                    }
                    else
                    {
                        num1 = memories32bit[common.getHex(*list3[rm % 4] + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(*list3[rm % 4] + disp, 0, 0));

                        num2 = registers[regs_32[reg]];
                        num3 = num1 + num2;
                        num4 = unsigned(num3);

                        memories32bit[common.getHex(*list3[rm % 4] + disp, 0, 0)] = num3;
                        memoryAccesses.push_back("Write " + to_string(num3) + " to " + common.getHex(*list3[rm % 4] + disp, 0, 0));
                    }

                    common.setOverflow32bit(num1, num2, num3, registers);
                    common.setCarry32bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    dispWithoutSIB = common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + "),%" + regs_32[reg] + "\n";
                }
                else
                {
                    int num1, num2, num3;
                    unsigned int num4;
                    if (rm < 4)
                    {
                        num1 = memories32bit[common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0));
                    }
                    else
                    {
                        num1 = memories32bit[common.getHex(*list3[rm % 4] + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(*list3[rm % 4] + disp, 0, 0));
                    }
                    num2 = registers[regs_32[reg]];
                    num3 = num1 + num2;
                    num4 = unsigned(num3);

                    common.setOverflow32bit(num1, num2, num3, registers);
                    common.setCarry32bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    registers[regs_32[reg]] = num3;

                    dispWithoutSIB = "%" + regs_32[reg] + "," + common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + ") \n";
                }
            }
        }
    }

    return dispWithoutSIB;
};

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
                memoryAccesses.push_back("add (%" + regs_16bitmode[rm] + "),%" + regs_8[reg]);
                string00 = "(%" + regs_16bitmode[rm] + "),%" + regs_8[reg] + "\n";

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
                    num2 = memories8bit[common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0));

                    num3 = num1 + num2;
                    num4 = unsigned(num3);

                    memories8bit[common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0)] = num3;
                    memoryAccesses.push_back("Write " + to_string(num3) + " to " + common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0));
                }
                else
                {
                    num2 = memories8bit[common.getHex(*list3[rm % 4], 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(*list3[rm % 4], 0, 0));

                    num3 = num1 + num2;
                    num4 = unsigned(num3);

                    memories8bit[common.getHex(*list3[rm % 4], 0, 0)] = num3;
                    memoryAccesses.push_back("Write " + to_string(num3) + " to " + common.getHex(*list3[rm % 4], 0, 0));
                }

                common.setOverflow8bit(num1, num2, num3, registers);
                common.setCarry8bit(num1, num4, registers);
                common.setSign(num3, registers);
                common.setZero(num3, registers);
            }
            else if (d == 1)
            {
                string00 = "%" + regs_8[reg] + ",(%" + regs_16bitmode[rm] + ")\n";

                int8_t num1, num2, num3;
                uint8_t num4;

                if (rm < 4)
                {
                    num2 = memories8bit[common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0));
                }
                else
                {
                    num2 = memories8bit[common.getHex(*list3[rm % 4], 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(*list3[rm % 4], 0, 0));
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
                    memoryAccesses.push_back("add (%" + regs_16bitmode[rm] + "),%" + regs_16[reg]);
                    string00 = "(%" + regs_16bitmode[rm] + "),%" + regs_16[reg] + "\n";

                    int16_t num1, num2, num3;
                    uint16_t num4;

                    num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                    if (rm < 4)
                    {
                        num2 = memories16bit[common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0));

                        num3 = num1 + num2;
                        num4 = unsigned(num3);

                        memories16bit[common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0)] = num3;
                        memoryAccesses.push_back("Write " + to_string(num3) + " to " + common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0));
                    }
                    else
                    {
                        num2 = memories16bit[common.getHex(*list3[rm % 4], 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(*list3[rm % 4], 0, 0));

                        num3 = num1 + num2;
                        num4 = unsigned(num3);

                        memories16bit[common.getHex(*list3[rm % 4], 0, 0)] = num3;
                        memoryAccesses.push_back("Write " + to_string(num3) + " to " + common.getHex(*list3[rm % 4], 0, 0));
                    }

                    common.setOverflow16bit(num1, num2, num3, registers);
                    common.setCarry16bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);
                }
                else if (d == 1)
                {
                    string00 = "%" + regs_16[reg] + ",(%" + regs_16bitmode[rm] + ")\n";

                    int16_t num1, num2, num3;
                    uint16_t num4;

                    num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                    if (rm < 4)
                    {
                        num2 = memories16bit[common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0));
                    }
                    else
                    {
                        num2 = memories16bit[common.getHex(*list3[rm % 4], 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(*list3[rm % 4], 0, 0));
                    }

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
                    memoryAccesses.push_back("add (%" + regs_16bitmode[rm] + "),%" + regs_32[reg]);

                    int num1, num2, num3;
                    unsigned int num4;
                    if (rm < 4)
                    {

                        num1 = memories32bit[common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0));

                        num2 = registers[regs_32[reg]]; //registers["ECX"]
                        num3 = num1 + num2;
                        num4 = unsigned(num3);

                        memories32bit[common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0)] = num3;
                        memoryAccesses.push_back("Write " + to_string(num3) + " to " + common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0));
                    }
                    else
                    {
                        num1 = memories32bit[common.getHex(*list3[rm % 4], 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(*list3[rm % 4], 0, 0));

                        num2 = registers[regs_32[reg]]; //registers["ECX"]
                        num3 = num1 + num2;
                        num4 = unsigned(num3);

                        memories32bit[common.getHex(*list3[rm % 4], 0, 0)] = num3;
                        memoryAccesses.push_back("Write " + to_string(num3) + " to " + common.getHex(*list3[rm % 4], 0, 0));
                    }

                    common.setOverflow32bit(num1, num2, num3, registers);
                    common.setCarry32bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    string00 = "(%" + regs_16bitmode[rm] + "),%" + regs_32[reg] + "\n";
                }
                else
                { 
                    int num1, num2, num3;
                    unsigned int num4;

                    if (rm < 4)
                    {
                        num1 = memories32bit[common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0));
                    }
                    else
                    {
                        num1 = memories32bit[common.getHex(*list3[rm % 4], 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(*list3[rm % 4], 0, 0));
                    }

                    num2 = registers[regs_32[reg]];
                    num3 = num1 + num2;
                    num4 = unsigned(num3);

                    common.setOverflow32bit(num1, num2, num3, registers);
                    common.setCarry32bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    string00 = "%" + regs_32[reg] + ",(%" + regs_16bitmode[rm] + ")\n";
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

string Adder_addOverride::decode_imm(int opCode, int w, int d)
{
    string dec_imm;
    int imm;

    if (w == 0)
    {
        int8_t num1, num2;
        uint8_t num3;
        imm = common.assemble_bits(1, instruction, registers);

        string st = common.getHex(imm, 0, 0);

        dec_imm =  "%%AL, $" + st;

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

string Adder_addOverride::decode_add(short prefixes[4])
{
    printf ("Adder when address override is present\n");

    short opCode = instruction.front();
    bool d = common.get_bits(2, 1, opCode);
    bool w = common.get_bits(1, 1, opCode);

    instruction.pop();
    registers["EIP"] = registers["EIP"] + 1;


    string decoded_bytes;
    if (prefixes[2] == 0x66)
    {
        opSize = true;
    }
    else
    {
        opSize = false;
    }

    if (opCode == 0x04 or opCode==0x05)
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
            Immediate_addOverride immediate_addOverride(common, instruction, registers, memories32bit, memories16bit, memories8bit, memoryAccesses);
            decoded_bytes = immediate_addOverride.decode_imm(prefixes, w, d, mod, rm);
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

    return decoded_bytes;
}