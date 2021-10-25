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

#include "and_immediate.h"

using namespace std;

And_override_immediate::And_override_immediate(Common com, queue<short> &instruction, map<string, int> &registers, map<string, int> &memories32bit, map<string, int16_t> &memories16bit, map<string, int8_t> &memories8bit, list<string> &memoryAccesses)
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

string And_override_immediate::decode_displacement_without_SIB(int w, int d, int mod, int rm)
{
    string dispWithoutSIB = "";
    int disp_bytes[] = {2, 1, 2};
    int bytes = disp_bytes[mod];

    int disp = common.assemble_bits(bytes, instruction, registers);

    if (mod == 0)
    {
        if (w == 0)
        {
            int8_t imm = common.assemble_bits(1, instruction, registers);
            memoryAccesses.push_back("mov " + common.getHex(disp, 0, 0) + ", $" + common.getHex(imm, 0, 0));

            memories8bit[common.getHex(disp, 0, 0)] = imm;
            memoryAccesses.push_back("Write " + to_string(imm) + " to " + common.getHex(disp, 0, 0));

            dispWithoutSIB = common.getHex(disp, 0, 0) + ", $" + common.getHex(imm, 0, 0) + "\n";
        }
        else if (w == 1)
        {
            if (opSize)
            {
                int16_t imm = common.assemble_bits(2, instruction, registers);
                memoryAccesses.push_back("mov " + common.getHex(disp, 0, 0) + ", $" + common.getHex(imm, 0, 0));

                memories16bit[common.getHex(disp, 0, 0)] = imm;
                memoryAccesses.push_back("Write " + to_string(imm) + " to " + common.getHex(disp, 0, 0));

                dispWithoutSIB = common.getHex(disp, 0, 0) + ", $" + common.getHex(imm, 0, 0) + "\n";
            }
            else
            {
                int imm = common.assemble_bits(4, instruction, registers);
                memoryAccesses.push_back("mov " + common.getHex(disp, 0, 0) + ", $" + common.getHex(imm, 0, 0));

                memories32bit[common.getHex(disp, 0, 0)] = imm;
                memoryAccesses.push_back("Write " + to_string(imm) + " to " + common.getHex(disp, 0, 0));

                dispWithoutSIB = common.getHex(disp, 0, 0) + ", $" + common.getHex(imm, 0, 0) + "\n";
            }
        }
    }
    else
    {
        if (w == 0)
        {
            int8_t imm = common.assemble_bits(1, instruction, registers);
            memoryAccesses.push_back("mov " + common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + ")" + ", $" + common.getHex(imm, 0, 0));

            if (rm < 4)
            {
                memories8bit[common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0)] = imm;
                memoryAccesses.push_back("Write " + to_string(imm) + " to " + common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0));
            }
            else
            {
                memories8bit[common.getHex(*list3[rm % 4] + disp, 0, 0)] = imm;
                memoryAccesses.push_back("Write " + to_string(imm) + " to " + common.getHex(*list3[rm % 4] + disp, 0, 0));
            }

            dispWithoutSIB = common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + ")" + ", $" + common.getHex(imm, 0, 0) + " \n";
        }
        else if (w == 1)
        {
            if (opSize)
            {
                int16_t imm = common.assemble_bits(2, instruction, registers);
                memoryAccesses.push_back("mov " + common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + ")" + ", $" + common.getHex(imm, 0, 0));

                if (rm < 4)
                {
                    memories16bit[common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0)] = imm;
                    memoryAccesses.push_back("Write " + to_string(imm) + " to " + common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0));
                }
                else
                {
                    memories16bit[common.getHex(*list3[rm % 4] + disp, 0, 0)] = imm;
                    memoryAccesses.push_back("Write " + to_string(imm) + " to " + common.getHex(*list3[rm % 4] + disp, 0, 0));
                }

                dispWithoutSIB = common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + ")" + ", $" + common.getHex(imm, 0, 0) + " \n";
            }
            else
            {
                    int imm = common.assemble_bits(4, instruction, registers);
                    memoryAccesses.push_back("mov " + common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + ")" + ", $" + common.getHex(imm, 0, 0));

                    if (rm < 4)
                    {
                        memories32bit[common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0)] = imm;
                        memoryAccesses.push_back("Write " + to_string(imm) + " to " + common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0));
                    }
                    else
                    {
                        memories32bit[common.getHex(*list3[rm % 4] + disp, 0, 0)] = imm;
                        memoryAccesses.push_back("Write " + to_string(imm) + " to " + common.getHex(*list3[rm % 4] + disp, 0, 0));
                    }
                    dispWithoutSIB = common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + ")" + ", $" + common.getHex(imm, 0, 0) + " \n";
            }
        }
    }
    return dispWithoutSIB;
};

string And_override_immediate::decode_mod_00(int w, int d, int rm)
{
    string string00 = "";
    if (rm == 6)
    {
        string00 = decode_displacement_without_SIB(w, d, 0, 5);
    }
    else
    {
        if (w == 0)
        {
            int8_t imm = common.assemble_bits(1, instruction, registers);
            memoryAccesses.push_back("mov (%" + regs_16bitmode[rm] + ")"+", $" + common.getHex(imm, 0, 0));
            string00 =  "(%" + regs_16bitmode[rm] + ")"+", $" + common.getHex(imm, 0, 0) +"\n";

            if (rm < 4)
            {
                memories8bit[common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0)] = imm;
                memoryAccesses.push_back("Write " + to_string(imm) + " to " + common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0));
            }
            else
            {
                memories8bit[common.getHex(*list3[rm % 4], 0, 0)] = imm;
                memoryAccesses.push_back("Write " + to_string(imm) + " to " + common.getHex(*list3[rm % 4], 0, 0));
            }
        }
        else if (w == 1)
        {
            if (opSize)
            {
                int16_t imm = common.assemble_bits(2, instruction, registers);
                memoryAccesses.push_back("mov (%" + regs_16bitmode[rm] + ")"+", $" + common.getHex(imm, 0, 0));
                string00 =  "(%" + regs_16bitmode[rm] + ")"+", $" + common.getHex(imm, 0, 0) +"\n";

                if (rm < 4)
                {
                    memories16bit[common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0)] = imm;
                    memoryAccesses.push_back("Write " + to_string(imm) + " to " + common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0));
                }
                else
                {
                    memories16bit[common.getHex(*list3[rm % 4], 0, 0)] = imm;
                    memoryAccesses.push_back("Write " + to_string(imm) + " to " + common.getHex(*list3[rm % 4], 0, 0));
                }
            }
            else
            {
                int imm = common.assemble_bits(4, instruction, registers);
                memoryAccesses.push_back("mov (%" + regs_16bitmode[rm] + ")"+", $" + common.getHex(imm, 0, 0));

                if (rm < 4)
                {
                    memories32bit[common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0)] = imm;
                    memoryAccesses.push_back("Write " + to_string(imm) + " to " + common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0));
                }
                else
                {
                    memories32bit[common.getHex(*list3[rm % 4], 0, 0)] = imm;
                    memoryAccesses.push_back("Write " + to_string(imm) + " to " + common.getHex(*list3[rm % 4], 0, 0));
                }

                string00 = "(%" + regs_16bitmode[rm] + ")"+", $" + common.getHex(imm, 0, 0) + "\n";
            }
        }
    }
    return string00;
}

string And_override_immediate::decode_mod_01(int w, int d, int rm)
{
    string string01 = decode_displacement_without_SIB(w, d, 1, rm);
    return string01;
}

string And_override_immediate::decode_mod_10(int w, int d, int rm)
{
    string string10 = decode_displacement_without_SIB(w, d, 2, rm);
    return string10;
}

string And_override_immediate::decode_mod_11(int w, int d, int rm)
{
    string string11 = "";
    if (w == 0)
    {
        int8_t imm;
        imm = common.assemble_bits(1, instruction, registers);

        if (rm < 4)
        {
            registers[regs_32[rm]] = ((registers[regs_32[rm]]) & 0xffffff00) | (imm & 0x000000ff);
        }
        else
        {
            registers[regs_32[rm]] = ((registers[regs_32[rm]]) & 0xffff00ff) | ((imm << 8) & 0x0000ff00);
        }

        string11 =  "%" + regs_8[rm] + ", $" + common.getHex(imm, 0, 0) +"\n";
    }
    else if (w == 1)
    {
        if (opSize)
        {
            int16_t imm;
            imm = common.assemble_bits(2, instruction, registers);

            registers[regs_32[rm]] = ((registers[regs_32[rm]]) & 0xffff0000) | (imm & 0x0000ffff);

            string11 =  "%" + regs_16[rm] +", $" + common.getHex(imm, 0, 0) + "\n";
        }
        else
        {
            int imm = common.assemble_bits(4, instruction, registers);

            registers[regs_32[rm]] = imm;

            string11 =  "%" + regs_32[rm] + ", $" + common.getHex(imm, 0, 0) +"\n";
        }
    }

    return string11;
}

string And_override_immediate::decode_imm(short prefixes[4], int w, int d, int mod, int rm)
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
        decoded_bytes = decode_mod_00(w,d, rm);
    }
    else if (mod == 1)
    {
        decoded_bytes = decode_mod_01(w,d ,rm); 
    }
    else if (mod == 2)
    {
        decoded_bytes = decode_mod_10(w,d ,rm);
    }
    else
    {
        decoded_bytes = decode_mod_11(w,d, rm);
    }
    return decoded_bytes;
}
