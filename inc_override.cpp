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

#include "inc_dec.h"

using namespace std;

Inc_override::Inc_override(Common com, queue<short> &instruction, map<string, int> &registers, map<string, int> &memories32bit, map<string, int16_t> &memories16bit, map<string, int8_t> &memories8bit, list<string> &memoryAccesses)
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

string Inc_override::decode_displacement_without_SIB(int w, int mod, int rm)
{
    string dispWithoutSIB = "";
    int disp_bytes[] = {2, 1, 2};
    int bytes = disp_bytes[mod];

    int disp = common.assemble_bits(bytes, instruction, registers);

    if (mod == 0)
    {
        if (w == 0)
        {
            dispWithoutSIB =common.getHex(disp, 0, 0) + "\n";
            memoryAccesses.push_back("inc " + common.getHex(disp, 0, 0));

            int8_t num1, num2;
            num1 = memories8bit[common.getHex(disp, 0, 0)];
            memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(disp, 0, 0));
            num2 = num1 +1;
            memories8bit[common.getHex(disp, 0, 0)] = num2;
            memoryAccesses.push_back("Write " + to_string(num2) + " to " + common.getHex(disp, 0, 0));

            common.setOverflow32bit(num1, 1 , num2, registers);
            common.setSign(num2, registers);
            common.setZero(num2, registers);
        }
        else if (w == 1)
        {
            if (opSize)
            {
                memoryAccesses.push_back("inc " + common.getHex(disp, 0, 0));

                int16_t num1, num2;
                num1 = memories16bit[common.getHex(disp, 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(disp, 0, 0));
                num2 = num1 +1;
                memories16bit[common.getHex(disp, 0, 0)] = num2;
                memoryAccesses.push_back("Write " + to_string(num2) + " to " + common.getHex(disp, 0, 0));

                dispWithoutSIB = common.getHex(disp, 0, 0) + "\n";

                common.setOverflow32bit(num1, 1 , num2, registers);
                common.setSign(num2, registers);
                common.setZero(num2, registers);
            }
            else
            {
                memoryAccesses.push_back("inc " + common.getHex(disp, 0, 0) );

                int num1 = memories32bit[common.getHex(disp, 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(disp, 0, 0));
                int num2 = num1 +1;
                memories32bit[common.getHex(disp, 0, 0)] = num2;
                memoryAccesses.push_back("Write " + to_string(num2) + " to " + common.getHex(disp, 0, 0));

                dispWithoutSIB = common.getHex(disp, 0, 0) + ",\n";

                common.setOverflow32bit(num1, 1 , num2, registers);
                common.setSign(num2, registers);
                common.setZero(num2, registers);
            }
        }
    }
    else
    {
        if (w == 0)
        {
            memoryAccesses.push_back("inc " +common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + ")");
            int8_t num1, num2;

            if (rm < 4)
            {
                num1 = memories8bit[common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0));
                num2= num1 +1;
                memories8bit[common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0)] = num2;
                memoryAccesses.push_back("Write " + to_string(num2) + " to " + common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0));
            }
            else
            {
                num1 = memories8bit[common.getHex(*list3[rm % 4] + disp, 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(*list3[rm % 4] + disp, 0, 0));
                num2 = num1 +1;
                memories8bit[common.getHex(*list3[rm % 4] + disp, 0, 0)] = num2;
                memoryAccesses.push_back("Write " + to_string(num2) + " to " + common.getHex(*list3[rm % 4] + disp, 0, 0));
            }
            dispWithoutSIB = common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + ")\n";

            common.setOverflow32bit(num1, 1 , num2, registers);
            common.setSign(num2, registers);
            common.setZero(num2, registers);
        }
        else if (w == 1)
        {
            if (opSize)
            {
                memoryAccesses.push_back("inc " + common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + ")");

                int16_t num1, num2;

                if (rm < 4)
                {
                    num1 = memories16bit[common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0));
                    num2 = num1+1;
                    memories16bit[common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0)] = num2;
                    memoryAccesses.push_back("Write " + to_string(num2) + " to " + common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0));
                }
                else
                {
                    num1 = memories16bit[common.getHex(*list3[rm % 4] + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(*list3[rm % 4] + disp, 0, 0));
                    num2 = num1 +1;
                    memories16bit[common.getHex(*list3[rm % 4] + disp, 0, 0)] = num2;
                    memoryAccesses.push_back("Write " + to_string(num2) + " to " + common.getHex(*list3[rm % 4] + disp, 0, 0));
                }

                dispWithoutSIB = common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + ")\n";

                common.setOverflow32bit(num1, 1 , num2, registers);
                common.setSign(num2, registers);
                common.setZero(num2, registers);
            }
            else
            {
                memoryAccesses.push_back("inc " + common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + ")");

                int num1, num2;

                if (rm < 4)
                {
                    num1 = memories32bit[common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0));
                    num2 = num1+ 1;

                    memories32bit[common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0)] = num2;
                    memoryAccesses.push_back("Write " + to_string(num2) + " to " + common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0));
                }
                else
                {
                    num1 = memories32bit[common.getHex(*list3[rm % 4] + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(*list3[rm % 4] + disp, 0, 0));
                    num2 = num1 +1;
                    memories32bit[common.getHex(*list3[rm % 4] + disp, 0, 0)] = num2;
                    memoryAccesses.push_back("Write " + to_string(num2) + " to " + common.getHex(*list3[rm % 4] + disp, 0, 0));
                }

                dispWithoutSIB = common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + ")\n";
                
                common.setOverflow32bit(num1, 1 , num2, registers);
                common.setSign(num2, registers);
                common.setZero(num2, registers);
            }
        }
    }
    return dispWithoutSIB;
};

string Inc_override::decode_mod_00(int w, int rm)
{
    string string00 = "";
    if (rm == 6)
    {
        string00 = decode_displacement_without_SIB(w, 0, 5);
    }
    else
    {
        if (w == 0)
        {
            memoryAccesses.push_back("inc (%" + regs_16bitmode[rm] + "),%" );
            string00 = "(%" + regs_16bitmode[rm] + ")\n";

            int8_t num1, num2;

            if (rm < 4)
            {
                num1 =  memories8bit[common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num1) + " form " + common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0));
                num2 = num1 + 1;
                memories8bit[common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0)] = num2;
                memoryAccesses.push_back("Write " + to_string(num2) + " to " + common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0));
            }
            else
            {
                num1 =   memories8bit[common.getHex(*list3[rm % 4], 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(*list3[rm % 4], 0, 0));
                num2 = num1 + 1;
                memories8bit[common.getHex(*list3[rm % 4], 0, 0)] = num2;
                memoryAccesses.push_back("Write " + to_string(num2) + " to " + common.getHex(*list3[rm % 4], 0, 0));
            }
            common.setOverflow32bit(num1, 1 , num2, registers);
            common.setSign(num2, registers);
            common.setZero(num2, registers);
        }
        else if (w == 1)
        {
            if (opSize)
            {
                memoryAccesses.push_back("inc (%" + regs_16bitmode[rm] + ")");
                string00 = "(%" + regs_16bitmode[rm] + ")\n";

                int16_t num1, num2;

                if (rm < 4)
                {
                    num1 = memories16bit[common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0));
                    num2 = num1 + 1;
                    memories16bit[common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0)] = num2;
                    memoryAccesses.push_back("Write " + to_string(num2) + " to " + common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0));
                }
                else
                {
                    num1 = memories16bit[common.getHex(*list3[rm % 4], 0, 0)];
                    num2 = num1 + 1;
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(*list3[rm % 4], 0, 0));
                    memories16bit[common.getHex(*list3[rm % 4], 0, 0)] = num2;
                    memoryAccesses.push_back("Write " + to_string(num2) + " to " + common.getHex(*list3[rm % 4], 0, 0));
                }
                common.setOverflow32bit(num1, 1 , num2, registers);
                common.setSign(num2, registers);
                common.setZero(num2, registers);
            }
            else
            {
                memoryAccesses.push_back("inc (%" + regs_16bitmode[rm] + ")");

                int num1, num2;
                if (rm < 4)
                {
                    num1 = memories32bit[common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0));
                    num2 = num1 +1;
                    memories32bit[common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0)] = num2;
                    memoryAccesses.push_back("Write " + to_string(num2) + " to " + common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0));
                }
                else
                {
                    num1 = memories32bit[common.getHex(*list3[rm % 4], 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(*list3[rm % 4], 0, 0));
                    num2 = num1 +1;
                    memories32bit[common.getHex(*list3[rm % 4], 0, 0)] = num2;
                    memoryAccesses.push_back("Write " + to_string(num2) + " to " + common.getHex(*list3[rm % 4], 0, 0));
                }

                string00 = "(%" + regs_16bitmode[rm] + "),\n";
                common.setOverflow32bit(num1, 1 , num2, registers);
                common.setSign(num2, registers);
                common.setZero(num2, registers);
            }
        }
    }
    return string00;
}

string Inc_override::decode_mod_01(int w, int rm)
{
    string string01 = decode_displacement_without_SIB(w, 1, rm);
    return string01;
}

string Inc_override::decode_mod_10(int w, int rm)
{
    string string10 = decode_displacement_without_SIB(w, 2, rm);
    return string10;
}

string Inc_override::decode_mod_11(int w, int rm)
{
    string string11 = "";
    int8_t num1, num2;
    if (w == 0)
    {
        int8_t num1, num2;
        if (rm < 4)
        {
            num1 = common.get_bits(1, 8, registers[regs_32[rm]]);
            num2 = num1 + 1;
            registers[regs_32[rm]] = ((registers[regs_32[rm]]) & 0xffffff00) | (num2 & 0x000000ff);
        }
        else
        {
            num1 = common.get_bits(9, 8, registers[regs_32[rm % 4]]);
            num2 = num1 + 1;
            registers[regs_32[rm]] = ((registers[regs_32[rm]]) & 0xffff00ff) | ((num2 << 8) & 0x0000ff00);
        }

        common.setOverflow8bit(num1, 1 , num2, registers);
        common.setSign(num2, registers);
        common.setZero(num2, registers);

        string11 = "%" + regs_8[rm] +  "\n";
    }
    else if (w == 1)
    {
        if (opSize)
        {
            int16_t num1, num2;
            num1 = common.get_bits(1, 16, registers[regs_32[rm]]);
            num2 = num1 + 1;;

            registers[regs_32[rm]] = ((registers[regs_32[rm]]) & 0xffff0000) | (num2 & 0x0000ffff);

            common.setOverflow16bit(num1, 1 , num2, registers);
            common.setSign(num2, registers);
            common.setZero(num2, registers);

            string11 = "%" + regs_16[rm] + "\n";
        }
        else
        {
            int32_t num1, num2;
            num1 = registers[regs_32[rm]];
            num2 = num1 + 1;
            registers[regs_32[rm]] = num1;

            common.setOverflow32bit(num1, 1 , num2, registers);
            common.setSign(num2, registers);
            common.setZero(num2, registers);

            string11 = "%" + regs_32[rm] + "\n";
        }
    }
    return string11;
}

string Inc_override::decode_inc(short prefixes[4])
{
    printf ("Inc when address override is present\n");

    short opCode = instruction.front();
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

    short modrm = instruction.front();
    int mod = modrm >> 6;
    int reg = common.get_bits(4, 3, modrm);
    int rm = common.get_bits(1, 3, modrm);

    if(reg == 1){
        Dec_override Dec_override(common, instruction, registers, memories32bit, memories16bit, memories8bit, memoryAccesses);
        decoded_bytes = Dec_override.decode_dec(prefixes, w);
    }
    else
    {
        instruction.pop();
        registers["EIP"] = registers["EIP"] + 1;
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
    }    
    return "inc " + decoded_bytes;
}