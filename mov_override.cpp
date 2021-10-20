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

Mov_override::Mov_override(Common com, queue<short> &instruction, map<string, int> &registers, map<string, int> &memories32bit, map<string, int16_t> &memories16bit, map<string, int8_t> &memories8bit, list<string> &memoryAccesses)
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

string Mov_override::decode_displacement_without_SIB(int w, int d, int mod, int reg, int rm)
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
                dispWithoutSIB =common.getHex(disp, 0, 0) + ",%" + regs_8[reg] + "\n";
                memoryAccesses.push_back("mov " + common.getHex(disp, 0, 0) + ",%" + regs_8[reg]);

                int8_t num1;

                if (reg < 4)
                {
                    num1 = common.get_bits(1, 8, registers[regs_32[reg]]);
                }
                else
                {
                    num1 = common.get_bits(9, 8, registers[regs_32[reg % 4]]);
                }

                memories8bit[common.getHex(disp, 0, 0)] = num1;
                memoryAccesses.push_back("Write " + to_string(num1) + " to " + common.getHex(disp, 0, 0));
            }
            else if (d == 1)
            {
                dispWithoutSIB =  "%" + regs_8[reg] + "," + common.getHex(disp, 0, 0) + "\n";
                memoryAccesses.push_back("mov %" + regs_8[reg] + "," + common.getHex(disp, 0, 0));
                int8_t num1;

                if (reg < 4)
                {
                    num1 = memories8bit[common.getHex(disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(disp, 0, 0));

                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (num1 & 0x000000ff);
                }
                else
                {
                    num1 = memories8bit[common.getHex(disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(disp, 0, 0));

                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff00ff) | ((num1 << 8) & 0x0000ff00);
                }
            }
        }
        else if (w == 1)
        {
            if (opSize)
            {
                if (d == 0)
                {
                    memoryAccesses.push_back("mov " + common.getHex(disp, 0, 0) + ",%" + regs_16[reg]);

                    int16_t num1;
                    num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                    memories16bit[common.getHex(disp, 0, 0)] = num1;
                    memoryAccesses.push_back("Write " + to_string(num1) + " to " + common.getHex(disp, 0, 0));

                    dispWithoutSIB = common.getHex(disp, 0, 0) + ",%" + regs_16[reg] + "\n";
                }
                else if (d == 1)
                {
                    int16_t num1;
                    num1 = memories16bit[common.getHex(disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(disp, 0, 0));

                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff0000) | (num1 & 0x0000ffff);

                    dispWithoutSIB = "%" + regs_16[reg] + "," + common.getHex(disp, 0, 0) + "\n";
                }
            }
            else
            {
                if (d == 0)
                {
                    memoryAccesses.push_back("mov " + common.getHex(disp, 0, 0) + ",%" + regs_32[reg] );


                    int num1 = registers[regs_32[reg]];

                    memories32bit[common.getHex(disp, 0, 0)] = num1;
                    memoryAccesses.push_back("Write " + to_string(num1) + " to " + common.getHex(disp, 0, 0));

                    dispWithoutSIB = common.getHex(disp, 0, 0) + ",%" + regs_32[reg] + "\n";
                }
                else
                {
                    memoryAccesses.push_back("mov %" + regs_32[reg] + "," + common.getHex(disp, 0, 0));
                    int num1 = memories32bit[common.getHex(disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(disp, 0, 0));

                    registers[regs_32[reg]] = num1;

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
                memoryAccesses.push_back("mov " +common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + "),%" + regs_8[reg]);
                int8_t num1;

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
                    memories8bit[common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0)] = num1;
                    memoryAccesses.push_back("Write " + to_string(num1) + " to " + common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0));
                }
                else
                {
                    memories8bit[common.getHex(*list3[rm % 4] + disp, 0, 0)] = num1;
                    memoryAccesses.push_back("Write " + to_string(num1) + " to " + common.getHex(*list3[rm % 4] + disp, 0, 0));
                }

                dispWithoutSIB = common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + "),%" + regs_8[reg] + "\n";
            }
            else if (d == 1)
            {
                memoryAccesses.push_back("mov %" + regs_8[reg] + "," + common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + ")");
                dispWithoutSIB = "%" + regs_8[reg] + "," + common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + ") \n";
                int8_t num1;

                if (rm < 4)
                {
                    num1 = memories8bit[common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0));
                }
                else
                {
                    num1 = memories8bit[common.getHex(*list3[rm % 4] + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(*list3[rm % 4] + disp, 0, 0));
                }

                if (reg < 4)
                {
                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (num1 & 0x000000ff);
                }
                else
                {
                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff00ff) | ((num1 << 8) & 0x0000ff00);
                }
            }
        }
        else if (w == 1)
        {
            if (opSize)
            {
                if (d == 0)
                {
                    memoryAccesses.push_back("mov " + common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + "),%" + regs_16[reg]);

                    int16_t num1;
                    num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                    if (rm < 4)
                    {
                        memories16bit[common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0)] = num1;
                        memoryAccesses.push_back("Write " + to_string(num1) + " to " + common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0));
                    }
                    else
                    {
                        memories16bit[common.getHex(*list3[rm % 4] + disp, 0, 0)] = num1;
                        memoryAccesses.push_back("Write " + to_string(num1) + " to " + common.getHex(*list3[rm % 4] + disp, 0, 0));
                    }

                    dispWithoutSIB = common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + "),%" + regs_16[reg] + "\n";
                }
                else if (d == 1)
                {
                    int16_t num1;

                    dispWithoutSIB = "%" + regs_16[reg] + "," + common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + ") \n";
                    memoryAccesses.push_back("mov %" + regs_16[reg] + "," + common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + ") \n");

                    if (rm < 4)
                    {
                        num1 = memories16bit[common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0));
                    }
                    else
                    {
                        num1 = memories16bit[common.getHex(*list3[rm % 4] + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(*list3[rm % 4] + disp, 0, 0));
                    }

                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff0000) | (num1 & 0x0000ffff);
                }
            }
            else
            {
                if (d == 0)
                {
                    memoryAccesses.push_back("mov " + common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + "),%" + regs_32[reg] );

                    int num1;

                    if (rm < 4)
                    {
                        num1 = registers[regs_32[reg]];

                        memories32bit[common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0)] = num1;
                        memoryAccesses.push_back("Write " + to_string(num1) + " to " + common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0));
                    }
                    else
                    {
                        num1 = registers[regs_32[reg]];

                        memories32bit[common.getHex(*list3[rm % 4] + disp, 0, 0)] = num1;
                        memoryAccesses.push_back("Write " + to_string(num1) + " to " + common.getHex(*list3[rm % 4] + disp, 0, 0));
                    }

                    dispWithoutSIB = common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + "),%" + regs_32[reg] + "\n";
                }
                else
                {
                    memoryAccesses.push_back("mov %" + regs_32[reg] + "," + common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + ")");
                    dispWithoutSIB = "%" + regs_32[reg] + "," + common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + ") \n";

                    int num1;

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
                    registers[regs_32[reg]] = num1;                    
                }
            }
        }
    }

    return dispWithoutSIB;
};

string Mov_override::decode_mod_00(int w, int d, int reg, int rm)
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
                memoryAccesses.push_back("mov (%" + regs_16bitmode[rm] + "),%" + regs_8[reg]);
                string00 = "(%" + regs_16bitmode[rm] + "),%" + regs_8[reg] + "\n";

                int8_t num1;

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
                    memories8bit[common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0)] = num1;
                    memoryAccesses.push_back("Write " + to_string(num1) + " to " + common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0));
                }
                else
                {
                    memories8bit[common.getHex(*list3[rm % 4], 0, 0)] = num1;
                    memoryAccesses.push_back("Write " + to_string(num1) + " to " + common.getHex(*list3[rm % 4], 0, 0));
                }

            }
            else if (d == 1)
            {
                memoryAccesses.push_back("mov %" + regs_8[reg] + ",(%" + regs_16bitmode[rm] + ")");
                string00 = "%" + regs_8[reg] + ",(%" + regs_16bitmode[rm] + ")\n";

                int8_t num1;

                if (rm < 4)
                {
                    num1 = memories8bit[common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0));
                }
                else
                {
                    num1 = memories8bit[common.getHex(*list3[rm % 4], 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(*list3[rm % 4], 0, 0));
                }

                if (reg < 4)
                {
                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (num1 & 0x000000ff);
                }
                else
                {
                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff00ff) | ((num1 << 8) & 0x0000ff00);
                }
            }
        }
        else if (w == 1)
        {
            if (opSize)
            {
                if (d == 0)
                {
                    memoryAccesses.push_back("mov (%" + regs_16bitmode[rm] + "),%" + regs_16[reg]);
                    string00 = "(%" + regs_16bitmode[rm] + "),%" + regs_16[reg] + "\n";

                    int16_t num1;

                    num1 = common.get_bits(1, 16, registers[regs_32[reg]]);

                    if (rm < 4)
                    {
                        memories16bit[common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0)] = num1;
                        memoryAccesses.push_back("Write " + to_string(num1) + " to " + common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0));
                    }
                    else
                    {
                        memories16bit[common.getHex(*list3[rm % 4], 0, 0)] = num1;
                        memoryAccesses.push_back("Write " + to_string(num1) + " to " + common.getHex(*list3[rm % 4], 0, 0));
                    }
                }
                else if (d == 1)
                {
                    memoryAccesses.push_back("mov %" + regs_16[reg] + ",(%" + regs_16bitmode[rm] + ")");
                    string00 = "%" + regs_16[reg] + ",(%" + regs_16bitmode[rm] + ")\n";

                    int16_t num1;

                    if (rm < 4)
                    {
                        num1 = memories16bit[common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0));
                    }
                    else
                    {
                        num1 = memories16bit[common.getHex(*list3[rm % 4], 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(*list3[rm % 4], 0, 0));
                    }
                    registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff0000) | (num1 & 0x0000ffff);
                }
            }
            else
            {
                if (d == 0)
                {
                    memoryAccesses.push_back("mov (%" + regs_16bitmode[rm] + "),%" + regs_32[reg]);

                    int num1;
                    num1 = registers[regs_32[reg]];
                    if (rm < 4)
                    {
                                                memories32bit[common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0)] = num1;
                        memoryAccesses.push_back("Write " + to_string(num1) + " to " + common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0));
                    }
                    else
                    {
                        memories32bit[common.getHex(*list3[rm % 4], 0, 0)] = num1;
                        memoryAccesses.push_back("Write " + to_string(num1) + " to " + common.getHex(*list3[rm % 4], 0, 0));
                    }

                    string00 = "(%" + regs_16bitmode[rm] + "),%" + regs_32[reg] + "\n";
                }
                else
                {
                    memoryAccesses.push_back("mov %" + regs_32[reg] + ",(%" + regs_16bitmode[rm] + ")"); 
                    string00 = "%" + regs_32[reg] + ",(%" + regs_16bitmode[rm] + ")\n";
                    int num1;

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
                    registers[regs_32[reg]] = num1;
                }
            }
        }
    }

    return string00;
}

string Mov_override::decode_mod_01(int w, int d, int reg, int rm)
{
    string string01 = decode_displacement_without_SIB(w, d, 1, reg, rm);
    return string01;
}

string Mov_override::decode_mod_10(int w, int d, int reg, int rm)
{
    string string10 = decode_displacement_without_SIB(w, d, 2, reg, rm);
    return string10;
}

string Mov_override::decode_mod_11(int w, int d, int reg, int rm)
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
                registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff00ff) | ((registers[regs_32[rm]] << 8) & 0x0000ff00); 
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
                registers[regs_32[rm]] = ((registers[regs_32[rm]]) & 0xffff00ff) | ((registers[regs_32[reg]] << 8) & 0x0000ff00); 
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
            registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffff00ff) | ((imm << 8) & 0x0000ff00);
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

string Mov_override::decode_mov(short prefixes[4])
{
    printf ("Mov when address override is present\n");

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

    if(opCode == 0xb0 or opCode == 0xb8)
    {
        bool w = common.get_bits(4, 1, opCode);
        int reg = common.get_bits(0, 3, opCode);
        decoded_bytes = decode_imm_alt(opCode,w,reg);
    }
    else if(opCode == 0xc7 or opCode == 0xc6)
    {
        int mod = instruction.front() >> 6;
        int reg = common.get_bits(4, 3, instruction.front());
        int rm = common.get_bits(1, 3, instruction.front());

        instruction.pop();
        registers["EIP"] = registers["EIP"] + 1;
            
        Mov_override_immediate mov_override_immediate(common, instruction, registers, memories32bit, memories16bit, memories8bit, memoryAccesses);
        decoded_bytes = mov_override_immediate.decode_imm(prefixes, w, mod, rm);            
    }
    else
    {
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
    return decoded_bytes;
}