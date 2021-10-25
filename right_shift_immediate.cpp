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

#include "left_right_shift.h"

using namespace std;

Right_shift_immediate::Right_shift_immediate(Common com, queue<short> &instruction, map<string, int> &registers, map<string, int> &memories32bit, map<string, int16_t> &memories16bit, map<string, int8_t> &memories8bit, list<string> &memoryAccesses)
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

string Right_shift_immediate::decode_displacement_with_SIB(int w, int mod, int index, int scale, int base)
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

    int8_t unmaskedImm = common.assemble_bits(1, instruction, registers);
    int imm = common.get_bits(1, 5, unmaskedImm);

    if (mod == 0)
    {
        if (base == 5)
        {
            if (w == 0)
            {
                
                if (index == 4)
                {
                    int8_t num1;
                    int num2;
                    num1 =  memories8bit[st];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + st);
                    num2 = num1 >> imm;

                    memoryAccesses.push_back("shr " +st + ", $" + common.getHex(unmaskedImm, 0, 0));

                    memories8bit[st] = (int8_t)num2;
                    memoryAccesses.push_back("write " + to_string(num2) + " to " + st);

                    dispWithSIB = st + ", $" + common.getHex(unmaskedImm, 0, 0)  + "\n";
                            
                    common.setCarry32bit(num1, num2, registers);
                    common.setSign(num2, registers);
                    common.setZero(num2, registers);
                }
                else
                {
                    int8_t num1;
                    int num2;
                    num1 =  memories8bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));
                    num2 = num1 >> imm;

                    memoryAccesses.push_back("shr " + st + "(,%" + regs_32[index] + "," + to_string(scale) + "), $" + common.getHex(unmaskedImm, 0, 0) );

                    memories8bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)] = (int8_t)num2;
                    memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                    dispWithSIB = st + "(,%" + regs_32[index] + "," + to_string(scale) + "), $" + common.getHex(unmaskedImm, 0, 0)  + "\n";
                  
                    common.setCarry32bit(num1, num2, registers);
                    common.setSign(num2, registers);
                    common.setZero(num2, registers);
                }
            }
            else if (w == 1)
            {
                if (opSize)
                {            
                    if (index == 4)
                    {
                        int16_t num1;
                        int num2;
                        num1 =  memories16bit[st];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + st);
                        num2 = num1 >> imm;

                        memoryAccesses.push_back("shr " + st + ", $" + common.getHex(unmaskedImm, 0, 0));

                        memories16bit[st] = (int16_t)num2;
                        memoryAccesses.push_back("write " + to_string(num2) + " to " + st);

                        dispWithSIB =   st + ", $" + common.getHex(unmaskedImm, 0, 0) +"\n";

                        common.setCarry32bit(num1, num2, registers);
                        common.setSign(num2, registers);
                        common.setZero(num2, registers);
                    }
                    else
                    {
                        int16_t num1;
                        int num2;
                        num1 =  memories16bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));
                        num2 = num1 >> imm;

                        memoryAccesses.push_back("shr " + st + "(,%" + regs_32[index] + "," + to_string(scale) + "), $" + common.getHex(unmaskedImm, 0, 0));

                        memories16bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)] = (int16_t)num2;
                        memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                        dispWithSIB =  st + "(,%" + regs_32[index] + "," + to_string(scale) + "), $" + common.getHex(unmaskedImm, 0, 0) + "\n";
                        
                        common.setCarry32bit(num1, num2, registers);
                        common.setSign(num2, registers);
                        common.setZero(num2, registers);
                    }

                }
                else
                {
                    
                    if (index == 4)
                    {
                        int num1;
                        int num2;
                        num1 =  memories32bit[st];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + st);
                        num2 = num1 >> imm;

                        memoryAccesses.push_back("shr " + st + ", $" + common.getHex(unmaskedImm, 0, 0));

                        memories32bit[st] = num2;
                        memoryAccesses.push_back("write " + to_string(num2) + " to " + st);

                        dispWithSIB =  st + ", $" + common.getHex(unmaskedImm, 0, 0) +"\n";

                        common.setCarry32bit(num1, num2, registers);
                        common.setSign(num2, registers);
                        common.setZero(num2, registers);
                    }
                    else
                    {
                        int num1;
                        int num2;
                        num1 =  memories32bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));
                        num2 = num1 >> imm;
                        memoryAccesses.push_back("shr " + st + "(,%" + regs_32[index] + "," + to_string(scale) + "), $" + common.getHex(unmaskedImm, 0, 0));

                        memories32bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)] = num2;
                        memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                        dispWithSIB =  st + "(,%" + regs_32[index] + "," + to_string(scale) + "), $" + common.getHex(unmaskedImm, 0, 0) + "\n";
                        
                        common.setCarry32bit(num1, num2, registers);
                        common.setSign(num2, registers);
                        common.setZero(num2, registers);
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
                    int8_t num1;
                    int num2;
                    num1 =  memories8bit[common.getHex(registers[regs_32[base]], 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]], 0, 0));
                    num2 = num1 >> imm;

                    memoryAccesses.push_back("shr (%" + regs_32[base] + "), $" + common.getHex(unmaskedImm, 0, 0));

                    memories8bit[common.getHex(registers[regs_32[base]], 0, 0)] = (int8_t)num2;
                    memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]], 0, 0));

                    dispWithSIB =   "(%" + regs_32[base] + "), $" + common.getHex(unmaskedImm, 0, 0) +"\n";
                    
                    common.setCarry32bit(num1, num2, registers);
                    common.setSign(num2, registers);
                    common.setZero(num2, registers);
                }
                else
                {
                    int8_t num1;
                    int num2;
                    num1 = memories8bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0));
                    num2 = num1 >> imm;

                    memoryAccesses.push_back("shr (%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "$" + common.getHex(unmaskedImm, 0, 0));

                    memories8bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0)] = (int8_t)num2;
                    memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0));

                    dispWithSIB =  "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "$" + common.getHex(unmaskedImm, 0, 0) + "\n";
                    
                    common.setCarry32bit(num1, num2, registers);
                    common.setSign(num2, registers);
                    common.setZero(num2, registers);
                }
            }
            else if (w == 1)
            {
                if (opSize)
                {
                    
                    if (index == 4)
                    {
                        int16_t num1;
                        int num2;
                        num1 = memories16bit[common.getHex(registers[regs_32[base]], 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]], 0, 0));
                        num2 = num1 >> imm;

                        memoryAccesses.push_back("shr (%" + regs_32[base] + "), $" + common.getHex(unmaskedImm, 0, 0));

                        memories16bit[common.getHex(registers[regs_32[base]], 0, 0)] = (int16_t)num2;
                        memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]], 0, 0));

                        dispWithSIB =   "(%" + regs_32[base] + "), $" + common.getHex(unmaskedImm, 0, 0) +"\n";
                        
                        common.setCarry32bit(num1, num2, registers);
                        common.setSign(num2, registers);
                        common.setZero(num2, registers);
                    }
                    else
                    {
                        int16_t num1;
                        int num2;
                        num1 = memories16bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));
                        num2 = num1 >> imm;

                        memoryAccesses.push_back("shr (%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "), $" + common.getHex(unmaskedImm, 0, 0));

                        memories16bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)] = (int16_t)num2;
                        memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                        dispWithSIB = "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "), $" + common.getHex(unmaskedImm, 0, 0) + "\n";

                        common.setCarry32bit(num1, num2, registers);
                        common.setSign(num2, registers);
                        common.setZero(num2, registers);
                    }
                }
                else
                {
                    
                    if (index == 4)
                    {
                        int num1;
                        int num2;
                        num1 = memories32bit[common.getHex(registers[regs_32[base]], 0, 0)];
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]], 0, 0));
                        num2 = num1 >> imm;

                        memoryAccesses.push_back("shr (%" + regs_32[base] + ")" +", $" + common.getHex(unmaskedImm, 0, 0) );

                        memories32bit[common.getHex(registers[regs_32[base]], 0, 0)] = num2;
                        memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]], 0, 0));

                        dispWithSIB = "(%" + regs_32[base] + ")" +", $" + common.getHex(unmaskedImm, 0, 0) + "\n";

                        common.setCarry32bit(num1, num2, registers);
                        common.setSign(num2, registers);
                        common.setZero(num2, registers);
                    }
                    else
                    {
                        int num1;
                        int num2;
                        num1 = memories32bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)] ;
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));
                        num2 = num1 >> imm;

                        memoryAccesses.push_back("shr (%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "$" + common.getHex(unmaskedImm, 0, 0));

                        memories32bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)] = num2;
                        memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                        dispWithSIB =  "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "$" + common.getHex(unmaskedImm, 0, 0) + "\n";

                        common.setCarry32bit(num1, num2, registers);
                        common.setSign(num2, registers);
                        common.setZero(num2, registers);
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
                int8_t num1;
                int num2;
                num1 =  memories8bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + disp, 0, 0));
                num2 = num1 >> imm;

                memoryAccesses.push_back("shr " +st + "(%" + regs_32[base] + ")" + ", $" + common.getHex(unmaskedImm, 0, 0));

                memories8bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)] = (int8_t)num2;
                memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                dispWithSIB =  st + "(%" + regs_32[base] + ")" + ", $" + common.getHex(unmaskedImm, 0, 0) + "\n";

                common.setCarry32bit(num1, num2, registers);
                common.setSign(num2, registers);
                common.setZero(num2, registers);
            }
            else
            {
                int8_t num1;
                int num2;
                num1 =  memories8bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));
                num2 = num1 >> imm;

                memoryAccesses.push_back("shr " + st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + ", $" + common.getHex(unmaskedImm, 0, 0));

                memories8bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)] = (int8_t)num2;
                memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                dispWithSIB = st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + ", $" + common.getHex(unmaskedImm, 0, 0) +"\n";

                common.setCarry32bit(num1, num2, registers);
                common.setSign(num2, registers);
                common.setZero(num2, registers);
            }
        }
        else if (w == 1)
        {
            if (opSize)
            {               
                if (index == 4)
                {
                    int16_t num1;
                    int num2;
                    num1 = memories16bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + disp, 0, 0));
                    num2 = num1 >> imm;

                    memoryAccesses.push_back("shr " + st + "(%" + regs_32[base] + ")" + ", $" + common.getHex(unmaskedImm, 0, 0));

                    memories16bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)] = (int16_t)num2;
                    memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                    dispWithSIB =   st + "(%" + regs_32[base] + ")" + ", $" + common.getHex(unmaskedImm, 0, 0) +"\n";

                    common.setCarry32bit(num1, num2, registers);
                    common.setSign(num2, registers);
                    common.setZero(num2, registers);
                }
                else
                {
                    int16_t num1;
                    int num2;
                    num1 = memories16bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));
                    num2 = num1 >> imm;

                    memoryAccesses.push_back("shr " +  st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" +", $" + common.getHex(unmaskedImm, 0, 0) );

                    memories16bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)] = (int16_t)num2;
                    memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                    dispWithSIB =  st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" +", $" + common.getHex(unmaskedImm, 0, 0) +"\n";

                    common.setCarry32bit(num1, num2, registers);
                    common.setSign(num2, registers);
                    common.setZero(num2, registers);
                }
            }
            else
            {               
                if (index == 4)
                {
                    int num1;
                    int num2;
                    num1 = memories32bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + disp, 0, 0));
                    num2 = num1 >> imm;

                    memoryAccesses.push_back("shr " + st + "(%" + regs_32[base] + ")" + ", $" + common.getHex(unmaskedImm, 0, 0));

                    memories32bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)] = num2;
                    memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                    dispWithSIB =   st + "(%" + regs_32[base] + ")" + ", $" + common.getHex(unmaskedImm, 0, 0) +"\n";

                    common.setCarry32bit(num1, num2, registers);
                    common.setSign(num2, registers);
                    common.setZero(num2, registers);
                }
                else
                {
                    int num1;
                    int num2;
                    num1 = memories32bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));
                    num2 = num1 >> imm;
                    memoryAccesses.push_back("shr " +  st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + ", $" + common.getHex(unmaskedImm, 0, 0));

                    memories32bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)] = num2;
                    memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                    dispWithSIB =   st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + ", $" + common.getHex(unmaskedImm, 0, 0) +"\n";

                    common.setCarry32bit(num1, num2, registers);
                    common.setSign(num2, registers);
                    common.setZero(num2, registers);
                }
            }
        }
    }
    return dispWithSIB;
}

string Right_shift_immediate::decode_displacement_without_SIB(int w, int mod, int rm)
{
    string dispWithoutSIB = "";
    int disp_bytes[] = {4, 1, 4};
    int bytes = disp_bytes[mod];

    unsigned int disp = common.assemble_bits(bytes, instruction, registers);
    string st = common.getHex(disp, 0, 0);

    int8_t unmaskedImm = common.assemble_bits(1, instruction, registers);
    int imm = common.get_bits(1, 5, unmaskedImm);

    if (mod == 0)
    {
        if (w == 0)
        {
            int8_t num1;
            int num2;
            num1 =  memories8bit[st];
            memoryAccesses.push_back("Read " + to_string(num1) + " from " + st);
            num2 = num1 >> imm;


            memoryAccesses.push_back("shr " + st + ", $" + common.getHex(unmaskedImm, 0, 0));

            memories8bit[st] = (int8_t)num2;
            memoryAccesses.push_back("write " + to_string(num2) + " to " + st);

            dispWithoutSIB = st + ", $" + common.getHex(unmaskedImm, 0, 0) + "\n";

            common.setCarry32bit(num1, num2, registers);
            common.setSign(num2, registers);
            common.setZero(num2, registers);
        }
        else if (w == 1)
        {
            if (opSize)
            {
                int16_t num1;
                int num2;
                num1 =  memories16bit[st];
                memoryAccesses.push_back("Read " + to_string(num1) + " from " + st);
                num2 = num1 >> imm;

                memoryAccesses.push_back("shr " +st + ", $" + common.getHex(unmaskedImm, 0, 0));

                memories16bit[st] = (int16_t)num2;
                memoryAccesses.push_back("write " + to_string(num2) + " to " + st);

                dispWithoutSIB =  st + ", $" + common.getHex(unmaskedImm, 0, 0) +"\n";

                common.setCarry32bit(num1, num2, registers);
                common.setSign(num2, registers);
                common.setZero(num2, registers);

            }
            else
            {
                int num1;
                int num2;
                num1 =  memories32bit[st];
                memoryAccesses.push_back("Read " + to_string(num1) + " from " + st);
                num2 = num1 >> imm;
                memoryAccesses.push_back("shr " + st + ", $" + common.getHex(unmaskedImm, 0, 0));

                memories32bit[st] = num2;
                memoryAccesses.push_back("write " + to_string(num2) + " to " + st);

                dispWithoutSIB =  st + ", $" + common.getHex(unmaskedImm, 0, 0) +"\n";

                common.setCarry32bit(num1, num2, registers);
                common.setSign(num2, registers);
                common.setZero(num2, registers);
            }
        }
    }
    else
    {
        if (w == 0)
        {
            int8_t num1;
            int num2;
            num1 =  memories8bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)];
            memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));
            num2 = num1 >> imm;

            memoryAccesses.push_back("shr " + st + "(%" + regs_32[rm] + ")"+", $" + common.getHex(unmaskedImm, 0, 0));

            memories8bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)] = (int8_t)num2;
            memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

            dispWithoutSIB =   st + "(%" + regs_32[rm] + ")"+", $" + common.getHex(unmaskedImm, 0, 0) +" \n";

            common.setCarry32bit(num1, num2, registers);
            common.setSign(num2, registers);
            common.setZero(num2, registers);
        }
        else if (w == 1)
        {
            if (opSize)
            {
                int16_t num1;
                int num2;
                num1 =  memories16bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));
                num2 = num1 >> imm;

                memoryAccesses.push_back("add " +  st + "(%" + regs_32[rm] + ") "+", $" + common.getHex(unmaskedImm, 0, 0));

                memories16bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)] = (int16_t)num2;
                memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

                dispWithoutSIB =    st + "(%" + regs_32[rm] + ") "+", $" + common.getHex(unmaskedImm, 0, 0)+"\n";

                common.setCarry32bit(num1, num2, registers);
                common.setSign(num2, registers);
                common.setZero(num2, registers);
            }
            else
            {
                int num1;
                int num2;
                num1 =  memories32bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));
                num2 = num1 >> imm;

                memories32bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)] = num2;
                memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

                dispWithoutSIB =  st + "(%" + regs_32[rm] + ")"+", $" + common.getHex(unmaskedImm, 0, 0) + " \n";

                common.setCarry32bit(num1, num2, registers);
                common.setSign(num2, registers);
                common.setZero(num2, registers);
            }
        }
    }
    return dispWithoutSIB;
};

string Right_shift_immediate::decode_SIB(int w, int mod)
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

string Right_shift_immediate::decode_mod_00(int w, int rm)
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
        int8_t unmaskedImm = common.assemble_bits(1, instruction, registers);
        int imm = common.get_bits(1, 5, unmaskedImm);
        if (w == 0)
        {
            int8_t num1;
            int num2;
            num1 =  memories8bit[common.getHex(registers[regs_32[rm]], 0, 0)];
            memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[rm]], 0, 0));
            num2 = num1 >> imm;

            memoryAccesses.push_back("shr (%" + regs_32[rm] + "), $" + common.getHex(unmaskedImm, 0, 0));
            string00 =   "(%" + regs_32[rm] + "), $" + common.getHex(unmaskedImm, 0, 0)+"\n";

            memories8bit[common.getHex(registers[regs_32[rm]], 0, 0)] = (int8_t)num2;
            memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[rm]], 0, 0));

            common.setCarry32bit(num1, num2, registers);
            common.setSign(num2, registers);
            common.setZero(num2, registers);
        }
        else if (w == 1)
        {
            if (opSize)
            {
                int16_t num1;
                int num2;
                num1 =  memories16bit[common.getHex(registers[regs_32[rm]], 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[rm]], 0, 0));
                num2 = num1 >> imm;

                memoryAccesses.push_back("shr (%" + regs_32[rm] + "), $" + common.getHex(unmaskedImm, 0, 0));

                string00 =  "(%" + regs_32[rm] + "), $" + common.getHex(unmaskedImm, 0, 0) +"\n";
                memories16bit[common.getHex(registers[regs_32[rm]], 0, 0)] = (int16_t)num2;
                memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[rm]], 0, 0));

                common.setCarry32bit(num1, num2, registers);
                common.setSign(num2, registers);
                common.setZero(num2, registers);
            }
            else
            {
                int num1;
                int num2;
                num1 =  memories32bit[common.getHex(registers[regs_32[rm]], 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[rm]], 0, 0));
                num2 = num1 >> imm;
                memoryAccesses.push_back("shr (%" + regs_32[rm] + "), $" + common.getHex(unmaskedImm, 0, 0));

                string00 =  "(%" + regs_32[rm] + "), $" + common.getHex(unmaskedImm, 0, 0) +"\n";
                memories32bit[common.getHex(registers[regs_32[rm]], 0, 0)] = num2;
                memoryAccesses.push_back("write " + to_string(num2) + " to " + common.getHex(registers[regs_32[rm]], 0, 0));

                common.setCarry32bit(num1, num2, registers);
                common.setSign(num2, registers);
                common.setZero(num2, registers);
            }
        }
    }

    return string00;
}

string Right_shift_immediate::decode_mod_01(int w, int rm)
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

string Right_shift_immediate::decode_mod_10(int w, int rm)
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

string Right_shift_immediate::decode_mod_11(int w, int rm)
{
    int8_t unmaskedImm = common.assemble_bits(1, instruction, registers);
    int imm = common.get_bits(1, 5, unmaskedImm);

    string string11 = "";
    if (w == 0)
    {
        int8_t num1;
        int num2;
        num1 = common.get_bits(1, 8, registers[regs_32[rm]]);
        num2 = num1 >> imm;
        if (rm < 4)
        {
            registers[regs_32[rm]] = ((registers[regs_32[rm]]) & 0xffffff00) | ((int8_t)num2 & 0x000000ff);
        }
        else
        {
            registers[regs_32[rm]] = ((registers[regs_32[rm]]) & 0xffff00ff) | (((int8_t)num2 << 8) & 0x0000ff00);
        }
        common.setCarry32bit(num1, num2, registers);
        common.setSign(num2, registers);
        common.setZero(num2, registers);

        string11 = "%" + regs_8[rm] + ",$" + common.getHex(unmaskedImm, 0, 0)+"\n";
    }
    else if (w == 1)
    {
        if (opSize)
        {
            int16_t num1;
            int num2;
            num1 = common.get_bits(1, 16, registers[regs_32[rm]]);
            num2 = num1 >> imm;
            registers[regs_32[rm]] = ((registers[regs_32[rm]]) & 0xffff0000) | ((int16_t)num2 & 0x0000ffff);

            string11 = "%" + regs_16[rm] + ",$" + common.getHex(unmaskedImm, 0, 0)+"\n";
            common.setCarry32bit(num1, num2, registers);
            common.setSign(num2, registers);
            common.setZero(num2, registers);
        }
        else
        {
            int num1;
            int num2;
            num1 = registers[regs_32[rm]];
            num2 = num1 >> imm;
            registers[regs_32[rm]] = num2;

            string11 = "%" + regs_32[rm] + ",$" + common.getHex(unmaskedImm, 0, 0) + "\n";
            common.setCarry32bit(num1, num2, registers);
            common.setSign(num2, registers);
            common.setZero(num2, registers);
        }
    }

    return string11;
}

string Right_shift_immediate::decode_shr(short prefixes[4], int w, int mod, int rm)
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
    return "shr" + decoded_bytes;
}
