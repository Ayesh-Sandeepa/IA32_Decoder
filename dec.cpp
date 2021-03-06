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

Dec::Dec(Common com, queue<short> &instruction, map<string, int> &registers, map<string, int> &memories32bit, map<string, int16_t> &memories16bit, map<string, int8_t> &memories8bit, list<string> &memoryAccesses,ofstream &myoutput)
    : common(com), instruction(instruction), registers(registers), memories32bit(memories32bit), memories16bit(memories16bit), memories8bit(memories8bit), memoryAccesses(memoryAccesses),myoutput(myoutput)
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

string Dec::decode_displacement_with_SIB(int w, int mod, int index, int scale, int base)
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
                    memoryAccesses.push_back("dec " + st);

                    int8_t num1, num2;

                    num1 = memories8bit[st];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + st);

                    num2 = num1 - 1;

                    memories8bit[st] = num2;

                    memoryAccesses.push_back("write " + to_string(num2) + " to " + st);

                    common.setOverflow8bit(num1, 1, num2, registers);
                    common.setSign(num2, registers);
                    common.setZero(num2, registers);

                    dispWithSIB = st + "\n";
                }
                else
                {
                    memoryAccesses.push_back("dec %" + st + "(,%" + regs_32[index] + "," + to_string(scale) + ")");

                    int8_t num1, num2;

                    num1 = memories8bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                    num2 = num1 - 1;

                    memories8bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)] = num2;
                    memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                    common.setOverflow8bit(num1, 1, num2, registers);
                    common.setSign(num2, registers);
                    common.setZero(num2, registers);

                    dispWithSIB =  st + "(,%" + regs_32[index] + "," + to_string(scale) + ")\n";
                    }
            }
            else if (w == 1)
            {
                if (opSize)
                {
                    if (index == 4)
                    {
                        memoryAccesses.push_back("dec "+st );

                        int16_t num1, num2;

                        num1 = memories16bit[st];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + st);

                        num2 = num1 - 1;

                        memories16bit[st] = num2;
                        memoryAccesses.push_back("write " + to_string(num2) + " to " + st);

                        common.setOverflow16bit(num1, 1, num2, registers);
                        common.setSign(num2, registers);
                        common.setZero(num2, registers);

                        dispWithSIB = st + "\n";
                    }
                    else
                    {
                        memoryAccesses.push_back("dec "+st + "(,%" + regs_32[index] + "," + to_string(scale) + ")");

                        int16_t num1, num2;

                        num1 = memories16bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                        num2 = num1 - 1;

                        memories16bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)] = num1;
                        memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                        common.setOverflow16bit(num1, 1, num2, registers);
                        common.setSign(num2, registers);
                        common.setZero(num2, registers);

                        dispWithSIB = st + "(,%" + regs_32[index] + "," + to_string(scale) + ")\n";
                        }
                }
                else
                {
                    if (index == 4)
                    {
                        memoryAccesses.push_back("dec "+st );

                        int num1 = memories32bit[st];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + st);

                        int num2 = num1 - 1;

                        common.setOverflow32bit(num1, 1, num2, registers);
                        common.setSign(num2, registers);
                        common.setZero(num2, registers);

                        memories32bit[st] = num2;
                        memoryAccesses.push_back("write " + to_string(num2) + " to " + st);

                        dispWithSIB = st + "\n";
                    }
                    else
                    {
                        memoryAccesses.push_back("dec " + st + "(,%" + regs_32[index] + "," + to_string(scale) + ")" );

                        int num1 = memories32bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                        int num2 = num1 - 1;

                        common.setOverflow32bit(num1, 1, num2, registers);
                        common.setSign(num2, registers);
                        common.setZero(num2, registers);

                        memories32bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)] = num2;
                        memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                        dispWithSIB = st + "(,%" + regs_32[index] + "," + to_string(scale) + ")\n";
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
                    memoryAccesses.push_back("dec "  "(%" + regs_32[base] + ")");

                    int8_t num1, num2;

                    num1 = memories8bit[common.getHex(registers[regs_32[base]], 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]], 0, 0));

                    num2 = num1 - 1;

                    memories8bit[common.getHex(registers[regs_32[base]], 0, 0)] = num2;
                    memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]], 0, 0));

                    common.setOverflow8bit(num1, 1, num2, registers);
                    common.setSign(num2, registers);
                    common.setZero(num2, registers);

                    dispWithSIB =  "(%" + regs_32[base] + ")\n";
                }
                else
                {
                    memoryAccesses.push_back("dec (%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")");

                    int8_t num1, num2;

                    num1 = memories8bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2, 0, 0));

                    num2 = num1 - 1;

                    memories8bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2, 0, 0)] = num2;
                    memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2, 0, 0));

                    common.setOverflow8bit(num1, 1, num2, registers);
                    common.setSign(num2, registers);
                    common.setZero(num2, registers);

                    dispWithSIB = "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")\n";
                }
            }
            else if (w == 1)
            {
                if (opSize)
                {
                    if (index == 4)
                    {
                        memoryAccesses.push_back("dec (%" + regs_32[base] + ")");

                        int16_t num1, num2;

                        num1 = memories16bit[common.getHex(registers[regs_32[base]], 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]], 0, 0));

                        num2 = num1 - 1;

                        memories16bit[common.getHex(registers[regs_32[base]], 0, 0)] = num2;
                        memoryAccesses.push_back("write " + to_string(num2) + " to " + to_string((unsigned)registers[regs_32[base]]));

                        common.setOverflow16bit(num1, 1, num2, registers);
                        common.setSign(num2, registers);
                        common.setZero(num2, registers);

                        dispWithSIB = "(%" + regs_32[base] + ")\n";
                    }
                    else
                    {
                        memoryAccesses.push_back("dec (%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")");

                        int16_t num1, num2;

                        num1 = memories16bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0));

                        num2 = num1 - 1;

                        memories16bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0)] = num2;
                        memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0));

                        common.setOverflow16bit(num1, 1, num2, registers);
                        common.setSign(num2, registers);
                        common.setZero(num2, registers);

                        dispWithSIB = "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")\n";
                    }
                }
                else
                {
                    if (index == 4)
                    {
                        memoryAccesses.push_back("dec (%" + regs_32[base] + ")");

                        int num1 = memories32bit[common.getHex(registers[regs_32[base]], 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]], 0, 0));

                        int num2 = num1 - 1;

                        common.setOverflow32bit(num1, 1, num2, registers);
                        common.setSign(num2, registers);
                        common.setZero(num2, registers);

                        memories32bit[common.getHex(registers[regs_32[base]], 0, 0)] = num2;
                        memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]], 0, 0));

                        dispWithSIB = "(%" + regs_32[base] + ")\n";
                    }
                    else
                    {
                        memoryAccesses.push_back("dec (%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")");

                        int num1 = memories32bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                        int num2 = num1 - 1;

                        common.setOverflow32bit(num1, 1, num2, registers);
                        common.setSign(num2, registers);
                        common.setZero(num2, registers);

                        memories32bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)] = num2;
                        memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                        dispWithSIB = "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")\n";
                    }
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
                memoryAccesses.push_back("dec " +st + "(%" + regs_32[base] + ")" );

                int8_t num1, num2;

                num1 = memories8bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                num2 = num1 - 1;

                memories8bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)] = num2;
                memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                common.setOverflow8bit(num1, 1, num2, registers);
                common.setSign(num2, registers);
                common.setZero(num2, registers);

                dispWithSIB = st + "(%" + regs_32[base] + ")\n";
            }
            else
            {
                memoryAccesses.push_back("dec " + st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")");

                int8_t num1, num2;

                num1 = memories8bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0));

                num2 = num1 - 1;

                memories8bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0)] = num2;
                memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0));

                common.setOverflow8bit(num1, 1, num2, registers);
                common.setSign(num2, registers);
                common.setZero(num2, registers);

                dispWithSIB = st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")\n";
            }
        }
        else if (w == 1)
        {
            if (opSize)
            {
                if (index == 4)
                {
                    memoryAccesses.push_back("dec " + st + "(%" + regs_32[base] + ")");

                    int16_t num1, num2;

                    num1 = memories16bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                    num2 = num1 - 1;

                    memories16bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)] = num2;
                    memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                    common.setOverflow16bit(num1, 1, num2, registers);
                    common.setSign(num2, registers);
                    common.setZero(num2, registers);

                    dispWithSIB =  st + "(%" + regs_32[base] + ")\n";
                }
                else
                {
                    memoryAccesses.push_back("dec " + st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")");

                    int16_t num1, num2;

                    num1 = memories16bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0));

                    num2 = num1 - 1;

                    memories16bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0)] = num2;
                    memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0));

                    common.setOverflow16bit(num1, 1, num2, registers);
                    common.setSign(num2, registers);
                    common.setZero(num2, registers);

                    dispWithSIB = st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")\n";
                }
            }
            else
            {
                if (index == 4)
                {
                    memoryAccesses.push_back("dec " + st + "(%" + regs_32[base] + ")");

                    int num1 = memories32bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                    int num2 = num1 - 1;

                    common.setOverflow32bit(num1, 1, num2, registers);
                    common.setSign(num2, registers);
                    common.setZero(num2, registers);

                    memories32bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)] = num2;
                    memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                    dispWithSIB = st + "(%" + regs_32[base] + ")\n";
                }
                else
                {
                    memoryAccesses.push_back("dec " +st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")");

                    int num1 = memories32bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                    int num2 = num1 - 1;

                    common.setOverflow32bit(num1, 1, num2, registers);
                    common.setSign(num2, registers);
                    common.setZero(num2, registers);

                    memories32bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)] = num2;
                    memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                    dispWithSIB = st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")\n";
                }
            }
        }
    }
    return dispWithSIB;
}

string Dec::decode_displacement_without_SIB(int w, int mod, int rm)
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
            memoryAccesses.push_back("dec " + st);

            int8_t num1, num2;

            num1 = memories8bit[st];
            memoryAccesses.push_back("Read " + to_string(num1) + " from " + st);

            num2 = num1 - 1;

            memories8bit[st] = num2;
            memoryAccesses.push_back("write " + to_string(num2) + " to " + st);

            common.setOverflow8bit(num1, 1, num2, registers);
            common.setSign(num2, registers);
            common.setZero(num2, registers);

            dispWithoutSIB = st + "\n";
        }
        else if (w == 1)
        {
            if (opSize)
            {
                memoryAccesses.push_back("dec " + st );

                int16_t num1, num2;
                num1 = memories16bit[st];
                memoryAccesses.push_back("Read " + to_string(num1) + " from " + st);

                num2 = num1 - 1;

                memories16bit[st] = num2;
                memoryAccesses.push_back("write " + to_string(num2) + " to " + st);

                common.setOverflow16bit(num1, 1, num2, registers);
                common.setSign(num2, registers);
                common.setZero(num2, registers);

                dispWithoutSIB = st + "\n";

            }
            else
            {
                memoryAccesses.push_back("dec " + st );

                int num1 = memories32bit[st];
                memoryAccesses.push_back("Read " + to_string(num1) + " from " + st);

                int num2 = num1 - 1;

                common.setOverflow32bit(num1, 1, num2, registers);
                common.setSign(num2, registers);
                common.setZero(num2, registers);

                memories32bit[st] = num2;
                memoryAccesses.push_back("write " + to_string(num2) + " to " + st);

                dispWithoutSIB = st + "\n";
            }
        }
    }
    else
    {
        if (w == 0)
        {
        memoryAccesses.push_back("dec " + st + "(%" + regs_32[rm] + ")");

        int8_t num1, num2;

        num1 = memories8bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)];
        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

        num2 = num1 - 1;

        memories8bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)] = num2;
        memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

        common.setOverflow8bit(num1, 1, num2, registers);
        common.setSign(num2, registers);
        common.setZero(num2, registers);

        dispWithoutSIB = st + "(%" + regs_32[rm] + ")\n";
        }
        else if (w == 1)
        {
            if (opSize)
            {                   
            memoryAccesses.push_back("dec " + st + "(%" + regs_32[rm] + ")");

            int16_t num1, num2;
            num1 = memories16bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)];
            memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

            num2 = num1 - 1;

            memories16bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)] = num2;
            memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

            common.setOverflow16bit(num1, 1, num2, registers);
            common.setSign(num2, registers);
            common.setZero(num2, registers);

            dispWithoutSIB = st + "(%" + regs_32[rm] + ")\n";
            }
            else
            {
                int num1 = memories32bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

                int num2 = num1 - 1;

                common.setOverflow32bit(num1, 1, num2, registers);
                common.setSign(num2, registers);
                common.setZero(num2, registers);

                memories32bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)] = num2;
                memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

                dispWithoutSIB = st + "(%" + regs_32[rm] + ")\n";

            }
        }
    }
    return dispWithoutSIB;
};

string Dec::decode_SIB(int w, int mod)
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

string Dec::decode_mod_00(int w, int rm)
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
            int8_t num1, num2;
            memoryAccesses.push_back("dec (%" + regs_32[rm] + ")");
            string00 = "(%" + regs_32[rm] + ")\n";

            num1 = memories8bit[common.getHex(registers[regs_32[rm]], 0, 0)];
            memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[rm]], 0, 0));
            num2 = num1 - 1;

            memories8bit[common.getHex(registers[regs_32[rm]], 0, 0)] = num2;
            memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[rm]], 0, 0));

            common.setOverflow8bit(num1, 1, num2, registers);
            common.setSign(num2, registers);
            common.setZero(num2, registers);
        }
        else if (w == 1)
        {
            if (opSize)
            {
                memoryAccesses.push_back("dec (%" + regs_32[rm] + ")");
                string00 = "(%" + regs_32[rm] + ")\n";

                int16_t num1, num2;
                num1 = memories16bit[common.getHex(registers[regs_32[rm]], 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[rm]], 0, 0));

                num2 = num1 - 1;

                memories16bit[common.getHex(registers[regs_32[rm]], 0, 0)] = num2;
                memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[rm]], 0, 0));

                common.setOverflow16bit(num1, 1, num2, registers);
                common.setSign(num2, registers);
                common.setZero(num2, registers);
            }
            else
            {
                memoryAccesses.push_back("dec (%" + regs_32[rm] + ")");

                int num1 = memories32bit[common.getHex(registers[regs_32[rm]], 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[rm]], 0, 0));

                int num2 = num1 - 1;

                common.setOverflow32bit(num1, 1, num2, registers);
                common.setSign(num2, registers);
                common.setZero(num2, registers);

                string00 = "(%" + regs_32[rm] + ")\n";
                memories32bit[common.getHex(registers[regs_32[rm]], 0, 0)] = num2;
                memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[rm]], 0, 0));
            }
        }
    }
    return string00;
}

string Dec::decode_mod_01(int w, int rm)
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

string Dec::decode_mod_10(int w, int rm)
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

string Dec::decode_mod_11(int w, int rm)
{
    string string11 = "";
    int8_t num1, num2;
    if (w == 0)
    {
        int8_t num1, num2;
        if (rm < 4)
        {
            num1 = common.get_bits(1, 8, registers[regs_32[rm]]);
            num2 = num1 - 1;
            registers[regs_32[rm]] = ((registers[regs_32[rm]]) & 0xffffff00) | (num2 & 0x000000ff);
        }
        else
        {
            num1 = common.get_bits(9, 8, registers[regs_32[rm % 4]]);
            num2 = num1 - 1;
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
            num2 = num1 - 1;;

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
            num2 = num1 - 1;
            registers[regs_32[rm]] = num1;

            common.setOverflow32bit(num1, 1 , num2, registers);
            common.setSign(num2, registers);
            common.setZero(num2, registers);

            string11 = "%" + regs_32[rm] + "\n";
        }
    }
    return string11;
}


// parity and auxiliary flag should be used .......................................................//
// Exception handling when memory address is not there, normally the addressest that are not initiailized are set to zero
// Display the memory accesses at final
// Decode instructions with prefixes
// Add other opcodes 0x81 etc
// Use of SS in using reference addresses of ESP and EBP registers
string Dec::decode_dec(short prefixes[4], int w)
{
    string decoded_bytes;

    if (prefixes[3] == 0x67)
    {
        Dec_override Dec_override(common, instruction, registers, memories32bit, memories16bit, memories8bit, memoryAccesses);
        decoded_bytes = Dec_override.decode_dec(prefixes, w);

        cout << "dec " << decoded_bytes;

        cout << "EAX: " << hex << registers["EAX"] << "\n";
        cout << "ECX: " << hex << registers["ECX"] << "\n";
        cout << "EDX: " << hex << registers["EDX"] << "\n";
        cout << "EBX: " << hex << registers["EBX"] << "\n";
        cout << "ESP: " << hex << registers["ESP"] << "\n";
        cout << "EBP: " << hex << registers["EBP"] << "\n";
        cout << "ESI: " << hex << registers["ESI"] << "\n";
        cout << "EDI: " << hex << registers["EDI"] << "\n";
        cout << "EFLAGS: " << hex << registers["EFLAGS"] << "\n \n";

        myoutput << "dec " << decoded_bytes;

        myoutput  << "EAX: " << hex << registers["EAX"] << "\n";
        myoutput << "ECX: " << hex << registers["ECX"] << "\n";
        myoutput  << "EDX: " << hex << registers["EDX"] << "\n";
        myoutput << "EBX: " << hex << registers["EBX"] << "\n";
        myoutput  << "ESP: " << hex << registers["ESP"] << "\n";
        myoutput  << "EBP: " << hex << registers["EBP"] << "\n";
        myoutput  << "ESI: " << hex << registers["ESI"] << "\n";
        myoutput  << "EDI: " << hex << registers["EDI"] << "\n";
        myoutput  << "EFLAGS: " << hex << registers["EFLAGS"] << "\n \n";
    }
    else
    {
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

    return decoded_bytes;
}
