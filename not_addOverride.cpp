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

#include "not.h"

using namespace std;

Not_addOverride::Not_addOverride(Common com, queue<short> &instruction, map<string, int> &registers, map<string, int> &memories32bit, map<string, int16_t> &memories16bit, map<string, int8_t> &memories8bit, list<string> &memoryAccesses)
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

string Not_addOverride::decode_displacement_without_SIB(int w, int mod, int rm)
{
    string dispWithoutSIB = "";
    int disp_bytes[] = {2, 1, 2};
    int bytes = disp_bytes[mod];

    int disp = common.assemble_bits(bytes, instruction, registers);

    if (mod == 0)
    {
        if (w == 0)
        {
                memoryAccesses.push_back("not " + common.getHex((unsigned int)disp, 0, 0) );

                int8_t  num2, num3;

               
                num2 = memories8bit[common.getHex((unsigned int)disp, 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex((unsigned int)disp, 0, 0));

                num3 =num2 xor 0xff; 

                memories8bit[common.getHex((unsigned int)disp, 0, 0)] = num3;
                memoryAccesses.push_back("Write " + to_string(num3) + " to " + common.getHex((unsigned int)disp, 0, 0));

                

                dispWithoutSIB = common.getHex((unsigned int)disp, 0, 0) +  "\n";
            
        }
        else if (w == 1)
        {
            if (opSize)
            {
                    memoryAccesses.push_back("not " + common.getHex((unsigned int)disp, 0, 0) );

                    int16_t num2, num3;
                    

                    num2 = memories16bit[common.getHex((unsigned int)disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex((unsigned int)disp, 0, 0));

                    num3 = num2 xor 0xffff;

                    memories16bit[common.getHex((unsigned int)disp, 0, 0)] = num3;
                    memoryAccesses.push_back("Write " + to_string(num3) + " to " + common.getHex((unsigned int)disp, 0, 0));


                    dispWithoutSIB = common.getHex((unsigned int)disp, 0, 0) + "\n";
                
            }
            else
            {
                    memoryAccesses.push_back("not " + common.getHex((unsigned int)disp, 0, 0) );

                    int num1 = memories32bit[common.getHex((unsigned int)disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex((unsigned int)disp, 0, 0));

                    
                    int num3 = num1 xor 0xffffffff;


                    memories32bit[common.getHex((unsigned int)disp, 0, 0)] = num3;
                    memoryAccesses.push_back("Write " + to_string(num3) + " to " + common.getHex((unsigned int)disp, 0, 0));

                    dispWithoutSIB = common.getHex((unsigned int)disp, 0, 0) +"\n";
                
            }
        }
    }
    else
    {
        if (w == 0)
        {
                memoryAccesses.push_back("not " + common.getHex((unsigned int)disp, 0, 0) + "(%" + regs_16bitmode[rm] + ")" );

                int8_t num2, num3;

             
                if (rm < 4)
                {
                    num2 = memories8bit[common.getHex((uint16_t)(*list1[rm / 2] + *list2[rm % 2] + disp), 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex((uint16_t)(*list1[rm / 2] + *list2[rm % 2] + disp), 0, 0));

                    num3 =  num2 xor 0xff;

                    memories8bit[common.getHex((uint16_t)(*list1[rm / 2] + *list2[rm % 2] + disp), 0, 0)] = num3;
                    memoryAccesses.push_back("Write " + to_string(num3) + " to " + common.getHex((uint16_t)(*list1[rm / 2] + *list2[rm % 2] + disp), 0, 0));
                }
                else
                {
                    num2 = memories8bit[common.getHex((uint16_t)(*list3[rm % 4] + disp), 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex((uint16_t)(*list3[rm % 4] + disp), 0, 0));

                    num3 =  num2 xor 0xff;

                    memories8bit[common.getHex((uint16_t)(*list3[rm % 4] + disp), 0, 0)] = num3;
                    memoryAccesses.push_back("Write " + to_string(num3) + " to " + common.getHex((uint16_t)(*list3[rm % 4] + disp), 0, 0));
                }


                dispWithoutSIB = common.getHex((unsigned int)disp, 0, 0) + "(%" + regs_16bitmode[rm] + ")" +"\n";
            
        }
        else if (w == 1)
        {
            if (opSize)
            {
                    memoryAccesses.push_back("not " + common.getHex((unsigned int)disp, 0, 0) + "(%" + regs_16bitmode[rm] + ")");

                    int16_t  num2, num3;


                    if (rm < 4)
                    {
                        num2 = memories16bit[common.getHex((uint16_t)(*list1[rm / 2] + *list2[rm % 2] + disp), 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex((uint16_t)(*list1[rm / 2] + *list2[rm % 2] + disp), 0, 0));

                        num3 =  num2 xor 0xffff;

                        memories16bit[common.getHex((uint16_t)(*list1[rm / 2] + *list2[rm % 2] + disp), 0, 0)] = num3;
                        memoryAccesses.push_back("Write " + to_string(num3) + " to " + common.getHex((uint16_t)(*list1[rm / 2] + *list2[rm % 2] + disp), 0, 0));
                    }
                    else
                    {
                        num2 = memories16bit[common.getHex((uint16_t)(*list3[rm % 4] + disp), 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex((uint16_t)(*list3[rm % 4] + disp), 0, 0));

                        num3 =num2 xor 0xffff; 

                        memories16bit[common.getHex((uint16_t)(*list3[rm % 4] + disp), 0, 0)] = num3;
                        memoryAccesses.push_back("Write " + to_string(num3) + " to " + common.getHex((uint16_t)(*list3[rm % 4] + disp), 0, 0));
                    }


                    dispWithoutSIB = common.getHex((unsigned int)disp, 0, 0) + "(%" + regs_16bitmode[rm] + ")" +  "\n";
                
            }
            else
            {
                    memoryAccesses.push_back("not " + common.getHex((unsigned int)disp, 0, 0) + "(%" + regs_16bitmode[rm] + ")" );

                    int num1, num3;

                    if (rm < 4)
                    {
                        num1 = memories32bit[common.getHex((uint16_t)(*list1[rm / 2] + *list2[rm % 2] + disp), 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex((uint16_t)(*list1[rm / 2] + *list2[rm % 2] + disp), 0, 0));

                        
                        num3 = num1 xor 0xffffffff;

                        memories32bit[common.getHex((uint16_t)(*list1[rm / 2] + *list2[rm % 2] + disp), 0, 0)] = num3;
                        memoryAccesses.push_back("Write " + to_string(num3) + " to " + common.getHex((uint16_t)(*list1[rm / 2] + *list2[rm % 2] + disp), 0, 0));
                    }
                    else
                    {
                        num1 = memories32bit[common.getHex((uint16_t)(*list3[rm % 4] + disp), 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex((uint16_t)(*list3[rm % 4] + disp), 0, 0));

                        
                        num3 = num1 xor 0xffffffff;

                        memories32bit[common.getHex((uint16_t)(*list3[rm % 4] + disp), 0, 0)] = num3;
                        memoryAccesses.push_back("Write " + to_string(num3) + " to " + common.getHex((uint16_t)(*list3[rm % 4] + disp), 0, 0));
                    }

                    

                    dispWithoutSIB = common.getHex((unsigned int)disp, 0, 0) + "(%" + regs_16bitmode[rm] + ")" +  "\n";
                
            }
        }
    }

    return dispWithoutSIB;
};

string Not_addOverride::decode_mod_00(int w, int rm)
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
                memoryAccesses.push_back("not (%" + regs_16bitmode[rm] + ")");
                string00 = "(%" + regs_16bitmode[rm] + ")" +  "\n";

                int8_t  num2, num3;

                

                if (rm < 4)
                {
                    num2 = memories8bit[common.getHex((uint16_t)(*list1[rm / 2] + *list2[rm % 2]), 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex((uint16_t)(*list1[rm / 2] + *list2[rm % 2]), 0, 0));

                    num3 = num2 xor 0xff;

                    memories8bit[common.getHex((uint16_t)(*list1[rm / 2] + *list2[rm % 2]), 0, 0)] = num3;
                    memoryAccesses.push_back("Write " + to_string(num3) + " to " + common.getHex((uint16_t)(*list1[rm / 2] + *list2[rm % 2]), 0, 0));
                }
                else
                {
                    num2 = memories8bit[common.getHex((uint16_t)*list3[rm % 4], 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex((uint16_t)*list3[rm % 4], 0, 0));

                    num3 =  num2 xor 0xff;

                    memories8bit[common.getHex((uint16_t)*list3[rm % 4], 0, 0)] = num3;
                    memoryAccesses.push_back("Write " + to_string(num3) + " to " + common.getHex((uint16_t)*list3[rm % 4], 0, 0));
                }

            
        }
        else if (w == 1)
        {
            if (opSize)
            {
                    memoryAccesses.push_back("not (%" + regs_16bitmode[rm] + ")");
                    string00 = "(%" + regs_16bitmode[rm] + ")" +  "\n";

                    int16_t num2, num3;


                    if (rm < 4)
                    {
                        num2 = memories16bit[common.getHex((uint16_t)(*list1[rm / 2] + *list2[rm % 2]), 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex((uint16_t)(*list1[rm / 2] + *list2[rm % 2]), 0, 0));

                        num3 =  num2 xor 0xffff;

                        memories16bit[common.getHex((uint16_t)(*list1[rm / 2] + *list2[rm % 2]), 0, 0)] = num3;
                        memoryAccesses.push_back("Write " + to_string(num3) + " to " + common.getHex((uint16_t)(*list1[rm / 2] + *list2[rm % 2]), 0, 0));
                    }
                    else
                    {
                        num2 = memories16bit[common.getHex((uint16_t)*list3[rm % 4], 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex((uint16_t)*list3[rm % 4], 0, 0));

                        num3 = num2 xor 0xffff;

                        memories16bit[common.getHex((uint16_t)*list3[rm % 4], 0, 0)] = num3;
                        memoryAccesses.push_back("Write " + to_string(num3) + " to " + common.getHex((uint16_t)*list3[rm % 4], 0, 0));
                    }

                
            }
            else
            {
                    memoryAccesses.push_back("not (%" + regs_16bitmode[rm] + ")");

                    int num1, num3;
                    if (rm < 4)
                    {

                        num1 = memories32bit[common.getHex((uint16_t)(*list1[rm / 2] + *list2[rm % 2]), 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex((uint16_t)(*list1[rm / 2] + *list2[rm % 2]), 0, 0));

                        
                        num3 = num1 xor 0xffffffff;

                        memories32bit[common.getHex((uint16_t)(*list1[rm / 2] + *list2[rm % 2]), 0, 0)] = num3;
                        memoryAccesses.push_back("Write " + to_string(num3) + " to " + common.getHex((uint16_t)(*list1[rm / 2] + *list2[rm % 2]), 0, 0));
                    }
                    else
                    {
                        num1 = memories32bit[common.getHex((uint16_t)*list3[rm % 4], 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex((uint16_t)*list3[rm % 4], 0, 0));

                       
                        num3 = num1 xor 0xffffffff;

                        memories32bit[common.getHex((uint16_t)*list3[rm % 4], 0, 0)] = num3;
                        memoryAccesses.push_back("Write " + to_string(num3) + " to " + common.getHex((uint16_t)*list3[rm % 4], 0, 0));
                    }


                    string00 = "(%" + regs_16bitmode[rm] + ")" +  "\n";
               
            }
        }
    }

    return string00;
}

string Not_addOverride::decode_mod_01(int w, int rm)
{
    string string01 = decode_displacement_without_SIB(w, 1, rm);
    return string01;
}

string Not_addOverride::decode_mod_10(int w, int rm)
{
    string string10 = decode_displacement_without_SIB(w, 2, rm);
    return string10;
}

string Not_addOverride::decode_mod_11(int w, int rm)
{
    string string11 = "";
    if (w == 0)
    { // printf("w:0 and d:0 \n");

        int8_t num2, num3;

        if (rm < 4)
        {
            num2 = common.get_bits(1, 8, registers[regs_32[rm]]);
            num3 = num2 xor 0xff;
            registers[regs_32[rm]] = ((registers[regs_32[rm]]) & 0xffffff00) | (num3 & 0x000000ff);
        }
        else
        {
            num2 = common.get_bits(9, 8, registers[regs_32[rm % 4]]);
            num3 = num2 xor 0xff;
            registers[regs_32[rm]] = ((registers[regs_32[rm]]) & 0xffff00ff) | ((num3 << 8) & 0x0000ff00);
        }

        string11 = "%" + regs_8[rm] + "\n";
    }
    else if (w == 1)
    {
        if (opSize)
        {
            int16_t num2, num3;

            num2 = common.get_bits(1, 16, registers[regs_32[rm]]);
            num3 = num2 xor 0xffff;
            registers[regs_32[rm]] = ((registers[regs_32[rm]]) & 0xffff0000) | (num3 & 0x0000ffff);

            string11 = "%" + regs_16[rm] + "\n";
        }
        else
        {
            int num2 = registers[regs_32[rm]];
            int num3 = num2 xor 0xffffffff;
            registers[regs_32[rm]] = num3;

            string11 = "%" + regs_32[rm] + "\n";
        }
    }

    return string11;
}

string Not_addOverride::decode_not(short prefixes[4])
{
    printf("Not when address override is present\n");

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