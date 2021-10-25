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

#include "left_right_shift.h"
//#include "left_right_shift_immediate.h"

using namespace std;

Left_shift::Left_shift(Common com, queue<short> &instruction, map<string, int> &registers, map<string, int> &memories32bit, map<string, int16_t> &memories16bit, map<string, int8_t> &memories8bit, list<string> &memoryAccesses)
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

string Left_shift::decode_displacement_with_SIB(int d, int w, int mod, int index, int scale, int base)
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
            if (w == 0 and d == 0)
            {
                if (index == 4)
                {
                    memoryAccesses.push_back("shl " + st);

                    int8_t num1;

                    num1 = memories8bit[st];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + st);

                    int num2 = num1 << 1;

                    memories8bit[st] = (int8_t)num2;

                    memoryAccesses.push_back("write " + to_string(num2) + " to " + st);

                    common.setOverflow8bit(num1, num1, num2, registers);
                    common.setCarry8bit(num1, num2, registers);
                    common.setSign(num2, registers);
                    common.setZero(num2, registers);

                    dispWithSIB = st + "\n";
                }
                else
                {
                    memoryAccesses.push_back("shl %" + st + "(,%" + regs_32[index] + "," + to_string(scale) + ")");

                    int8_t num1;

                    num1 = memories8bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                    int num2 = num1 << 1;

                    memories8bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)] = (int8_t)num2;
                    memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                    common.setOverflow8bit(num1, num1, num2, registers);
                    common.setCarry8bit(num1, num2, registers);
                    common.setSign(num2, registers);
                    common.setZero(num2, registers);

                    dispWithSIB =  st + "(,%" + regs_32[index] + "," + to_string(scale) + ")\n";
                    }
            }
            else if (w == 0 and d == 1)
            {
                if (index == 4)
                {
                    memoryAccesses.push_back("shl " + st);

                    int8_t num1;

                    num1 = memories8bit[st];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + st);

                    int num2 = num1 << common.get_bits(1, 5, registers["ECX"]);

                    memories8bit[st] = (int8_t)num2;

                    memoryAccesses.push_back("write " + to_string(num2) + " to " + st);

                    common.setCarry8bit(num1, num2, registers); 
                    common.setSign(num2, registers);
                    common.setZero(num2, registers);

                    dispWithSIB = st + "\n";
                }
                else
                {
                    memoryAccesses.push_back("shl %" + st + "(,%" + regs_32[index] + "," + to_string(scale) + ")");

                    int8_t num1;

                    num1 = memories8bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                    int num2 = num1 << common.get_bits(1, 5, registers["ECX"]);

                    memories8bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)] = (int8_t)num2;
                    memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));
  
                    common.setCarry8bit(num1, num2, registers); 
                    common.setSign(num2, registers);
                    common.setZero(num2, registers);

                    dispWithSIB =  st + "(,%" + regs_32[index] + "," + to_string(scale) + ")\n";
                    }
            }
            else if (w == 1 and d ==0)
            {
                if (opSize)
                {
                    if (index == 4)
                    {
                        memoryAccesses.push_back("shl "+st );

                        int16_t num1;

                        num1 = memories16bit[st];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + st);

                        int num2 = num1 << 1;

                        memories16bit[st] = (int16_t)num2;;
                        memoryAccesses.push_back("write " + to_string(num2) + " to " + st);

                        common.setOverflow16bit(num1, num1, num2, registers);
                        common.setCarry16bit(num1, num2, registers);
                        common.setSign(num2, registers);
                        common.setZero(num2, registers);

                        dispWithSIB = st + "\n";
                    }
                    else
                    {
                        memoryAccesses.push_back("shl "+st + "(,%" + regs_32[index] + "," + to_string(scale) + ")");

                        int16_t num1;

                        num1 = memories16bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                        int num2 = num1 << 1;

                        memories16bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)] = (int16_t)num2;;
                        memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                        common.setOverflow16bit(num1, num1, num2, registers);
                        common.setCarry16bit(num1, num2, registers);
                        common.setSign(num2, registers);
                        common.setZero(num2, registers);

                        dispWithSIB = st + "(,%" + regs_32[index] + "," + to_string(scale) + ")\n";
                        }
                }
                else
                {
                    if (index == 4)
                    {
                        memoryAccesses.push_back("shl "+st );

                        int num1 = memories32bit[st];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + st);

                        int num2 = num1 << 1;

                        common.setOverflow32bit(num1, num1, num2, registers);
                        common.setCarry32bit(num1, num2, registers);
                        common.setSign(num2, registers);
                        common.setZero(num2, registers);

                        memories32bit[st] = (int32_t)num2;
                        memoryAccesses.push_back("write " + to_string(num2) + " to " + st);

                        dispWithSIB = st + "\n";
                    }
                    else
                    {
                        memoryAccesses.push_back("shl " + st + "(,%" + regs_32[index] + "," + to_string(scale) + ")" );

                        int num1 = memories32bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                        int num2 = num1 << 1;

                        common.setOverflow32bit(num1, num1, num2, registers);
                        common.setCarry32bit(num1, num2, registers);
                        common.setSign(num2, registers);
                        common.setZero(num2, registers);

                        memories32bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)] = (int32_t)num2;;
                        memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                        dispWithSIB = st + "(,%" + regs_32[index] + "," + to_string(scale) + ")\n";
                    }
                }
            }
            else if (w == 1 and d ==1)
            {
                if (opSize)
                {
                    if (index == 4)
                    {
                        memoryAccesses.push_back("shl "+st );

                        int16_t num1;

                        num1 = memories16bit[st];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + st);

                        int num2 = num1 << common.get_bits(1, 5, registers["ECX"]);

                        memories16bit[st] = (int16_t)num2;
                        memoryAccesses.push_back("write " + to_string(num2) + " to " + st);


                        common.setCarry16bit(num1, num2, registers);
                        common.setSign(num2, registers);
                        common.setZero(num2, registers);

                        dispWithSIB = st + "\n";
                    }
                    else
                    {
                        memoryAccesses.push_back("shl "+st + "(,%" + regs_32[index] + "," + to_string(scale) + ")");

                        int16_t num1;

                        num1 = memories16bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                        int num2 = num1 << common.get_bits(1, 5, registers["ECX"]);

                        memories16bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)] = (int16_t)num2;
                        memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                        common.setCarry16bit(num1, num2, registers);
                        common.setSign(num2, registers);
                        common.setZero(num2, registers);

                        dispWithSIB = st + "(,%" + regs_32[index] + "," + to_string(scale) + ")\n";
                        }
                }
                else
                {
                    if (index == 4)
                    {
                        memoryAccesses.push_back("shl "+st );

                        int num1 = memories32bit[st];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + st);

                        int num2 = num1 << common.get_bits(1, 5, registers["ECX"]);

                        common.setCarry32bit(num1, num2, registers);
                        common.setSign(num2, registers);
                        common.setZero(num2, registers);

                        memories32bit[st] = (int32_t)num2;
                        memoryAccesses.push_back("write " + to_string(num2) + " to " + st);

                        dispWithSIB = st + "\n";
                    }
                    else
                    {
                        memoryAccesses.push_back("shl " + st + "(,%" + regs_32[index] + "," + to_string(scale) + ")" );

                        int num1 = memories32bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                        int num2 = num1 << common.get_bits(1, 5, registers["ECX"]);

                        common.setCarry32bit(num1, num2, registers);
                        common.setSign(num2, registers);
                        common.setZero(num2, registers);

                        memories32bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)] = (int32_t)num2;
                        memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                        dispWithSIB = st + "(,%" + regs_32[index] + "," + to_string(scale) + ")\n";
                    }
                }
            }
        }
        else
        {
            if (w == 0 and d == 0)
            {
                if (index == 4)
                {
                    memoryAccesses.push_back("shl "  "(%" + regs_32[base] + ")");

                    int8_t num1;

                    num1 = memories8bit[common.getHex(registers[regs_32[base]], 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]], 0, 0));

                    int num2 = num1 << 1;

                    memories8bit[common.getHex(registers[regs_32[base]], 0, 0)] = (int8_t)num2;;
                    memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]], 0, 0));

                    common.setOverflow8bit(num1, num1, num2, registers);
                    common.setCarry8bit(num1, num2, registers);
                    common.setSign(num2, registers);
                    common.setZero(num2, registers);

                    dispWithSIB =  "(%" + regs_32[base] + ")\n";
                }
                else
                {
                    memoryAccesses.push_back("shl (%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")");

                    int8_t num1;

                    num1 = memories8bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2, 0, 0));

                    int num2 = num1 << 1;

                    memories8bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2, 0, 0)] = (int8_t)num2;
                    memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2, 0, 0));

                    common.setOverflow8bit(num1, num1, num2, registers);
                    common.setCarry8bit(num1, num2, registers);
                    common.setSign(num2, registers);
                    common.setZero(num2, registers);

                    dispWithSIB = "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")\n";
                }
            }
            if (w == 0 and d == 1)
            {
                if (index == 4)
                {
                    memoryAccesses.push_back("shl "  "(%" + regs_32[base] + ")");

                    int8_t num1;

                    num1 = memories8bit[common.getHex(registers[regs_32[base]], 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]], 0, 0));

                    int num2 = num1 << common.get_bits(1, 5, registers["ECX"]);

                    memories8bit[common.getHex(registers[regs_32[base]], 0, 0)] = (int8_t)num2;
                    memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]], 0, 0));

                    common.setCarry8bit(num1, num2, registers);
                    common.setSign(num2, registers);
                    common.setZero(num2, registers);

                    dispWithSIB =  "(%" + regs_32[base] + ")\n";
                }
                else
                {
                    memoryAccesses.push_back("shl (%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")");

                    int8_t num1;

                    num1 = memories8bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2, 0, 0));

                    int num2 = num1 << common.get_bits(1, 5, registers["ECX"]);

                    memories8bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2, 0, 0)] = (int8_t)num2;
                    memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2, 0, 0));

                    common.setCarry8bit(num1, num2, registers);
                    common.setSign(num2, registers);
                    common.setZero(num2, registers);

                    dispWithSIB = "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")\n";
                }
            }
            else if (w == 1 and d == 0)
            {
                if (opSize)
                {
                    if (index == 4)
                    {
                        memoryAccesses.push_back("shl (%" + regs_32[base] + ")");

                        int16_t num1;

                        num1 = memories16bit[common.getHex(registers[regs_32[base]], 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]], 0, 0));

                        int num2 = num1 << 1;

                        memories16bit[common.getHex(registers[regs_32[base]], 0, 0)] = (int16_t)num2;
                        memoryAccesses.push_back("write " + to_string(num2) + " to " + to_string((unsigned)registers[regs_32[base]]));

                        common.setOverflow16bit(num1, num1, num2, registers);
                        common.setCarry16bit(num1, num2, registers);
                        common.setSign(num2, registers);
                        common.setZero(num2, registers);

                        dispWithSIB = "(%" + regs_32[base] + ")\n";
                    }
                    else
                    {
                        memoryAccesses.push_back("shl (%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")");

                        int16_t num1;

                        num1 = memories16bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0));

                        int num2 = num1 << 1;

                        memories16bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0)] = (int16_t)num2;
                        memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0));

                        common.setOverflow16bit(num1, num1, num2, registers);
                        common.setCarry16bit(num1, num2, registers);
                        common.setSign(num2, registers);
                        common.setZero(num2, registers);

                        dispWithSIB = "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")\n";
                    }
                }
                else
                {
                    if (index == 4)
                    {
                        memoryAccesses.push_back("shl (%" + regs_32[base] + ")");

                        int num1 = memories32bit[common.getHex(registers[regs_32[base]], 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]], 0, 0));

                        int num2 = num1 << 1;

                        common.setOverflow32bit(num1, num1, num2, registers);
                        common.setCarry32bit(num1, num2, registers);
                        common.setSign(num2, registers);
                        common.setZero(num2, registers);

                        memories32bit[common.getHex(registers[regs_32[base]], 0, 0)] = (int32_t)num2;
                        memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]], 0, 0));

                        dispWithSIB = "(%" + regs_32[base] + ")\n";
                    }
                    else
                    {
                        memoryAccesses.push_back("shl (%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")");

                        int num1 = memories32bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                        int num2 = num1 << 1;

                        common.setOverflow32bit(num1, num1, num2, registers);
                        common.setCarry32bit(num1, num2, registers);
                        common.setSign(num2, registers);
                        common.setZero(num2, registers);

                        memories32bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)] = (int32_t)num2;
                        memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                        dispWithSIB = "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")\n";
                    }
                }
            }
            else
            {
                if (opSize)
                {
                    if (index == 4)
                    {
                        memoryAccesses.push_back("shl (%" + regs_32[base] + ")");

                        int16_t num1;

                        num1 = memories16bit[common.getHex(registers[regs_32[base]], 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]], 0, 0));

                        int num2 = num1 << common.get_bits(1, 5, registers["ECX"]);

                        memories16bit[common.getHex(registers[regs_32[base]], 0, 0)] = (int16_t)num2;
                        memoryAccesses.push_back("write " + to_string(num2) + " to " + to_string((unsigned)registers[regs_32[base]]));

                        common.setCarry16bit(num1, num2, registers);
                        common.setSign(num2, registers);
                        common.setZero(num2, registers);

                        dispWithSIB = "(%" + regs_32[base] + ")\n";
                    }
                    else
                    {
                        memoryAccesses.push_back("shl (%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")");

                        int16_t num1;

                        num1 = memories16bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0));

                        int num2 = num1 << common.get_bits(1, 5, registers["ECX"]);

                        memories16bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0)] = (int16_t)num2;
                        memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0));

                        common.setCarry16bit(num1, num2, registers);
                        common.setSign(num2, registers);
                        common.setZero(num2, registers);

                        dispWithSIB = "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")\n";
                    }
                }
                else
                {
                    if (index == 4)
                    {
                        memoryAccesses.push_back("shl (%" + regs_32[base] + ")");

                        int num1 = memories32bit[common.getHex(registers[regs_32[base]], 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]], 0, 0));

                        int num2 = num1 << common.get_bits(1, 5, registers["ECX"]);

                        common.setCarry32bit(num1, num2, registers);
                        common.setSign(num2, registers);
                        common.setZero(num2, registers);

                        memories32bit[common.getHex(registers[regs_32[base]], 0, 0)] = (int32_t)num2;
                        memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]], 0, 0));

                        dispWithSIB = "(%" + regs_32[base] + ")\n";
                    }
                    else
                    {
                        memoryAccesses.push_back("shl (%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")");

                        int num1 = memories32bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                        int num2 = num1 << common.get_bits(1, 5, registers["ECX"]);

                        common.setCarry32bit(num1, num2, registers);
                        common.setSign(num2, registers);
                        common.setZero(num2, registers);

                        memories32bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)] = (int32_t)num2;
                        memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                        dispWithSIB = "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")\n";
                    }
                }
            }
        }
    }
    else
    {
        if (w == 0 and d == 0)
        {
            if (index == 4)
            {
                memoryAccesses.push_back("shl " +st + "(%" + regs_32[base] + ")" );

                int8_t num1;

                num1 = memories8bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                int num2 = num1 << 1;

                memories8bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)] = (int8_t)num2;
                memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                common.setOverflow8bit(num1, num1, num2, registers);
                common.setCarry8bit(num1, num2, registers);
                common.setSign(num2, registers);
                common.setZero(num2, registers);

                dispWithSIB = st + "(%" + regs_32[base] + ")\n";
            }
            else
            {
                memoryAccesses.push_back("shl " + st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")");

                int8_t num1;

                num1 = memories8bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0));

                int num2 = num1 << 1;

                memories8bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0)] = (int8_t)num2;
                memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0));

                common.setOverflow8bit(num1, num1, num2, registers);
                common.setCarry8bit(num1, num2, registers);
                common.setSign(num2, registers);
                common.setZero(num2, registers);

                dispWithSIB = st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")\n";
            }
        }
        if (w == 0 and d == 1)
        {
            if (index == 4)
            {
                memoryAccesses.push_back("shl " +st + "(%" + regs_32[base] + ")" );

                int8_t num1;

                num1 = memories8bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                int num2 = num1 << common.get_bits(1, 5, registers["ECX"]);

                memories8bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)] = (int8_t)num2;
                memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                common.setCarry8bit(num1, num2, registers);
                common.setSign(num2, registers);
                common.setZero(num2, registers);

                dispWithSIB = st + "(%" + regs_32[base] + ")\n";
            }
            else
            {
                memoryAccesses.push_back("shl " + st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")");

                int8_t num1;

                num1 = memories8bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0));

                int num2 = num1 << common.get_bits(1, 5, registers["ECX"]);

                memories8bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0)] = (int8_t)num2;
                memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0));

                common.setCarry8bit(num1, num2, registers);
                common.setSign(num2, registers);
                common.setZero(num2, registers);

                dispWithSIB = st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")\n";
            }
        }
        else if (w == 1 and d == 0)
        {
            if (opSize)
            {
                if (index == 4)
                {
                    memoryAccesses.push_back("shl " + st + "(%" + regs_32[base] + ")");

                    int16_t num1;

                    num1 = memories16bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                    int num2 = num1 << 1;

                    memories16bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)] = (int16_t)num2;
                    memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                    common.setOverflow16bit(num1, num1, num2, registers);
                    common.setCarry16bit(num1, num2, registers);
                    common.setSign(num2, registers);
                    common.setZero(num2, registers);

                    dispWithSIB =  st + "(%" + regs_32[base] + ")\n";
                }
                else
                {
                    memoryAccesses.push_back("shl " + st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")");

                    int16_t num1;

                    num1 = memories16bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0));

                    int num2 = num1 << 1;

                    memories16bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0)] = (int16_t)num2;
                    memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0));

                    common.setOverflow16bit(num1, num1, num2, registers);
                    common.setCarry16bit(num1, num2, registers);
                    common.setSign(num2, registers);
                    common.setZero(num2, registers);

                    dispWithSIB = st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")\n";
                }
            }
            else
            {
                if (index == 4)
                {
                    memoryAccesses.push_back("shl " + st + "(%" + regs_32[base] + ")");

                    int num1 = memories32bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                    int num2 = num1 << 1;

                    common.setOverflow32bit(num1, num1, num2, registers);
                    common.setCarry32bit(num1, num2, registers);
                    common.setSign(num2, registers);
                    common.setZero(num2, registers);

                    memories32bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)] = (int32_t)num2;
                    memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                    dispWithSIB = st + "(%" + regs_32[base] + ")\n";
                }
                else
                {
                    memoryAccesses.push_back("shl " +st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")");

                    int num1 = memories32bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                    int num2 = num1 << 1;

                    common.setOverflow32bit(num1, num1, num2, registers);
                    common.setCarry32bit(num1, num2, registers);
                    common.setSign(num2, registers);
                    common.setZero(num2, registers);

                    memories32bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)] = (int32_t)num2;
                    memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                    dispWithSIB = st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")\n";
                }
            }
        }
        else
        {
            if (opSize)
            {
                if (index == 4)
                {
                    memoryAccesses.push_back("shl " + st + "(%" + regs_32[base] + ")");

                    int16_t num1;

                    num1 = memories16bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                    int num2 = num1 << common.get_bits(1, 5, registers["ECX"]);

                    memories16bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)] = (int16_t)num2;
                    memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                    common.setCarry16bit(num1, num2, registers);
                    common.setSign(num2, registers);
                    common.setZero(num2, registers);

                    dispWithSIB =  st + "(%" + regs_32[base] + ")\n";
                }
                else
                {
                    memoryAccesses.push_back("shl " + st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")");

                    int16_t num1;

                    num1 = memories16bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0));

                    int num2 = num1 << common.get_bits(1, 5, registers["ECX"]);

                    memories16bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0)] = (int16_t)num2;
                    memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0));

                    common.setCarry16bit(num1, num2, registers);
                    common.setSign(num2, registers);
                    common.setZero(num2, registers);

                    dispWithSIB = st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")\n";
                }
            }
            else
            {
                if (index == 4)
                {
                    memoryAccesses.push_back("shl " + st + "(%" + regs_32[base] + ")");

                    int num1 = memories32bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                    int num2 = num1 << common.get_bits(1, 5, registers["ECX"]);

                    common.setCarry32bit(num1, num2, registers);
                    common.setSign(num2, registers);
                    common.setZero(num2, registers);

                    memories32bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)] = (int32_t)num2;
                    memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                    dispWithSIB = st + "(%" + regs_32[base] + ")\n";
                }
                else
                {
                    memoryAccesses.push_back("shl " +st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")");

                    int num1 = memories32bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                    int num2 = num1 << common.get_bits(1, 5, registers["ECX"]);

                    common.setCarry32bit(num1, num2, registers);
                    common.setSign(num2, registers);
                    common.setZero(num2, registers);

                    memories32bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)] = (int32_t)num2;
                    memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                    dispWithSIB = st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")\n";
                }
            }
        }
    }
    return dispWithSIB;
}

string Left_shift::decode_displacement_without_SIB(int d, int w, int mod, int rm)
{
    string dispWithoutSIB = "";
    int disp_bytes[] = {4, 1, 4};
    int bytes = disp_bytes[mod];

    unsigned int disp = common.assemble_bits(bytes, instruction, registers);
    string st = common.getHex(disp, 0, 0);

    if (mod == 0)
    {
        if (w == 0 and d == 0)
        {
            memoryAccesses.push_back("shl " + st);

            int8_t num1;

            num1 = memories8bit[st];
            memoryAccesses.push_back("Read " + to_string(num1) + " from " + st);

            int num2 = num1 << 1;

            memories8bit[st] = (int8_t)num2;
            memoryAccesses.push_back("write " + to_string(num2) + " to " + st);

            common.setOverflow8bit(num1, num1, num2, registers);
            common.setCarry8bit(num1, num2, registers);
            common.setSign(num2, registers);
            common.setZero(num2, registers);

            dispWithoutSIB = st + "\n";
        }
        else if (w == 0 and d == 1)
        {
            memoryAccesses.push_back("shl " + st);

            int8_t num1;

            num1 = memories8bit[st];
            memoryAccesses.push_back("Read " + to_string(num1) + " from " + st);

            int num2 = num1 <<  common.get_bits(1, 8, registers["ECX"]);

            memories8bit[st] = (int8_t)num2;
            memoryAccesses.push_back("write " + to_string(num2) + " to " + st);

            common.setCarry8bit(num1, num2, registers);
            common.setSign(num2, registers);
            common.setZero(num2, registers);

            dispWithoutSIB = st + "\n";
        }
        else if (w == 1 and d ==0)
        {
            if (opSize)
            {
                memoryAccesses.push_back("shl " + st );

                int16_t num1;
                num1 = memories16bit[st];
                memoryAccesses.push_back("Read " + to_string(num1) + " from " + st);

                int num2 = num1 << 1;

                memories16bit[st] = (int16_t)num2;
                memoryAccesses.push_back("write " + to_string(num2) + " to " + st);

                common.setOverflow16bit(num1, num1, num2, registers);
                common.setCarry16bit(num1, num2, registers);
                common.setSign(num2, registers);
                common.setZero(num2, registers);

                dispWithoutSIB = st + "\n";

            }
            else
            {
                memoryAccesses.push_back("shl " + st );

                int num1 = memories32bit[st];
                memoryAccesses.push_back("Read " + to_string(num1) + " from " + st);

                int num2 = num1 << 1;

                common.setOverflow32bit(num1, num1, num2, registers);
                common.setCarry32bit(num1, num2, registers);
                common.setSign(num2, registers);
                common.setZero(num2, registers);

                memories32bit[st] = (int32_t)num2;
                memoryAccesses.push_back("write " + to_string(num2) + " to " + st);

                dispWithoutSIB = st + "\n";
            }
        }
        else
        {
            if (opSize)
            {
                memoryAccesses.push_back("shl " + st );

                int16_t num1;
                num1 = memories16bit[st];
                memoryAccesses.push_back("Read " + to_string(num1) + " from " + st);

                int num2 = num1 << common.get_bits(1, 5, registers["ECX"]);

                memories16bit[st] = (int16_t)num2;
                memoryAccesses.push_back("write " + to_string(num2) + " to " + st);

                common.setCarry16bit(num1, num2, registers);
                common.setSign(num2, registers);
                common.setZero(num2, registers);

                dispWithoutSIB = st + "\n";

            }
            else
            {
                memoryAccesses.push_back("shl " + st );

                int num1 = memories32bit[st];
                memoryAccesses.push_back("Read " + to_string(num1) + " from " + st);

                int num2 = num1 << common.get_bits(1, 5, registers["ECX"]);

                common.setCarry32bit(num1, num2, registers);
                common.setSign(num2, registers);
                common.setZero(num2, registers);

                memories32bit[st] = (int32_t)num2;
                memoryAccesses.push_back("write " + to_string(num2) + " to " + st);

                dispWithoutSIB = st + "\n";
            }
        }
    }
    else
    {
        if (w == 0 and d ==0)
        {
            memoryAccesses.push_back("shl " + st + "(%" + regs_32[rm] + ")");

            int8_t num1;

            num1 = memories8bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)];
            memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

            int num2 = num1 << 1;

            memories8bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)] = (int8_t)num2;
            memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

            common.setOverflow8bit(num1, num1, num2, registers);
            common.setCarry8bit(num1, num2, registers);
            common.setSign(num2, registers);
            common.setZero(num2, registers);

            dispWithoutSIB = st + "(%" + regs_32[rm] + ")\n";
        }
        if (w == 0 and d ==1)
        {
            memoryAccesses.push_back("shl " + st + "(%" + regs_32[rm] + ")");

            int8_t num1;

            num1 = memories8bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)];
            memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

            int num2 = num1 << common.get_bits(1, 5, registers["ECX"]);


            memories8bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)] = (int8_t)num2;
            memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

            common.setCarry8bit(num1, num2, registers);
            common.setSign(num2, registers);
            common.setZero(num2, registers);

            dispWithoutSIB = st + "(%" + regs_32[rm] + ")\n";
        }
        else if (w == 1 and d == 0)
        {
            if (opSize)
            {                   
                memoryAccesses.push_back("shl " + st + "(%" + regs_32[rm] + ")");

                int16_t num1;
                num1 = memories16bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

                int num2 = num1 << 1;

                memories16bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)] = (int16_t)num2;
                memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

                common.setOverflow16bit(num1, num1, num2, registers);
                common.setCarry16bit(num1, num2, registers);
                common.setSign(num2, registers);
                common.setZero(num2, registers);

                dispWithoutSIB = st + "(%" + regs_32[rm] + ")\n";
            }
            else
            {
                int num1 = memories32bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

                int num2 = num1 << 1;

                common.setOverflow32bit(num1, num1, num2, registers);
                common.setCarry32bit(num1, num2, registers);
                common.setSign(num2, registers);
                common.setZero(num2, registers);

                memories32bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)] = (int32_t)num2;
                memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

                dispWithoutSIB = st + "(%" + regs_32[rm] + ")\n";

            }
        }
        else
        {
            if (opSize)
            {                   
                memoryAccesses.push_back("shl " + st + "(%" + regs_32[rm] + ")");

                int16_t num1;
                num1 = memories16bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

                int num2 = num1 << common.get_bits(1, 5, registers["ECX"]);

                memories16bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)] = (int16_t)num2;
                memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

                common.setCarry16bit(num1, num2, registers);
                common.setSign(num2, registers);
                common.setZero(num2, registers);

                dispWithoutSIB = st + "(%" + regs_32[rm] + ")\n";
            }
            else
            {
                int num1 = memories32bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

                int num2 = num1 << common.get_bits(1, 5, registers["ECX"]);

                common.setCarry32bit(num1, num2, registers);
                common.setSign(num2, registers);
                common.setZero(num2, registers);

                memories32bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)] = (int32_t)num2;
                memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

                dispWithoutSIB = st + "(%" + regs_32[rm] + ")\n";

            }
        }
    }
    return dispWithoutSIB;
};

string Left_shift::decode_SIB(int d, int w, int mod)
{
    string stringSib = "";

    int scale = common.get_bits(7, 2, instruction.front());
    int index = common.get_bits(4, 3, instruction.front());
    int base = common.get_bits(1, 3, instruction.front());

    instruction.pop();
    registers["EIP"] = registers["EIP"] << 1;

    scale = pow(2, scale);

    stringSib = decode_displacement_with_SIB(d, w, mod, index, scale, base);
    return stringSib;
}

string Left_shift::decode_mod_00(int d, int w, int rm)
{
    string string00 = "";
    if (rm == 4)
    {
        string00 = decode_SIB(d, w, 0);
    }
    else if (rm == 5)
    {
        string00 = decode_displacement_without_SIB(d, w, 0, 5);
    }
    else
    {
        if (w == 0 and d ==0)
        {
            int8_t num1;
            memoryAccesses.push_back("shl (%" + regs_32[rm] + ")");
            string00 = "(%" + regs_32[rm] + ")\n";

            num1 = memories8bit[common.getHex(registers[regs_32[rm]], 0, 0)];
            memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[rm]], 0, 0));
            int num2 = num1 << 1;

            memories8bit[common.getHex(registers[regs_32[rm]], 0, 0)] = (int8_t)num2;
            memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[rm]], 0, 0));

            common.setOverflow8bit(num1, num1, num2, registers);
            common.setCarry8bit(num1, num2, registers);
            common.setSign(num2, registers);
            common.setZero(num2, registers);
        }
        else if (w == 0 and  d == 1)
        {
            int8_t num1;
            memoryAccesses.push_back("shl (%" + regs_32[rm] + ")");
            string00 = "(%" + regs_32[rm] + ")\n";

            num1 = memories8bit[common.getHex(registers[regs_32[rm]], 0, 0)];
            memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[rm]], 0, 0));
            int num2 = num1 << common.get_bits(1, 5, registers["ECX"]);

            memories8bit[common.getHex(registers[regs_32[rm]], 0, 0)] = (int8_t)num2;
            memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[rm]], 0, 0));

            common.setCarry8bit(num1, num2, registers);
            common.setSign(num2, registers);
            common.setZero(num2, registers);
        }
        else if (w == 1 and d ==0)
        {
            if (opSize)
            {
                memoryAccesses.push_back("shl (%" + regs_32[rm] + ")");
                string00 = "(%" + regs_32[rm] + ")\n";

                int16_t num1;
                num1 = memories16bit[common.getHex(registers[regs_32[rm]], 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[rm]], 0, 0));

                int num2 = num1 << 1;

                memories16bit[common.getHex(registers[regs_32[rm]], 0, 0)] = (int16_t)num2;
                memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[rm]], 0, 0));

                common.setOverflow16bit(num1, num1, num2, registers);
                common.setCarry16bit(num1, num2, registers);
                common.setSign(num2, registers);
                common.setZero(num2, registers);
            }
            else
            {
                memoryAccesses.push_back("shl (%" + regs_32[rm] + ")");

                int num1 = memories32bit[common.getHex(registers[regs_32[rm]], 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[rm]], 0, 0));

                int num2 = num1 << 1;

                common.setOverflow32bit(num1, num1, num2, registers);
                common.setCarry32bit(num1, num2, registers);
                common.setSign(num2, registers);
                common.setZero(num2, registers);

                string00 = "(%" + regs_32[rm] + ")\n";
                memories32bit[common.getHex(registers[regs_32[rm]], 0, 0)] = (int32_t)num2;
                memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[rm]], 0, 0));
            }
        }
        else
        {
            if (opSize)
            {
                memoryAccesses.push_back("shl (%" + regs_32[rm] + ")");
                string00 = "(%" + regs_32[rm] + ")\n";

                int16_t num1;
                num1 = memories16bit[common.getHex(registers[regs_32[rm]], 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[rm]], 0, 0));

                int num2 = num1 << common.get_bits(1, 5, registers["ECX"]);

                memories16bit[common.getHex(registers[regs_32[rm]], 0, 0)] = (int16_t)num2;
                memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[rm]], 0, 0));

                common.setCarry16bit(num1, num2, registers);
                common.setSign(num2, registers);
                common.setZero(num2, registers);
            }
            else
            {
                memoryAccesses.push_back("shl (%" + regs_32[rm] + ")");

                int num1 = memories32bit[common.getHex(registers[regs_32[rm]], 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[rm]], 0, 0));

                int num2 = num1 << common.get_bits(1, 5, registers["ECX"]);

                common.setCarry32bit(num1, num2, registers);
                common.setSign(num2, registers);
                common.setZero(num2, registers);

                string00 = "(%" + regs_32[rm] + ")\n";
                memories32bit[common.getHex(registers[regs_32[rm]], 0, 0)] = (int32_t)num2;
                memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[rm]], 0, 0));
            }
        }
    }
    return string00;
}

string Left_shift::decode_mod_01(int d, int w, int rm)
{
    string string01 = "";
    if (rm == 4)
    {
        string01 = decode_SIB(d, w, 1);
    }
    else
    {
        string01 = decode_displacement_without_SIB(d, w, 1, rm);
    }
    return string01;
}

string Left_shift::decode_mod_10(int d, int w, int rm)
{
    string string10 = "";
    if (rm == 4)
    {
        string10 = decode_SIB(d, w, 2);
    }
    else
    {
        string10 = decode_displacement_without_SIB(d, w, 2, rm);
    }
    return string10;
}

string Left_shift::decode_mod_11(int d, int w, int rm)
{
    string string11 = "";
    int8_t num1;
    if (w == 0 and d == 0)
    {
        int8_t num1;
        int num2;
        if (rm < 4)
        {
            num1 = common.get_bits(1, 8, registers[regs_32[rm]]);
            num2 = num1 << 1;
            registers[regs_32[rm]] = ((registers[regs_32[rm]]) & 0xffffff00) | ((int8_t)num2 & 0x000000ff);
        }
        else
        {
            num1 = common.get_bits(9, 8, registers[regs_32[rm % 4]]);
            num2 = num1 << 1;
            registers[regs_32[rm]] = ((registers[regs_32[rm]]) & 0xffff00ff) | (((int8_t)num2 << 8) & 0x0000ff00);
        }

        common.setOverflow8bit(num1, num1 , num2, registers);
        common.setCarry8bit(num1, num2, registers);
        common.setSign(num2, registers);
        common.setZero(num2, registers);

        string11 = "%" + regs_8[rm] +  "\n";
    }
    else if (w == 0 and d == 1)
    {
        int8_t num1;
        int num2;
        if (rm < 4)
        {
            num1 = common.get_bits(1, 8, registers[regs_32[rm]]);
            num2 = num1 << common.get_bits(1, 5, registers["ECX"]);
            registers[regs_32[rm]] = ((registers[regs_32[rm]]) & 0xffffff00) | ((int8_t)num2 & 0x000000ff);
        }
        else
        {
            num1 = common.get_bits(9, 8, registers[regs_32[rm % 4]]);
            num2 = num1 << common.get_bits(1, 5, registers["ECX"]);
            registers[regs_32[rm]] = ((registers[regs_32[rm]]) & 0xffff00ff) | (((int8_t)num2 << 8) & 0x0000ff00);
        }

        common.setCarry8bit(num1, num2, registers);
        common.setSign(num2, registers);
        common.setZero(num2, registers);

        string11 = "%" + regs_8[rm] +  "\n";
    }
    else if (w == 1 and d ==0)
    {
        if (opSize)
        {
            int16_t num1;
            num1 = common.get_bits(1, 16, registers[regs_32[rm]]);
            int num2 = num1 << 1;

            registers[regs_32[rm]] = ((registers[regs_32[rm]]) & 0xffff0000) | ((int16_t)num2 & 0x0000ffff);

            common.setOverflow16bit(num1, num1 , num2, registers);
            common.setCarry16bit(num1, num2, registers);
            common.setSign(num2, registers);
            common.setZero(num2, registers);

            string11 = "%" + regs_16[rm] + "\n";
        }
        else
        {
            int32_t num1;
            num1 = registers[regs_32[rm]];
            int num2 = num1 << 1;
            registers[regs_32[rm]] = num1;

            common.setOverflow32bit(num1, num1 , num2, registers);
            common.setCarry32bit(num1, num2, registers);
            common.setSign(num2, registers);
            common.setZero(num2, registers);

            string11 = "%" + regs_32[rm] + "\n";
        }
    }
    else
    {
        if (opSize)
        {
            int16_t num1;
            num1 = common.get_bits(1, 16, registers[regs_32[rm]]);
            int num2 = num1 << common.get_bits(1, 5, registers["ECX"]);

            registers[regs_32[rm]] = ((registers[regs_32[rm]]) & 0xffff0000) | ((int16_t)num2 & 0x0000ffff);

            common.setCarry16bit(num1, num2, registers);
            common.setSign(num2, registers);
            common.setZero(num2, registers);

            string11 = "%" + regs_16[rm] + "\n";
        }
        else
        {
            int32_t num1;
            num1 = registers[regs_32[rm]];
            int num2 = num1 << common.get_bits(1, 5, registers["ECX"]);
            registers[regs_32[rm]] = num1;

            common.setCarry32bit(num1, num2, registers);
            common.setSign(num2, registers);
            common.setZero(num2, registers);

            string11 = "%" + regs_32[rm] + "\n";
        }
    }
    return string11;
}

string Left_shift::decode_shl(short prefixes[4])
{
    string decoded_bytes;
    short opCode = instruction.front();
    bool immediate = common.get_bits(5, 1, opCode);

    if (prefixes[3] == 0x67)
    {
        Left_shift_override Left_shift_override(common, instruction, registers, memories32bit, memories16bit, memories8bit, memoryAccesses);
        decoded_bytes = Left_shift_override.decode_shl(prefixes);
        cout << decoded_bytes;
    }
    else if (immediate == 0)
    {
        Left_shift_override_immediate Left_shift_override_immediate(common, instruction, registers, memories32bit, memories16bit, memories8bit, memoryAccesses);
        decoded_bytes = Left_shift_override_immediate.decode_shl(prefixes);
        cout << decoded_bytes;
    }
    else
    {
        short opCode = instruction.front();
        bool w = common.get_bits(1, 1, opCode);
        bool d = common.get_bits(2, 1, opCode);

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
        int reg = common.get_bits(4, 3, modrm);
        int rm = common.get_bits(1, 3, modrm);

        if(reg == 5){
            Right_shift Right_shift(common, instruction, registers, memories32bit, memories16bit, memories8bit, memoryAccesses);
            decoded_bytes = Right_shift.decode_shr(prefixes, d, w);

            cout << "shr " << decoded_bytes;
        }
        else
        {
            instruction.pop();
            registers["EIP"] = registers["EIP"] + 1;
            if (mod == 0)
            {
                decoded_bytes = decode_mod_00(d, w, rm);
            }
            else if (mod == 1)
            {
                decoded_bytes = decode_mod_01(d, w, rm);
            }
            else if (mod == 2)
            {
                decoded_bytes = decode_mod_10(d, w, rm);
            }
            else
            {
                decoded_bytes = decode_mod_11(d, w, rm);
            }

            cout << "shl " << decoded_bytes;
        }
    }

    cout << "EAX: " << hex << registers["EAX"] << "\n";
    cout << "ECX: " << hex << registers["ECX"] << "\n";
    cout << "EDX: " << hex << registers["EDX"] << "\n";
    cout << "EBX: " << hex << registers["EBX"] << "\n";
    cout << "ESP: " << hex << registers["ESP"] << "\n";
    cout << "EBP: " << hex << registers["EBP"] << "\n";
    cout << "ESI: " << hex << registers["ESI"] << "\n";
    cout << "EDI: " << hex << registers["EDI"] << "\n";
    cout << "EFLAGS: " << hex << registers["EFLAGS"] << "\n \n";

    return "SHL/SHR instantiated and done\n";
}