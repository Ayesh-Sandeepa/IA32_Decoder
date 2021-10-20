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

#include "mov_immediate.h"

using namespace std;

Mov_immediate::Mov_immediate(Common com, queue<short> &instruction, map<string, int> &registers, map<string, int> &memories32bit, map<string, int16_t> &memories16bit, map<string, int8_t> &memories8bit, list<string> &memoryAccesses)
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

string Mov_immediate::decode_displacement_with_SIB(int w, int mod, int index, int scale, int base)
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
                int8_t imm = common.assemble_bits(1, instruction, registers);
                if (index == 4)
                {
                    memoryAccesses.push_back("mov " +st + ", $" + common.getHex(imm, 0, 0));

                    memories8bit[st] = imm;
                    memoryAccesses.push_back("write " + to_string(imm) + " to " + st);

                    dispWithSIB = st + ", $" + common.getHex(imm, 0, 0)  + "\n";
                }
                else
                {
                    memoryAccesses.push_back("mov " + st + "(,%" + regs_32[index] + "," + to_string(scale) + "), $" + common.getHex(imm, 0, 0) );

                    memories8bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)] = imm;
                    memoryAccesses.push_back("write " + to_string(imm) + " to " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                    dispWithSIB = st + "(,%" + regs_32[index] + "," + to_string(scale) + "), $" + common.getHex(imm, 0, 0)  + "\n";
                }
            }
            else if (w == 1)
            {
                if (opSize)
                {
                    int16_t imm = common.assemble_bits(2, instruction, registers);
                    if (index == 4)
                    {
                        memoryAccesses.push_back("mov " + st + ", $" + common.getHex(imm, 0, 0));

                        memories16bit[st] = imm;
                        memoryAccesses.push_back("write " + to_string(imm) + " to " + st);

                        dispWithSIB =   st + ", $" + common.getHex(imm, 0, 0) +"\n";
                    }
                    else
                    {
                        memoryAccesses.push_back("mov " + st + "(,%" + regs_32[index] + "," + to_string(scale) + "), $" + common.getHex(imm, 0, 0));

                        memories16bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)] = imm;
                        memoryAccesses.push_back("write " + to_string(imm) + " to " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                        dispWithSIB =  st + "(,%" + regs_32[index] + "," + to_string(scale) + "), $" + common.getHex(imm, 0, 0) + "\n";
                    }

                }
                else
                {
                    int32_t imm = common.assemble_bits(4, instruction, registers);
                    if (index == 4)
                    {
                        memoryAccesses.push_back("mov " + st + ", $" + common.getHex(imm, 0, 0));

                        memories32bit[st] = imm;
                        memoryAccesses.push_back("write " + to_string(imm) + " to " + st);

                        dispWithSIB =  st + ", $" + common.getHex(imm, 0, 0) +"\n";
                    }
                    else
                    {
                        memoryAccesses.push_back("mov " + st + "(,%" + regs_32[index] + "," + to_string(scale) + "), $" + common.getHex(imm, 0, 0));

                        memories32bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)] = imm;
                        memoryAccesses.push_back("write " + to_string(imm) + " to " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                        dispWithSIB =  st + "(,%" + regs_32[index] + "," + to_string(scale) + "), $" + common.getHex(imm, 0, 0) + "\n";
                        }
                }
            }
        }
        else
        {
            if (w == 0)
            {
                int8_t imm = common.assemble_bits(1, instruction, registers);
                if (index == 4)
                {
                    memoryAccesses.push_back("mov (%" + regs_32[base] + "), $" + common.getHex(imm, 0, 0));

                    memories8bit[common.getHex(registers[regs_32[base]], 0, 0)] = imm;
                    memoryAccesses.push_back("write " + to_string(imm) + " to " + common.getHex(registers[regs_32[base]], 0, 0));

                    dispWithSIB =   "(%" + regs_32[base] + "), $" + common.getHex(imm, 0, 0) +"\n";
                }
                else
                {
                    memoryAccesses.push_back("mov (%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "$" + common.getHex(imm, 0, 0));

                    memories8bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0)] = imm;
                    memoryAccesses.push_back("write " + to_string(imm) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0));

                    dispWithSIB =  "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "$" + common.getHex(imm, 0, 0) + "\n";
                }
            }
            else if (w == 1)
            {
                if (opSize)
                {
                    int16_t imm = common.assemble_bits(2, instruction, registers);
                    if (index == 4)
                    {
                        memoryAccesses.push_back("mov (%" + regs_32[base] + "), $" + common.getHex(imm, 0, 0));

                        memories16bit[common.getHex(registers[regs_32[base]], 0, 0)] = imm;
                        memoryAccesses.push_back("write " + to_string(imm) + " to " + common.getHex(registers[regs_32[base]], 0, 0));

                        dispWithSIB =   "(%" + regs_32[base] + "), $" + common.getHex(imm, 0, 0) +"\n";
                    }
                    else
                    {
                        memoryAccesses.push_back("mov (%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "), $" + common.getHex(imm, 0, 0));

                        memories16bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)] = imm;
                        memoryAccesses.push_back("write " + to_string(imm) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                        dispWithSIB = "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "), $" + common.getHex(imm, 0, 0) + "\n";
                    }
                }
                else
                {
                    int32_t imm = common.assemble_bits(4, instruction, registers);
                    if (index == 4)
                    {
                        memoryAccesses.push_back("mov (%" + regs_32[base] + ")" +", $" + common.getHex(imm, 0, 0) );

                        memories32bit[common.getHex(registers[regs_32[base]], 0, 0)] = imm;
                        memoryAccesses.push_back("write " + to_string(imm) + " to " + common.getHex(registers[regs_32[base]], 0, 0));

                        dispWithSIB = "(%" + regs_32[base] + ")" +", $" + common.getHex(imm, 0, 0) + "\n";
                    }
                    else
                    {
                        memoryAccesses.push_back("mov (%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "$" + common.getHex(imm, 0, 0));

                        memories32bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)] = imm;
                        memoryAccesses.push_back("write " + to_string(imm) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                        dispWithSIB =  "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "$" + common.getHex(imm, 0, 0) + "\n";
                    }
                }
            }
        }
    }
    else
    {
        if (w == 0)
        {
            int8_t imm = common.assemble_bits(1, instruction, registers);
            if (index == 4)
            {
                memoryAccesses.push_back("mov " +st + "(%" + regs_32[base] + ")" + ", $" + common.getHex(imm, 0, 0));

                memories8bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)] = imm;
                memoryAccesses.push_back("write " + to_string(imm) + " to " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                dispWithSIB =  st + "(%" + regs_32[base] + ")" + ", $" + common.getHex(imm, 0, 0) + "\n";
            }
            else
            {
                memoryAccesses.push_back("mov " + st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + ", $" + common.getHex(imm, 0, 0));

                memories8bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)] = imm;
                memoryAccesses.push_back("write " + to_string(imm) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                dispWithSIB = st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + ", $" + common.getHex(imm, 0, 0) +"\n";
            }
        }
        else if (w == 1)
        {
            if (opSize)
            {
                int16_t imm = common.assemble_bits(2, instruction, registers);
                if (index == 4)
                {
                    memoryAccesses.push_back("mov " + st + "(%" + regs_32[base] + ")" + ", $" + common.getHex(imm, 0, 0));

                    memories16bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)] = imm;
                    memoryAccesses.push_back("write " + to_string(imm) + " to " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                    dispWithSIB =   st + "(%" + regs_32[base] + ")" + ", $" + common.getHex(imm, 0, 0) +"\n";
                }
                else
                {
                    memoryAccesses.push_back("mov " +  st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" +", $" + common.getHex(imm, 0, 0) );

                    memories16bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)] = imm;
                    memoryAccesses.push_back("write " + to_string(imm) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                    dispWithSIB =  st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" +", $" + common.getHex(imm, 0, 0) +"\n";
                }
            }
            else
            {
                int imm = common.assemble_bits(4, instruction, registers);
                if (index == 4)
                {
                    memoryAccesses.push_back("mov " + st + "(%" + regs_32[base] + ")" + ", $" + common.getHex(imm, 0, 0));

                    memories32bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)] = imm;
                    memoryAccesses.push_back("write " + to_string(imm) + " to " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                    dispWithSIB =   st + "(%" + regs_32[base] + ")" + ", $" + common.getHex(imm, 0, 0) +"\n";
                }
                else
                {
                    memoryAccesses.push_back("mov " +  st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + ", $" + common.getHex(imm, 0, 0));

                    memories32bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)] = imm;
                    memoryAccesses.push_back("write " + to_string(imm) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                    dispWithSIB =   st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + ", $" + common.getHex(imm, 0, 0) +"\n";
                }
            }
        }
    }
    return dispWithSIB;
}

string Mov_immediate::decode_displacement_without_SIB(int w, int mod, int rm)
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
            int8_t imm = common.assemble_bits(1, instruction, registers);
            memoryAccesses.push_back("mov " + st + ", $" + common.getHex(imm, 0, 0));

            memories8bit[st] = imm;
            memoryAccesses.push_back("write " + to_string(imm) + " to " + st);

            dispWithoutSIB = st + ", $" + common.getHex(imm, 0, 0) + "\n";
        }
        else if (w == 1)
        {
            if (opSize)
            {

                int16_t imm = common.assemble_bits(2, instruction, registers);
                memoryAccesses.push_back("mov " +st + ", $" + common.getHex(imm, 0, 0));

                memories16bit[st] = imm;
                memoryAccesses.push_back("write " + to_string(imm) + " to " + st);

                dispWithoutSIB =  st + ", $" + common.getHex(imm, 0, 0) +"\n";

            }
            else
            {
                int imm = common.assemble_bits(4, instruction, registers);
                memoryAccesses.push_back("mov " + st + ", $" + common.getHex(imm, 0, 0));

                memories32bit[st] = imm;
                memoryAccesses.push_back("write " + to_string(imm) + " to " + st);

                dispWithoutSIB =  st + ", $" + common.getHex(imm, 0, 0) +"\n";

            }
        }
    }
    else
    {
        if (w == 0)
        {
            int8_t imm = common.assemble_bits(1, instruction, registers);
            memoryAccesses.push_back("mov " + st + "(%" + regs_32[rm] + ")"+", $" + common.getHex(imm, 0, 0));

            memories8bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)] = imm;
            memoryAccesses.push_back("write " + to_string(imm) + " to " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

            dispWithoutSIB =   st + "(%" + regs_32[rm] + ")"+", $" + common.getHex(imm, 0, 0) +" \n";
        }
        else if (w == 1)
        {
            if (opSize)
            {

                int16_t imm = common.assemble_bits(2, instruction, registers);
                memoryAccesses.push_back("add " +  st + "(%" + regs_32[rm] + ") "+", $" + common.getHex(imm, 0, 0));

                memories16bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)] = imm;
                memoryAccesses.push_back("write " + to_string(imm) + " to " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

                dispWithoutSIB =    st + "(%" + regs_32[rm] + ") "+", $" + common.getHex(imm, 0, 0)+"\n";
            }
            else
            {
                int32_t imm = common.assemble_bits(4, instruction, registers);
                int num1 = memories32bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)];

                memories32bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)] = imm;
                memoryAccesses.push_back("write " + to_string(imm) + " to " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

                dispWithoutSIB =  st + "(%" + regs_32[rm] + ")"+", $" + common.getHex(imm, 0, 0) + " \n";
            }
        }
    }
    return dispWithoutSIB;
};

string Mov_immediate::decode_SIB(int w, int mod)
{
    string stringSib = "";

    int scale = common.get_bits(7, 2, instruction.front());
    int index = common.get_bits(4, 3, instruction.front());
    int base = common.get_bits(1, 3, instruction.front());

    instruction.pop();
    registers["EIP"] = registers["EIP"] + 1;

    scale = pow(2, scale);

    stringSib = decode_displacement_with_SIB(w, mod, index, scale, base);
    return stringSib;
}

string Mov_immediate::decode_mod_00(int w, int rm)
{
    string string00 = "";
    if (rm == 4)
    {
        string00 = decode_SIB(w, 0);
    }
    else if (rm == 5)
    {
        string00 = decode_displacement_without_SIB(w, 0, 5);
    }
    else
    {
        if (w == 0)
        {
            int8_t imm = common.assemble_bits(1, instruction, registers);
            memoryAccesses.push_back("mov (%" + regs_32[rm] + "), $" + common.getHex(imm, 0, 0));
            string00 =   "(%" + regs_32[rm] + "), $" + common.getHex(imm, 0, 0)+"\n";

            memories8bit[common.getHex(registers[regs_32[rm]], 0, 0)] = imm;
            memoryAccesses.push_back("write " + to_string(imm) + " to " + common.getHex(registers[regs_32[rm]], 0, 0));
        }
        else if (w == 1)
        {
            if (opSize)
            {
                int16_t imm = common.assemble_bits(2, instruction, registers);
                memoryAccesses.push_back("mov (%" + regs_32[rm] + "), $" + common.getHex(imm, 0, 0));

                string00 =  "(%" + regs_32[rm] + "), $" + common.getHex(imm, 0, 0) +"\n";
                memories16bit[common.getHex(registers[regs_32[rm]], 0, 0)] = imm;
                memoryAccesses.push_back("write " + to_string(imm) + " to " + common.getHex(registers[regs_32[rm]], 0, 0));
            }
            else
            {
                int32_t imm = common.assemble_bits(4, instruction, registers);
                memoryAccesses.push_back("mov (%" + regs_32[rm] + "), $" + common.getHex(imm, 0, 0));

                string00 =  "(%" + regs_32[rm] + "), $" + common.getHex(imm, 0, 0) +"\n";
                memories32bit[common.getHex(registers[regs_32[rm]], 0, 0)] = imm;
                memoryAccesses.push_back("write " + to_string(imm) + " to " + common.getHex(registers[regs_32[rm]], 0, 0));

            }
        }
    }

    return string00;
}

string Mov_immediate::decode_mod_01(int w, int rm)
{
    string string01 = "";
    if (rm == 4)
    {
        string01 = decode_SIB(w, 1);
    }
    else
    {
        string01 = decode_displacement_without_SIB(w, 1, rm);
    }
    return string01;
}

string Mov_immediate::decode_mod_10(int w, int rm)
{
    string string10 = "";
    if (rm == 4)
    {
        string10 = decode_SIB(w, 2);
    }
    else
    {
        string10 = decode_displacement_without_SIB(w, 2, rm);
    }
    return string10;
}

string Mov_immediate::decode_mod_11(int w, int rm)
{
    string string11 = "";
    if (w == 0)
    {
        int8_t imm = common.assemble_bits(1, instruction, registers);

        if (rm < 4)
        {
            registers[regs_32[rm]] = ((registers[regs_32[rm]]) & 0xffffff00) | (imm & 0x000000ff);
        }
        else
        {
            registers[regs_32[rm]] = ((registers[regs_32[rm]]) & 0xffff00ff) | ((imm << 8) & 0x0000ff00);
        }

        string11 = "%" + regs_8[rm] + ",$" + common.getHex(imm, 0, 0)+"\n";
    }
    else if (w == 1)
    {
        if (opSize)
        {
            int16_t imm = common.assemble_bits(2, instruction, registers);
            registers[regs_32[rm]] = ((registers[regs_32[rm]]) & 0xffff0000) | (imm & 0x0000ffff);

            string11 = "%" + regs_16[rm] + ",$" + common.getHex(imm, 0, 0)+"\n";

        }
        else
        {
            int32_t imm = common.assemble_bits(4, instruction, registers);
            registers[regs_32[rm]] = imm;

            string11 = "%" + regs_32[rm] + ",$" + common.getHex(imm, 0, 0) + "\n";
        }
    }

    return string11;
}

string Mov_immediate::decode_imm(short prefixes[4], int w, int mod, int rm)
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
        decoded_bytes = decode_mod_00(w, rm);
    }
    else if (mod == 1)
    {
        decoded_bytes = decode_mod_01(w, rm);
    }
    else if (mod == 2)
    {
        decoded_bytes = decode_mod_10(w, rm);
    }
    else
    {
        decoded_bytes = decode_mod_11(w, rm);
    }

    return decoded_bytes;
}
