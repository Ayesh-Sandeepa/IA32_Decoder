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

#include "divider.h"

using namespace std;

Divider_addOverride::Divider_addOverride(Common com, queue<short> &instruction, map<string, int> &registers, map<string, int> &memories32bit, map<string, int16_t> &memories16bit, map<string, int8_t> &memories8bit, list<string> &memoryAccesses)
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

string Divider_addOverride::decode_displacement_without_SIB(int w, int mod, int rm)
{
    string dispWithoutSIB = "";
    int disp_bytes[] = {2, 1, 2};
    int bytes = disp_bytes[mod];

    int disp = common.assemble_bits(bytes, instruction, registers);

    if (mod == 0)
    {
        if (w == 0)
        {
            int16_t num1, num2, num3;
            uint8_t num4, num5;

            num1 = common.get_bits(1, 16, registers["EAX"]);
            num2 = memories8bit[common.getHex(disp, 0, 0)];
            memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(disp, 0, 0));

            num3 = num1 / num2;
            num4 = common.get_bits(1, 8, num3);
            num5 = num1%num2;

            registers["EAX"] = ((registers["EAX"]) & 0xffffff00) | (num4 & 0x000000ff);
            registers["EAX"] = ((registers["EAX"]) & 0xffff00ff) | ((num5<<8) & 0x0000ff00);
            

            dispWithoutSIB = common.getHex(disp, 0, 0) + "\n";
        }
        else if (w == 1)
        {
            if (opSize)
            {
                uint16_t temp1,temp2;
                temp1=common.get_bits(1, 16, registers["EAX"]);
                temp2=common.get_bits(1, 16, registers["EDX"]);
                int num1, num2, num3;
                uint16_t num4, num5;

                num1 = (temp2<<16)|temp1;
                num2 = memories16bit[common.getHex(disp, 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(disp, 0, 0));

                num3 = num1 / num2;
                num4 = common.get_bits(1, 16, num3);
                num5 = num1%num2;

                registers["EAX"] = ((registers["EAX"]) & 0xffff0000) | (num4 & 0x0000ffff);
                registers["EDX"] = ((registers["EDX"]) & 0xffff0000) | (num5 & 0x0000ffff);

                

                dispWithoutSIB = common.getHex(disp, 0, 0) + "\n";
            }
            else
            {
                unsigned int temp1,temp2;
                temp1=registers["EAX"];
                temp2=registers["EDX"];
                long num1 = (unsigned int)memories32bit[common.getHex(disp, 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(disp, 0, 0));

                long num2 = (temp2<<32)|temp1;
                long num3 = num2 / num1;
                unsigned int num4 = common.get_bits(1, 32, num3);
                unsigned int num5 = num2%num1;

                
                registers["EAX"] = num4;
                registers["EDX"] = num5;

                dispWithoutSIB = common.getHex(disp, 0, 0) + "\n";
            }
        }
    }
    else
    {
        if (w == 0)
        {
            int16_t num1, num2, num3;
            uint8_t num4, num5;

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

            num1 = common.get_bits(1, 16, registers["EAX"]);
            num3 = num1 / num2;
            num4 = common.get_bits(1, 8, num3);
            num4 = num1%num2;

            registers["EAX"] = ((registers["EAX"]) & 0xffffff00) | (num4 & 0x000000ff);
            registers["EAX"] = ((registers["EAX"]) & 0xffff00ff) | ((num5<<8) & 0x0000ff00);
            

            dispWithoutSIB = common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + ") \n";
        }
        else if (w == 1)
        {
            if (opSize)
            {
                uint16_t temp1,temp2;
                temp1=common.get_bits(1, 16, registers["EAX"]);
                temp2=common.get_bits(1, 16, registers["EDX"]);

                int num1, num2, num3;
                uint16_t num4, num5;

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

                num1 = (temp2<<16)|temp1;
                num3 = num1 / num2;
                num4 = common.get_bits(1, 16, num3);
                num5 = num1%num2;

                registers["EAX"] = ((registers["EAX"]) & 0xffff0000) | (num4 & 0x0000ffff);
                registers["EDX"] = ((registers["EDX"]) & 0xffff0000) | (num5 & 0x0000ffff);

                

                dispWithoutSIB = common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + ") \n";
            }
            else
            {
                unsigned int temp1,temp2;
                temp1=registers["EAX"];
                temp2=registers["EDX"];
                long num1, num2, num3;
                unsigned int num4, num5;
                if (rm < 4)
                {
                    num1 = (unsigned int)memories32bit[common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(*list1[rm / 2] + *list2[rm % 2] + disp, 0, 0));
                }
                else
                {
                    num1 = (unsigned int)memories32bit[common.getHex(*list3[rm % 4] + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(*list3[rm % 4] + disp, 0, 0));
                }
                num2 = (temp2<<32)|temp1;
                num3 = num2/ num1;
                num4 = common.get_bits(1, 32, num3);
                num5 = num2%num1;

                

                registers["EAX"] = num4;
                registers["EDX"] = num5;

                dispWithoutSIB = common.getHex(disp, 0, 0) + "(%" + regs_16bitmode[rm] + ") \n";
            }
        }
    }

    return dispWithoutSIB;
};

string Divider_addOverride::decode_mod_00(int w, int rm)
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
            string00 = "(%" + regs_16bitmode[rm] + ")\n";

            int16_t num1, num2, num3;
            uint8_t num4, num5;

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

            num1 = common.get_bits(1, 16, registers["EAX"]);
            num3 = num1 / num2;
            num4 = common.get_bits(1, 8, num3);
            num5 = num1%num2;

            registers["EAX"] = ((registers["EAX"]) & 0xffffff00) | (num4 & 0x000000ff);
            registers["EAX"] = ((registers["EAX"]) & 0xffff00ff) | ((num5<<8) & 0x0000ff00); 

            
        }
        else if (w == 1)
        {
            if (opSize)
            {
                uint16_t temp1,temp2;
                temp1=common.get_bits(1, 16, registers["EAX"]);
                temp2=common.get_bits(1, 16, registers["EDX"]);

                string00 = "(%" + regs_16bitmode[rm] + ")\n";

                int num1, num2, num3;
                uint16_t num4, num5;

                num1 = (temp2<<16)|temp1;

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

                num3 = num1 / num2;
                num4 = common.get_bits(1, 16, num3);
                num5 = num1%num2;

                registers["EAX"] = ((registers["EAX"]) & 0xffff0000) | (num4 & 0x0000ffff);
                registers["EDX"] = ((registers["EDX"]) & 0xffff0000) | (num5 & 0x0000ffff);

            }
            else
            {
                unsigned int temp1,temp2;
                temp1=registers["EAX"];
                temp2=registers["EDX"];

                long num1, num2, num3;
                unsigned int num4, num5;

                if (rm < 4)
                {
                    num1 = (unsigned int)memories32bit[common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(*list1[rm / 2] + *list2[rm % 2], 0, 0));
                }
                else
                {
                    num1 = (unsigned int)memories32bit[common.getHex(*list3[rm % 4], 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(*list3[rm % 4], 0, 0));
                }

                num2 = (temp2<<32)|temp1;
                num3 = num2 / num1;
                num4 = common.get_bits(1, 32, num3);
                num5 = num2%num1;

                string00 = "(%" + regs_16bitmode[rm] + ")\n";
                registers["EAX"] = num4;
                registers["EDX"] = num5;
            }
        }
    }

    return string00;
}

string Divider_addOverride::decode_mod_01(int w, int rm)
{
    string string01 = decode_displacement_without_SIB(w, 1, rm);
    return string01;
}

string Divider_addOverride::decode_mod_10(int w, int rm)
{
    string string10 = decode_displacement_without_SIB(w, 2, rm);
    return string10;
}

string Divider_addOverride::decode_mod_11(int w, int rm)
{
    string string11 = "";
    if (w == 0)
    {
        int16_t num1, num2;
        int16_t num3;
        uint8_t num4, num5;

        num1 = common.get_bits(1, 16, registers["EAX"]);

        if (rm < 4)
        {
            num2 = common.get_bits(1, 8, registers[regs_32[rm]]);
        }
        else
        {
            num2 = common.get_bits(9, 8, registers[regs_32[rm % 4]]);
        }

        num3 = num1 / num2;
        num4 = common.get_bits(1, 8, num3);
        num5 = num1 % num2;
        registers["EAX"] = ((registers["EAX"]) & 0xffffff00) | (num4 & 0x000000ff);
        registers["EAX"] = ((registers["EAX"]) & 0xffffff00) | ((num5 << 8) & 0x0000ff00);

        string11 = "%" + regs_8[rm] + "\n";
    }
    else if (w == 1)
    {
        if (opSize)
        {
            uint16_t temp1, temp2;
            int num1, num2;
            int num3;
            uint16_t num4, num5;

            temp1 = common.get_bits(1, 16, registers["EAX"]);
            temp2 = common.get_bits(1, 16, registers["EDX"]);
            num1 = (temp2 << 16) | temp1;
            num2 = common.get_bits(1, 16, registers[regs_32[rm]]);
            num3 = num1 / num2;
            num4 = common.get_bits(1, 16, num3);
            num5 = num1 % num2;
            registers["EAX"] = ((registers["EAX"]) & 0xffff0000) | (num4 & 0x0000ffff);
            registers["EDX"] = ((registers["EDX"]) & 0xffff0000) | (num5 & 0x0000ffff);

            string11 = "%" + regs_16[rm] + "\n";
        }
        else
        {
            unsigned int temp1, temp2;
            temp1 = registers["EAX"];
            temp2 = registers["EDX"];

            long num1 = (temp2 << 32) | temp1;
            long num2 = (unsigned int)registers[regs_32[rm]];

            // cout << "EAX:" << common.getHex(num1,0,0) << " ; " << regs_32[rm] <<":"<<common.getHex((unsigned long int)num2,0,0)<<"\n";

            long int num3 = num1 / num2;
            // cout << "out:" << common.getHex((unsigned long int)num3,0,0)<<"\n";
            // cout << sizeof(int) <<":"<<sizeof(long int)<<"\n";

            unsigned int num4 = common.get_bits(1, 32, num3);
            unsigned int num5 = num1 % num2;

            // cout << "num4:"<<common.getHex(num4,0,0)<<" ; num5:"<<common.getHex(num5,0,0)<<"\n";
            registers["EAX"] = num4;
            registers["EDX"] = num5;

            string11 = "%" + regs_32[rm] + "\n";
        }
    }
    return string11;

    return string11;
}

string Divider_addOverride::decode_div(short prefixes[4])
{
    printf("Divider when address override is present\n");

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
    int rm = common.get_bits(1, 3, modrm);

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

    return decoded_bytes;
}

