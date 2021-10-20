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

#include "mul.h"

using namespace std;

Multiplier::Multiplier(Common com, queue<short> &instruction, map<string, int> &registers, map<string, int> &memories32bit, map<string, int16_t> &memories16bit, map<string, int8_t> &memories8bit, list<string> &memoryAccesses)
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

string Multiplier::decode_displacement_with_SIB(int w, int mod, int index, int scale, int base)
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

                if (index == 4)
                {
                    int8_t num1, num2;
                    int16_t num3;
                    uint8_t num4, num5;

                    num1 = common.get_bits(1, 8, registers["EAX"]);
                    num2 = memories8bit[st];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + st);

                    num3 = num1 * num2;
                    num4 = common.get_bits(1, 8, num3);
                    num5 = common.get_bits(8, 8, num3);

                    registers["EAX"] = ((registers["EAX"]) & 0xffff0000) | (num3 & 0x0000ffff);

                    setOverflowCarry8(num5);

                    dispWithSIB = st + "\n";
                }
                else
                {
                    int8_t num1, num2;
                    int16_t num3;
                    uint8_t num4, num5;

                    num1 = common.get_bits(1, 8, registers["EAX"]);
                    num2 = memories8bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                    num3 = num1 * num2;
                    num4 = common.get_bits(1, 8, num3);
                    num5 = common.get_bits(8, 8, num3);

                    registers["EAX"] = ((registers["EAX"]) & 0xffff0000) | (num3 & 0x0000ffff);

                    setOverflowCarry8(num5);

                    dispWithSIB = st + "(,%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                }
            }
            else if (w == 1)
            {
                if (opSize)
                {
                    if (index == 4)
                    {
                        int16_t num1, num2;
                        int num3;
                        uint16_t num4, num5;

                        num1 = common.get_bits(1, 16, registers["EAX"]);
                        num2 = memories16bit[st];
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + st);

                        num3 = num1 * num2;
                        num4 = common.get_bits(1, 16, num3);
                        num5 = common.get_bits(16, 16, num3);

                        registers["EAX"] = ((registers["EAX"]) & 0xffff0000) | (num4 & 0x0000ffff);
                        registers["EDX"] = ((registers["EDX"]) & 0xffff0000) | (num5 & 0x0000ffff);

                        setOverflowCarry8(num5);

                        dispWithSIB = st + "\n";
                    }
                    else
                    {
                        int16_t num1, num2;
                        int num3;
                        uint16_t num4, num5;

                        num1 = common.get_bits(1, 16, registers["EAX"]);
                        num2 = memories16bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                        num3 = num1 * num2;
                        num4 = common.get_bits(1, 16, num3);
                        num5 = common.get_bits(16, 16, num3);

                        registers["EAX"] = ((registers["EAX"]) & 0xffff0000) | (num4 & 0x0000ffff);
                        registers["EDX"] = ((registers["EDX"]) & 0xffff0000) | (num5 & 0x0000ffff);

                        common.setOverflow16bit(num1, num2, num3, registers);
                        setOverflowCarry16(num5);

                        dispWithSIB = st + "(,%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                    }
                }
                else
                {
                    if (index == 4)
                    {
                        int num1 = memories32bit[st];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + st);

                        int num2 = registers["EAX"];
                        long int num3 = num1 * num2;
                        unsigned int num4 = common.get_bits(1, 32, num3);
                        unsigned int num5 = common.get_bits(32, 32, num3);

                        registers["EAX"] = num4;
                        registers["EDX"] = num5;

                        dispWithSIB = st + "\n";
                    }
                    else
                    {

                        int num1 = memories32bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                        int num2 = registers["EAX"];
                        long int num3 = num1 * num2;
                        unsigned int num4 = common.get_bits(1, 32, num3);
                        unsigned int num5 = common.get_bits(32, 32, num3);

                        registers["EAX"] = num4;
                        registers["EDX"] = num5;

                        dispWithSIB = st + "(,%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                    }
                }
            }
        }
        else
        {
            if (w == 0)
            {
                if (index == 4)
                {

                    int8_t num1, num2;
                    int16_t num3;
                    uint8_t num4, num5;

                    num1 = common.get_bits(1, 8, registers["EAX"]);
                    num2 = memories8bit[common.getHex(registers[regs_32[base]], 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[base]], 0, 0));

                    num3 = num1 * num2;
                    num4 = common.get_bits(1, 8, num3);
                    num5 = common.get_bits(8, 8, num3);

                    registers["EAX"] = ((registers["EAX"]) & 0xffff0000) | (num3 & 0x0000ffff);

                    setOverflowCarry8(num5);

                    dispWithSIB = "(%" + regs_32[base] + ")" + "\n";
                }
                else
                {
                    int8_t num1, num2;
                    int16_t num3;
                    uint8_t num4, num5;

                    num1 = common.get_bits(1, 8, registers["EAX"]);
                    num2 = memories8bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale, 0, 0));

                    num3 = num1 * num2;
                    num4 = common.get_bits(1, 8, num3);
                    num5 = common.get_bits(8, 8, num3);

                    registers["EAX"] = ((registers["EAX"]) & 0xffff0000) | (num3 & 0x0000ffff);

                    setOverflowCarry8(num5);

                    dispWithSIB = "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                }
            }
            else if (w == 1)
            {
                if (opSize)
                {
                    if (index == 4)
                    {

                        int16_t num1, num2;
                        int num3;
                        uint16_t num4, num5;

                        num1 = common.get_bits(1, 16, registers["EAX"]);
                        num2 = memories16bit[common.getHex(registers[regs_32[base]], 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[base]], 0, 0));

                        num3 = num1 * num2;
                        num4 = common.get_bits(1, 16, num3);
                        num5 = common.get_bits(16, 16, num3);

                        registers["EAX"] = ((registers["EAX"]) & 0xffff0000) | (num4 & 0x0000ffff);
                        registers["EDX"] = ((registers["EDX"]) & 0xffff0000) | (num5 & 0x0000ffff);

                        setOverflowCarry16(num5);

                        dispWithSIB = "(%" + regs_32[base] + ")" + "\n";
                    }
                    else
                    {

                        int16_t num1, num2;
                        int num3;
                        uint16_t num4, num5;

                        num1 = common.get_bits(1, 16, registers["EAX"]);
                        num2 = memories16bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                        num3 = num1 * num2;
                        num4 = common.get_bits(1, 16, num3);
                        num5 = common.get_bits(16, 16, num3);

                        registers["EAX"] = ((registers["EAX"]) & 0xffff0000) | (num4 & 0x0000ffff);
                        registers["EDX"] = ((registers["EDX"]) & 0xffff0000) | (num5 & 0x0000ffff);

                        setOverflowCarry16(num5);

                        dispWithSIB = "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                    }
                }
                else
                {
                    if (index == 4)
                    {
                        int num1 = memories32bit[common.getHex(registers[regs_32[base]], 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]], 0, 0));

                        int num2 = registers["EAX"];
                        long int num3 = num1 * num2;
                        unsigned int num4 = common.get_bits(1, 32, num3);
                        unsigned int num5 = common.get_bits(32, 32, num3);

                        registers["EAX"] = num4;
                        registers["EDX"] = num5;

                        setOverflowCarry32(num5);

                        dispWithSIB = "(%" + regs_32[base] + ")" + "\n";
                    }
                    else
                    {
                        int num1 = memories32bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                        int num2 = registers["EAX"];
                        long long num3 = num1 * num2;
                        unsigned int num4 = common.get_bits(1, 32, num3);
                        unsigned int num5 = common.get_bits(32, 32, num3);

                        registers["EAX"] = num4;
                        registers["EDX"] = num5;

                        setOverflowCarry32(num5);

                        dispWithSIB = "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                    }
                }
            }
        }
        // printf("mod equals 0");
    }
    else
    {
        if (w == 0)
        {
            if (index == 4)
            {
                int8_t num1, num2;
                int16_t num3;
                uint8_t num4, num5;

                num1 = common.get_bits(1, 8, registers["EAX"]);
                num2 = memories8bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                num3 = num1 * num2;
                num4 = common.get_bits(1, 8, num3);
                num5 = common.get_bits(8, 8, num3);

                registers["EAX"] = ((registers["EAX"]) & 0xffff0000) | (num3 & 0x0000ffff);

                setOverflowCarry8(num5);

                dispWithSIB = st + "(%" + regs_32[base] + ")" + "\n";
            }
            else
            {
                int8_t num1, num2;
                int16_t num3;
                uint8_t num4, num5;

                num1 = common.get_bits(1, 8, registers["EAX"]);
                num2 = memories8bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                num3 = num1 * num2;
                num4 = common.get_bits(1, 8, num3);
                num5 = common.get_bits(8, 8, num3);

                registers["EAX"] = ((registers["EAX"]) & 0xffff0000) | (num3 & 0x0000ffff);

                setOverflowCarry8(num5);

                dispWithSIB = st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
            }
        }
        else if (w == 1)
        {
            if (opSize)
            {

                if (index == 4)
                {
                    int16_t num1, num2;
                    int num3;
                    uint16_t num4, num5;

                    num1 = common.get_bits(1, 16, registers["EAX"]);
                    num2 = memories16bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                    num3 = num1 * num2;
                    num4 = common.get_bits(1, 16, num3);
                    num5 = common.get_bits(16, 16, num3);

                    registers["EAX"] = ((registers["EAX"]) & 0xffff0000) | (num4 & 0x0000ffff);
                    registers["EDX"] = ((registers["EDX"]) & 0xffff0000) | (num5 & 0x0000ffff);

                    setOverflowCarry16(num5);

                    dispWithSIB = st + "(%" + regs_32[base] + ")" + "\n";
                }
                else
                {
                    int16_t num1, num2;
                    int num3;
                    uint16_t num4, num5;

                    num1 = common.get_bits(1, 16, registers["EAX"]);
                    num2 = memories16bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                    num3 = num1 * num2;
                    num4 = common.get_bits(1, 16, num3);
                    num5 = common.get_bits(16, 16, num3);

                    registers["EAX"] = ((registers["EAX"]) & 0xffff0000) | (num4 & 0x0000ffff);
                    registers["EDX"] = ((registers["EDX"]) & 0xffff0000) | (num5 & 0x0000ffff);

                    setOverflowCarry16(num5);

                    dispWithSIB = st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                }
            }
            else
            {
                if (index == 4)
                {
                    int num1 = memories32bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                    int num2 = registers["EAX"];
                    long int num3 = num1 * num2;
                    unsigned int num4 = common.get_bits(1, 32, num3);
                    unsigned int num5 = common.get_bits(32, 32, num3);

                    registers["EAX"] = num4;
                    registers["EDX"] = num5;

                    dispWithSIB = st + "(%" + regs_32[base] + ")" + "\n";
                }
                else
                {
                    int num1 = memories32bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                    int num2 = registers["EAX"];
                    long int num3 = num1 * num2;
                    unsigned int num4 = common.get_bits(1, 32, num3);
                    unsigned int num5 = common.get_bits(32, 32, num3);

                    registers["EAX"] = num4;
                    registers["EDX"] = num5;

                    dispWithSIB = st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                }
            }
        }
    }
    return dispWithSIB;
}

string Multiplier::decode_displacement_without_SIB(int w, int mod, int rm)
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
            int8_t num1, num2;
            int16_t num3;
            uint8_t num4,num5;

            
                num1 = common.get_bits(1, 8, registers["EAX"]);
                num2 = memories8bit[st];
                memoryAccesses.push_back("Read " + to_string(num2) + " from " + st);

                num3 = num1 * num2;
                num4 =common.get_bits(1,8,num3);
                num5 =common.get_bits(8,8,num3);

                registers["EAX"] = ((registers["EAX"]) & 0xffff0000) | (num3 & 0x0000ffff);
            

            setOverflowCarry8(num5);

            dispWithoutSIB =  st + "\n";
        }
        else if (w == 1)
        {
            if (opSize)
            {

                int16_t num1, num2;
                int num3;
                uint16_t num4,num5;

                num1 = common.get_bits(1, 16, registers["EAX"]);
                num2 = memories16bit[st];
                memoryAccesses.push_back("Read " + to_string(num2) + " from " + st);

                num3 = num1 * num2;
                num4 = common.get_bits(1,16,num3);
                num5 = common.get_bits(16,16,num3);

                registers["EAX"] = ((registers["EAX"]) & 0xffff0000) | (num4 & 0x0000ffff);
                registers["EDX"] = ((registers["EDX"]) & 0xffff0000) | (num5 & 0x0000ffff);

                setOverflowCarry16(num5);

                dispWithoutSIB = st + "\n";
            }
            else
            {
                int num1 = memories32bit[st];
                memoryAccesses.push_back("Read " + to_string(num1) + " from " + st);

                int num2 = registers["EAX"];
                long int num3 = num1 * num2;
                unsigned int num4 =common.get_bits(1,32,num3);
                unsigned int num5 =common.get_bits(32,32,num3);

                registers["EAX"] = num4;
                 registers["EDX"] = num5;

                 setOverflowCarry32(num5);

                dispWithoutSIB = st + "\n";
            }
        }
    }
    else
    {
        if (w == 0)
        {
            int8_t num1, num2;
            int16_t num3;
            uint8_t num4,num5;

            
                num1 = common.get_bits(1, 8, registers["EAX"]);
                num2 = memories8bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

                num3 = num1 * num2;
                num4 = common.get_bits(1,8,num3);
                num5=common.get_bits(8,8,num3);

                registers["EAX"] = ((registers["EAX"]) & 0xffff0000) | (num3 & 0x0000ffff);
            

            setOverflowCarry8(num5);
            dispWithoutSIB =  st + "(%" + regs_32[rm] + ") \n";
        }
        else if (w == 1)
        {
            if (opSize)
            {
                int16_t num1, num2;
                int num3;
                uint16_t num4,num5;

                num1 = common.get_bits(1, 16, registers["EAX"]);
                num2 = memories16bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

                num3 = num1 * num2;
                num4 = common.get_bits(1,16,num3);
                num5 = common.get_bits(16,16,num3);

                registers["EDX"] = ((registers["EDX"]) & 0xffff0000) | (num5 & 0x0000ffff);

                registers["EAX"] = ((registers["EAX"]) & 0xffff0000) | (num4 & 0x0000ffff);

                setOverflowCarry16(num5);

                dispWithoutSIB = st + "(%" + regs_32[rm] + ") \n";
            }
            else
            {
                int num1 = memories32bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

                int num2 = registers["EAX"];
                long int num3 = num1 * num2;
                unsigned int num4 = common.get_bits(1,32,num3);
                unsigned int num5 = common.get_bits(32,32,num3);


                registers["EAX"] = num4;
                registers["EDX"]=num5;

                setOverflowCarry32(num5);

                dispWithoutSIB =  st + "(%" + regs_32[rm] + ") \n";
            }
        }
    }

    return dispWithoutSIB;
};

string Multiplier::decode_SIB(int w, int mod)
{
    string stringSib = "";

    int scale = common.get_bits(7, 2, instruction.front());
    int index = common.get_bits(4, 3, instruction.front());
    int base = common.get_bits(1, 3, instruction.front());

    instruction.pop();
    registers["EIP"] = registers["EIP"] + 1;

    scale = pow(2, scale);

    stringSib = decode_displacement_with_SIB(w, mod,  index, scale, base);
    return stringSib;
}

string Multiplier::decode_mod_00(int w, int rm)
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
            string00 = "(%" + regs_32[rm] + ")" + "\n";

            int8_t num1, num2;
            int16_t num3;
            uint8_t num4, num5;

            num1 = common.get_bits(1, 8, registers["EAX"]);

            num2 = memories8bit[common.getHex(registers[regs_32[rm]], 0, 0)];
            memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[rm]], 0, 0));

            num3 = num1 * num2;
            num4 = common.get_bits(1, 8, num3);
            num5 = common.get_bits(8, 8, num3);

            registers["EAX"] = registers["EAX"] = ((registers["EAX"]) & 0xffff0000) | (num3 & 0x0000ffff);

            setOverflowCarry8(num5);
        }
        else if (w == 1)
        {
            if (opSize)
            {
                string00 = "(%" + regs_32[rm] + ")" + "\n";

                int16_t num1, num2;
                int num3;
                uint16_t num4, num5;

                num1 = common.get_bits(1, 16, registers["EAX"]);
                num2 = memories16bit[common.getHex(registers[regs_32[rm]], 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[rm]], 0, 0));

                num3 = num1 * num2;
                num4 = common.get_bits(1, 16, num3);
                num5 = common.get_bits(16, 16, num3);

                registers["EAX"] = ((registers["EAX"]) & 0xffff0000) | (num4 & 0x0000ffff);
                registers["EDX"] = ((registers["EDX"]) & 0xffff0000) | (num5 & 0x0000ffff);

                setOverflowCarry16(num5);
            }
            else
            {
                int num1 = memories32bit[common.getHex(registers[regs_32[rm]], 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[rm]], 0, 0));

                int num2 = registers["EAX"]; // registers["ECX"]
                long int num3 = num1 * num2;
                unsigned int num4 = common.get_bits(1, 32, num3);
                unsigned int num5 = common.get_bits(32, 32, num3);

                string00 = "(%" + regs_32[rm] + ")" + "\n";
                registers["EAX"] = num4;
                registers["EDX"] = num5;

                setOverflowCarry32(num5);
            }
        }
    }

    return string00;
}

string Multiplier::decode_mod_01(int w, int rm)
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

string Multiplier::decode_mod_10(int w, int rm)
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

string Multiplier::decode_mod_11(int w, int rm)
{
    string string11 = "";
    if (w == 0)
    {
        int8_t num1, num2;
        int16_t num3;
        uint8_t num4;

        num1 = common.get_bits(1, 8, registers["EAX"]);

        if (rm < 4)
        {
            num2 = common.get_bits(1, 8, registers[regs_32[rm]]);
            num3 = num1 * num2;
            num4 = common.get_bits(8, 8, num3);
            registers["EAX"] = ((registers["EAX"]) & 0xffff0000) | (num3 & 0x0000ffff);
            
        }
        else
        {
            num2 = common.get_bits(9, 8, registers[regs_32[rm % 4]]);
            num3 = num1 * num2;
            num4 = common.get_bits(8, 8, num3);
            registers[regs_32[rm]] = ((registers["EAX"]) & 0xffff0000) | (num3 & 0x0000ffff);
        }
        cout << "Num1:"<<common.getHex(num1,0,0)<<" ; Num2:"<<common.getHex(num2,0,0) << " ; Num3:"<<common.getHex(num3,0,0)<<"\n";

        setOverflowCarry8(num4);

        string11 = "%" + regs_8[rm] + "\n";
    }
    else if (w == 1)
    {
        if (opSize)
        {
            int16_t num1, num2;
            int num3;
            uint16_t num4, num5;

            num1 = common.get_bits(1, 16, registers["EAX"]);
            num2 = common.get_bits(1, 16, registers[regs_32[rm]]);
            num3 = num1 * num2;
            num4 = common.get_bits(1, 16, num3);
            num5 = common.get_bits(16, 16, num3);
            registers["EAX"] = ((registers["EAX"]) & 0xffff0000) | (num4 & 0x0000ffff);
            registers["EDX"] = ((registers["EDX"]) & 0xffff0000) | (num5 & 0x0000ffff);

            setOverflowCarry16(num5);

            string11 = "%" + regs_16[rm] + "\n";
        }
        else
        {
            int num1 = registers["EAX"];
            int num2 = registers[regs_32[rm]];

            cout << "EAX:" << common.getHex(num1,0,0) << " ; " << regs_32[rm] <<":"<<common.getHex(num2,0,0)<<"\n";
            
            long int num3 = num1 * num2;
            cout << "out:" << common.getHex(num3,0,0)<<"\n";
            cout << sizeof(int) <<":"<<sizeof(long int)<<"\n";

            unsigned int num4 = common.get_bits(1, 32, num3);
            unsigned int num5 = common.get_bits(32, 32, num3);

            cout << "num1:"<<common.getHex(num4,0,0)<<" ; num5:"<<common.getHex(num5,0,0)<<"\n";
            registers["EAX"] = num4;
            registers["EDX"] = num5;

            setOverflowCarry32(num5);

            string11 = "%" + regs_32[rm] + "\n";
        }
    }
    return string11;
}

string Multiplier::decode_mul(short prefixes[4])
{
    string decoded_bytes;

    if (prefixes[3] == 0x67)
    {
        // Adder_addOverride adder_addOverride(common, instruction, registers, memories32bit, memories16bit, memories8bit, memoryAccesses);
        // decoded_bytes = adder_addOverride.decode_add(prefixes);
    }
    else
    {
        short opCode = instruction.front();
        bool w = common.get_bits(1, 1, opCode);

        instruction.pop();
        registers["EIP"] = registers["EIP"] + 1;

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
    }

    cout << "mul " << decoded_bytes;

    cout << "EAX: " << hex << registers["EAX"] << "\n";
    cout << "ECX: " << hex << registers["ECX"] << "\n";
    cout << "EDX: " << hex << registers["EDX"] << "\n";
    cout << "EBX: " << hex << registers["EBX"] << "\n";
    cout << "ESP: " << hex << registers["ESP"] << "\n";
    cout << "EBP: " << hex << registers["EBP"] << "\n";
    cout << "ESI: " << hex << registers["ESI"] << "\n";
    cout << "EDI: " << hex << registers["EDI"] << "\n";
    cout << "EFLAGS: " << hex << registers["EFLAGS"] << "\n \n";

    return "Multiplier instantiated and done";
}

void Multiplier::setOverflowCarry8(uint8_t num)
{
    if (num > 0)
    {
        registers["EFLAGS"] = registers["EFLAGS"] | 0x801;
    }
    else
    {
        registers["EFLAGS"] = registers["EFLAGS"] & 0xfffff7fe;
    }
}

void Multiplier::setOverflowCarry16(uint16_t num)
{
    if (num > 0)
    {
        registers["EFLAGS"] = registers["EFLAGS"] | 0x801;
    }
    else
    {
        registers["EFLAGS"] = registers["EFLAGS"] & 0xfffff7fe;
    }
}

void Multiplier::setOverflowCarry32(unsigned int num)
{
    if (num > 0)
    {
        registers["EFLAGS"] = registers["EFLAGS"] | 0x801;
    }
    else
    {
        registers["EFLAGS"] = registers["EFLAGS"] & 0xfffff7fe;
    }
}
