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

#include "cmp_immediate.h"

using namespace std;

Cmp_immediate::Cmp_immediate(Common com, queue<short> &instruction, map<string, int> &registers, map<string, int> &memories32bit, map<string, int16_t> &memories16bit, map<string, int8_t> &memories8bit, list<string> &memoryAccesses)
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

string Cmp_immediate::decode_displacement_with_SIB(int w, int s, int mod, int index, int scale, int base)
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
                int8_t num1 = common.assemble_bits(1, instruction, registers);
                if (index == 4)
                {
                    memoryAccesses.push_back("cmp " +st + ", $" + common.getHex(num1, 0, 0));

                    int8_t num2, num3;
                    uint8_t num4;

                    num2 = memories8bit[st];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + st);

                    num3 = num1 - num2;
                    num4 = unsigned(num3);

                    memoryAccesses.push_back("write " + to_string(num3) + " to " + st);

                    common.setOverflow8bit(num1, num2, num3, registers);
                    common.setCarry8bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    dispWithSIB = st + ", $" + common.getHex(num1, 0, 0)  + "\n";
                }
                else
                {
                    memoryAccesses.push_back("cmp " + st + "(,%" + regs_32[index] + "," + to_string(scale) + "), $" + common.getHex(num1, 0, 0) );

                    int8_t num1, num2, num3;
                    uint8_t num4;

                    num2 = memories8bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                    num3 = num1 - num2;
                    num4 = unsigned(num3);

                    memories8bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)] = num3;
                    memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                    common.setOverflow8bit(num1, num2, num3, registers);
                    common.setCarry8bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    dispWithSIB = st + "(,%" + regs_32[index] + "," + to_string(scale) + "), $" + common.getHex(num1, 0, 0)  + "\n";
                }
            }
            else if (w == 1)
            {
                if (opSize)
                {
                    if (s == 0)
                    {
                        int16_t num1 = common.assemble_bits(2, instruction, registers);
                        if (index == 4)
                        {
                            memoryAccesses.push_back("cmp " + st + ", $" + common.getHex(num1, 0, 0));

                            int16_t num2, num3;
                            uint16_t num4;

                            num2 = memories16bit[st];         // R/M
                            memoryAccesses.push_back("Read " + to_string(num2) + " from " + st);

                            num3 = num2 - num1;
                            num4 = unsigned(num3);

                            if (num3 > 0) {
                                cout << "First operand is greater than second operand " << endl; 
                            }
                            else {
                                cout << "First operand is less than second operand " << endl;
                            }

                            memoryAccesses.push_back("write " + to_string(num3) + " to " + st);

                            common.setOverflow16bit(num1, num2, num3, registers);
                            common.setCarry16bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            dispWithSIB =   st + ", $" + common.getHex(num1, 0, 0) +"\n";
                        }
                        else
                        {
                            memoryAccesses.push_back("cmp " + st + "(,%" + regs_32[index] + "," + to_string(scale) + "), $" + common.getHex(num1, 0, 0));

                            int16_t num2, num3;
                            uint16_t num4;

                            num2 = memories16bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)];  // R/M bit
                            memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                            num3 = num2 - num1;
                            num4 = unsigned(num3);

                            if (num3 > 0) {
                                cout << "First operand is greater than second operand " << endl; 
                            }
                            else {
                                cout << "First operand is less than second operand " << endl;
                            }
                            memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                            common.setOverflow16bit(num1, num2, num3, registers);
                            common.setCarry16bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            dispWithSIB =  st + "(,%" + regs_32[index] + "," + to_string(scale) + "), $" + common.getHex(num1, 0, 0) + "\n";
                        }
                    }
                    else if (s == 1)
                    {
                        int8_t num = common.assemble_bits(1, instruction, registers);
                        if (index == 4)
                        {
                            int16_t num1, num2, num3;
                            uint16_t num4;

                            num1 = num;  // Reg bit 
                            num2 = memories16bit[st];   //  R/M bit 
                            memoryAccesses.push_back("Read " + to_string(num2) + " from " + st);

                            num3 = num1 - num2;
                            num4 = unsigned(num3);

                            memoryAccesses.push_back("write " + to_string(num3) + " to " + st);

                            if (num3 > 0) {
                                cout << "First operand is greater than second operand " << endl; 
                            }
                            else {
                                cout << "First operand is less than second operand " << endl;
                            }

                            common.setOverflow16bit(num1, num2, num3, registers);
                            common.setCarry16bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            dispWithSIB =  st + ", $" + common.getHex(num, 4, 'f') + "\n";
                        }
                        else
                        {
                            int16_t num1, num2, num3;
                            uint16_t num4;

                            num1 = num;  // Reg bit 
                            num2 = memories16bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)]; // R/M bit
                            memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                            num3 = num1 - num2;
                            num4 = unsigned(num3);

                            if (num3 > 0) {
                                cout << "First operand is greater than second operand " << endl; 
                            }
                            else {
                                cout << "First operand is less than second operand " << endl;
                            }                            
                         
                            memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                            common.setOverflow16bit(num1, num2, num3, registers);
                            common.setCarry16bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            dispWithSIB =   st + "(,%" + regs_32[index] + "," + to_string(scale) + "), $" + common.getHex(num, 4, 'f') +"\n";
                        }
                    }
                }
                else
                {
                    if (s == 0)
                    {
                        int num2 = common.assemble_bits(4, instruction, registers);  // Reg bit
                        if (index == 4)
                        {
                            memoryAccesses.push_back("cmp " + st + ", $" + common.getHex(num2, 0, 0));

                            int num1 = memories32bit[st];  // R/M bit 
                            memoryAccesses.push_back("Read " + to_string(num1) + " from " + st);

                            int num3 = num1 - num2;
                            unsigned int num4 = unsigned(num3);

                            if (num3 > 0) {
                                cout << "First operand is greater than second operand " << endl; 
                            }
                            else {
                                cout << "First operand is less than second operand " << endl;
                            }

                            common.setOverflow32bit(num1, num2, num3, registers);
                            common.setCarry32bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            memoryAccesses.push_back("write " + to_string(num3) + " to " + st);

                            dispWithSIB =  st + ", $" + common.getHex(num2, 0, 0) +"\n";
                        }
                        else
                        {
                            memoryAccesses.push_back("cmp " + st + "(,%" + regs_32[index] + "," + to_string(scale) + "), $" + common.getHex(num2, 0, 0));

                            int num1 = memories32bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)];  //    R/M 
                            memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                            int num3 = num1 - num2;
                            unsigned int num4 = unsigned(num3);

                            if (num3 > 0) {
                                cout << "First operand is greater than second operand " << endl; 
                            }
                            else {
                                cout << "First operand is less than second operand " << endl;
                            }

                            common.setOverflow32bit(num1, num2, num3, registers);
                            common.setCarry32bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                            dispWithSIB =  st + "(,%" + regs_32[index] + "," + to_string(scale) + "), $" + common.getHex(num2, 0, 0) + "\n";
                        }
                    }
                    else
                    {
                        int8_t num = common.assemble_bits(1, instruction, registers);  // Reg
                        if (index == 4)
                        {
                            int num1 = memories32bit[st];  // R/M
                            memoryAccesses.push_back("Read " + to_string(num1) + " from " + st);

                            int num2 = num;
                            int num3 = num2 - num1;
                            unsigned int num4 = unsigned(num3);

                            if (num3 > 0) {
                                cout << "First operand is greater than second operand " << endl; 
                            }
                            else {
                                cout << "First operand is less than second operand " << endl;
                            }

                            common.setOverflow32bit(num1, num2, num3, registers);
                            common.setCarry32bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            memoryAccesses.push_back("write " + to_string(num3) + " to " + st);

                            dispWithSIB =  st + ", $" + common.getHex(num, 8, 'f') +"\n";
                        }
                        else
                        {

                            int num1 = memories32bit[common.getHex(registers[regs_32[index]] * scale + disp, 0, 0)];  // R/M
                            memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                            int num2 = num;  // Reg
                            int num3 = num2 - num1;
                            unsigned int num4 = unsigned(num3);

                            if (num3 > 0) {
                                cout << "First operand is greater than second operand " << endl; 
                            }
                            else {
                                cout << "First operand is less than second operand " << endl;
                            }

                            common.setOverflow32bit(num1, num2, num3, registers);
                            common.setCarry32bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[index]] * scale + disp, 0, 0));

                            dispWithSIB =   st + "(,%" + regs_32[index] + "," + to_string(scale) + ")"+"$" + common.getHex(num, 8, 'f') +" \n";
                        }
                    }
                }
            }
        }
        else
        {
            if (w == 0)
            {
                int8_t num1 = common.assemble_bits(1, instruction, registers);
                if (index == 4)
                {
                    memoryAccesses.push_back("cmp (%" + regs_32[base] + "), $" + common.getHex(num1, 0, 0));

                    int8_t num2, num3;
                    uint8_t num4;

                    num2 = memories8bit[common.getHex(registers[regs_32[base]], 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[base]], 0, 0));

                    num3 = num1 - num2;
                    num4 = unsigned(num3);

                    memories8bit[common.getHex(registers[regs_32[base]], 0, 0)] = num3;
                    memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[base]], 0, 0));

                    common.setOverflow8bit(num1, num2, num3, registers);
                    common.setCarry8bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    dispWithSIB =   "(%" + regs_32[base] + "), $" + common.getHex(num1, 0, 0) +"\n";
                }
                else
                {
                    memoryAccesses.push_back("cmp (%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "$" + common.getHex(num1, 0, 0));

                    int8_t num2, num3;
                    uint8_t num4;

                    num2 = memories8bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0)];
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0));

                    num3 = num1 - num2;
                    num4 = unsigned(num3);

                    memories8bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0)] = num3;
                    memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * 2 + disp, 0, 0));

                    common.setOverflow8bit(num1, num2, num3, registers);
                    common.setCarry8bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    dispWithSIB =  "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "$" + common.getHex(num1, 0, 0) + "\n";
                }
            }
            else if (w == 1)
            {
                if (opSize)
                {
                    if (s == 0)
                    {
                        int16_t num1 = common.assemble_bits(2, instruction, registers);  // Reg
                        if (index == 4)
                        {
                            memoryAccesses.push_back("cmp (%" + regs_32[base] + "), $" + common.getHex(num1, 0, 0));

                            int16_t num2, num3;
                            uint16_t num4;

                            num2 = memories16bit[common.getHex(registers[regs_32[base]], 0, 0)];  // R/M
                            memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[base]], 0, 0));

                            num3 = num2 - num1;
                            num4 = unsigned(num3);

                            if (num3 > 0) {
                                cout << "First operand is greater than second operand " << endl; 
                            }
                            else {
                                cout << "First operand is less than second operand " << endl;
                            }

                            memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[base]], 0, 0));

                            common.setOverflow16bit(num1, num2, num3, registers);
                            common.setCarry16bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            dispWithSIB =   "(%" + regs_32[base] + "), $" + common.getHex(num1, 0, 0) +"\n";
                        }
                        else
                        {
                            memoryAccesses.push_back("cmp (%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "), $" + common.getHex(num1, 0, 0));

                            int16_t num2, num3;
                            uint16_t num4;

                            num2 = memories16bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)];  // R/M
                            memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                            num3 = num2 - num1;
                            num4 = unsigned(num3);

                            if (num3 > 0) {
                                cout << "First operand is greater than second operand " << endl; 
                            }
                            else {
                                cout << "First operand is less than second operand " << endl;
                            }

                            memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                            common.setOverflow16bit(num1, num2, num3, registers);
                            common.setCarry16bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            dispWithSIB = "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "), $" + common.getHex(num1, 0, 0) + "\n";
                        }
                    }
                    else if (s == 1)
                    {
                        int8_t num = common.assemble_bits(1, instruction, registers);   // Reg
                        if (index == 4)
                        {

                            int16_t num1, num2, num3;
                            uint16_t num4;

                            num1 = num;  // Reg
                            num2 = memories16bit[common.getHex(registers[regs_32[base]], 0, 0)]; // R/M
                            memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[base]], 0, 0));

                            num3 = num1 - num2;
                            num4 = unsigned(num3);

                            if (num3 > 0) {
                                cout << "First operand is greater than second operand " << endl; 
                            }
                            else {
                                cout << "First operand is less than second operand " << endl;
                            }

                            memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[base]], 0, 0));

                            common.setOverflow16bit(num1, num2, num3, registers);
                            common.setCarry16bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            dispWithSIB =  "(%" + regs_32[base] + ")" + ", $" + common.getHex(num, 4, 'f') +"\n";
                        }
                        else
                        {

                            int16_t num1, num2, num3;
                            uint16_t num4;

                            num1 = num;  // Reg
                            num2 = memories16bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)]; // R/M
                            memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                            num3 = num1 - num2;
                            num4 = unsigned(num3);

                            if (num3 > 0) {
                                cout << "First operand is greater than second operand " << endl; 
                            }
                            else {
                                cout << "First operand is less than second operand " << endl;
                            }

                            memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                            common.setOverflow16bit(num1, num2, num3, registers);
                            common.setCarry16bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            dispWithSIB =  "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + ", $" + common.getHex(num, 4, 'f') +"\n";
                        }
                    }
                }
                else
                {
                    if (s == 0)
                    {
                        int num2 = common.assemble_bits(4, instruction, registers);   // Reg
                        if (index == 4)
                        {
                            memoryAccesses.push_back("cmp (%" + regs_32[base] + ")" +", $" + common.getHex(num2, 0, 0) );

                            int num1 = memories32bit[common.getHex(registers[regs_32[base]], 0, 0)];  // R/M
                            memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]], 0, 0));

                            int num3 = num1 - num2;
                            unsigned int num4 = unsigned(num3);

                            if (num3 > 0) {
                                cout << "First operand is greater than second operand " << endl; 
                            }
                            else {
                                cout << "First operand is less than second operand " << endl;
                            }

                            common.setOverflow32bit(num1, num2, num3, registers);
                            common.setCarry32bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[base]], 0, 0));

                            dispWithSIB = "(%" + regs_32[base] + ")" +", $" + common.getHex(num2, 0, 0) + "\n";
                        }
                        else
                        {
                            memoryAccesses.push_back("cmp (%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "$" + common.getHex(num2, 0, 0));

                            int num1 = memories32bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)]; // R/M
                            memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                            int num3 = num1 - num2;
                            unsigned int num4 = unsigned(num3);

                            if (num3 > 0) {
                                cout << "First operand is greater than second operand " << endl; 
                            }
                            else {
                                cout << "First operand is less than second operand " << endl;
                            }

                            common.setOverflow32bit(num1, num2, num3, registers);
                            common.setCarry32bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                            dispWithSIB =  "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "$" + common.getHex(num2, 0, 0) + "\n";
                        }
                    }
                    else
                    {
                        int8_t num = common.assemble_bits(1, instruction, registers); // Reg
                        if (index == 4)
                        {
                            int num1 = memories32bit[common.getHex(registers[regs_32[base]], 0, 0)];  // R/M
                            memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]], 0, 0));

                            int num2 = num; // Reg
                            int num3 = num2 - num1;
                            unsigned int num4 = unsigned(num3);

                            if (num3 > 0) {
                                cout << "First operand is greater than second operand " << endl; 
                            }
                            else {
                                cout << "First operand is less than second operand " << endl;
                            }

                            common.setOverflow32bit(num1, num2, num3, registers);
                            common.setCarry32bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[base]], 0, 0));

                            dispWithSIB =  "(%" + regs_32[base] + ")" + ", $" + common.getHex(num, 8, 'f') +"\n";
                        }
                        else
                        {
                            int num1 = memories32bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)];  // R/M
                            memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                            int num2 = num;  // Reg
                            int num3 = num2 - num1;
                            unsigned int num4 = unsigned(num3);

                            if (num3 > 0) {
                                cout << "First operand is greater than second operand " << endl; 
                            }
                            else {
                                cout << "First operand is less than second operand " << endl;
                            }

                            common.setOverflow32bit(num1, num2, num3, registers);
                            common.setCarry32bit(num1, num4, registers);
                            common.setSign(num3, registers);
                            common.setZero(num3, registers);

                            memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                            dispWithSIB = "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" +", $" + common.getHex(num, 8, 'f') +  "\n";
                        }
                    }
                }
            }
        }
        //printf("mod equals 0");
    }
    else
    {
        //printf("mod not equals 0");

        if (w == 0)
        {
            int8_t num1 = common.assemble_bits(1, instruction, registers);
            if (index == 4)
            {
                memoryAccesses.push_back("cmp " +st + "(%" + regs_32[base] + ")" + ", $" + common.getHex(num1, 0, 0));

                int8_t num2, num3;
                uint8_t num4;

                num2 = memories8bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                num3 = num1 - num2;
                num4 = unsigned(num3);

                memories8bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)] = num3;
                memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                common.setOverflow8bit(num1, num2, num3, registers);
                common.setCarry8bit(num1, num4, registers);
                common.setSign(num3, registers);
                common.setZero(num3, registers);

                dispWithSIB =  st + "(%" + regs_32[base] + ")" + ", $" + common.getHex(num1, 0, 0) + "\n";
            }
            else
            {
                memoryAccesses.push_back("cmp " + st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + ", $" + common.getHex(num1, 0, 0));

                int8_t num2, num3;
                uint8_t num4;

                num2 = memories8bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)];
                memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                num3 = num1 - num2;
                num4 = unsigned(num3);

                memories8bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)] = num3;
                memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                common.setOverflow8bit(num1, num2, num3, registers);
                common.setCarry8bit(num1, num4, registers);
                common.setSign(num3, registers);
                common.setZero(num3, registers);

                dispWithSIB = st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + ", $" + common.getHex(num1, 0, 0) +"\n";
            }
        }
        else if (w == 1)
        {
            if (opSize)
            {
                if (s == 0)
                {
                    int16_t num1 = common.assemble_bits(2, instruction, registers);  // Reg
                    if (index == 4)
                    {
                        memoryAccesses.push_back("cmp " + st + "(%" + regs_32[base] + ")" + ", $" + common.getHex(num1, 0, 0));

                        int16_t num2, num3;
                        uint16_t num4;

                        num2 = memories16bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)];  // R/M
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                        num3 = num2 - num1;
                        num4 = unsigned(num3);

                        if (num3 > 0) {
                            cout << "First operand is greater than second operand " << endl; 
                        }
                        else {
                            cout << "First operand is less than second operand " << endl;
                        }

                        memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                        common.setOverflow16bit(num1, num2, num3, registers);
                        common.setCarry16bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        dispWithSIB =   st + "(%" + regs_32[base] + ")" + ", $" + common.getHex(num1, 0, 0) +"\n";
                    }
                    else
                    {
                        memoryAccesses.push_back("cmp " +  st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" +", $" + common.getHex(num1, 0, 0) );

                        int16_t num2, num3;
                        uint16_t num4;

                        num2 = memories16bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)]; // R/M
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                        num3 = num2 - num1;
                        num4 = unsigned(num3);

                        if (num3 > 0) {
                            cout << "First operand is greater than second operand " << endl; 
                        }
                        else {
                            cout << "First operand is less than second operand " << endl;
                        }

                        memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                        common.setOverflow16bit(num1, num2, num3, registers);
                        common.setCarry16bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        dispWithSIB =  st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" +", $" + common.getHex(num1, 0, 0) +"\n";
                    }
                }
                else if (s == 1)
                {
                    int8_t num = common.assemble_bits(1, instruction, registers); // Reg
                    if (index == 4)
                    {
                        int16_t num1, num2, num3;
                        uint16_t num4;

                        num1 = num; // Reg
                        num2 = memories16bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)]; // R/M
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                        num3 = num1 - num2;
                        num4 = unsigned(num3);

                        if (num3 > 0) {
                            cout << "First operand is greater than second operand " << endl; 
                        }
                        else {
                            cout << "First operand is less than second operand " << endl;
                        }

                        memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                        common.setOverflow16bit(num1, num2, num3, registers);
                        common.setCarry16bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        dispWithSIB =  st + "(%" + regs_32[base] + ")" + ", $" + common.getHex(num, 4, 'f') +"\n";
                    }
                    else
                    {
                        int16_t num1, num2, num3;
                        uint16_t num4;

                        num1 = num; // Reg
                        num2 = memories16bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)]; // R/M
                        memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                        num3 = num1 - num2;
                        num4 = unsigned(num3);

                        if (num3 > 0) {
                            cout << "First operand is greater than second operand " << endl; 
                        }
                        else {
                            cout << "First operand is less than second operand " << endl;
                        }

                        memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                        common.setOverflow16bit(num1, num2, num3, registers);
                        common.setCarry16bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        dispWithSIB =  st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + ", $" + common.getHex(num, 4, 'f') +"\n";
                    }
                }
            }
            else
            {
                if (s == 0)
                {
                    int num2 = common.assemble_bits(4, instruction, registers);  // Reg
                    if (index == 4)
                    {
                        memoryAccesses.push_back("cmp " + st + "(%" + regs_32[base] + ")" + ", $" + common.getHex(num2, 0, 0));

                        int num1 = memories32bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)];  // R/M
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                        int num3 = num1 - num2;
                        unsigned int num4 = unsigned(num3);

                        common.setOverflow32bit(num1, num2, num3, registers);
                        common.setCarry32bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        if (num3 > 0) {
                            cout << "First operand is greater than second operand " << endl; 
                        }
                        else {
                            cout << "First operand is less than second operand " << endl;
                        }

                        memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                        dispWithSIB =   st + "(%" + regs_32[base] + ")" + ", $" + common.getHex(num2, 0, 0) +"\n";
                    }
                    else
                    {
                        memoryAccesses.push_back("cmp " +  st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + ", $" + common.getHex(num2, 0, 0));

                        int num1 = memories32bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)];  // R/M
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                        int num3 = num1 - num2;
                        unsigned int num4 = unsigned(num3);

                        common.setOverflow32bit(num1, num2, num3, registers);
                        common.setCarry32bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        if (num3 > 0) {
                            cout << "First operand is greater than second operand " << endl; 
                        }
                        else {
                            cout << "First operand is less than second operand " << endl;
                        }

                        memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                        dispWithSIB =   st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + ", $" + common.getHex(num2, 0, 0) +"\n";
                    }
                }
                else
                {
                    int8_t num = common.assemble_bits(1, instruction, registers);  // Reg
                    if (index == 4)
                    {
                        int num1 = memories32bit[common.getHex(registers[regs_32[base]] + disp, 0, 0)];  // R/M
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                        int num2 = num;  // Reg
                        int num3 = num2 - num1;
                        unsigned int num4 = unsigned(num3);

                        if (num3 > 0) {
                            cout << "First operand is greater than second operand " << endl; 
                        }
                        else {
                            cout << "First operand is less than second operand " << endl;
                        }

                        common.setOverflow32bit(num1, num2, num3, registers);
                        common.setCarry32bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[base]] + disp, 0, 0));

                        dispWithSIB =  st + "(%" + regs_32[base] + ")" + ", $" + common.getHex(num, 8, 'f') + "\n";
                    }
                    else
                    {
                        int num1 = memories32bit[common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0)];  // R/M
                        memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                        int num2 = num; // Reg
                        int num3 = num2 - num1;
                        unsigned int num4 = unsigned(num3);

                        if (num3 > 0) {
                            cout << "First operand is greater than second operand " << endl; 
                        }
                        else {
                            cout << "First operand is less than second operand " << endl;
                        }

                        common.setOverflow32bit(num1, num2, num3, registers);
                        common.setCarry32bit(num1, num4, registers);
                        common.setSign(num3, registers);
                        common.setZero(num3, registers);

                        memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[base]] + registers[regs_32[index]] * scale + disp, 0, 0));

                        dispWithSIB =  st + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + ", $" + common.getHex(8, num, 'f') + "\n";
                    }
                }
            }
        }
    }
    return dispWithSIB;
}

string Cmp_immediate::decode_displacement_without_SIB(int w, int s, int mod, int rm)
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
            int8_t num1 = common.assemble_bits(1, instruction, registers);
            memoryAccesses.push_back("cmp " + st + ", $" + common.getHex(num1, 0, 0));

            int8_t num2, num3;
            uint8_t num4;

            num2 = memories8bit[st];
            memoryAccesses.push_back("Read " + to_string(num2) + " from " + st);

            num3 = num1 - num2;
            num4 = unsigned(num3);

            memories8bit[st] = num3;
            memoryAccesses.push_back("write " + to_string(num3) + " to " + st);

            common.setOverflow8bit(num1, num2, num3, registers);
            common.setCarry8bit(num1, num4, registers);
            common.setSign(num3, registers);
            common.setZero(num3, registers);

            dispWithoutSIB = st + ", $" + common.getHex(num1, 0, 0) + "\n";
        }
        else if (w == 1)
        {
            if (opSize)
            {
                if (s == 0)
                {
                    int16_t num1 = common.assemble_bits(2, instruction, registers);  // Reg
                    memoryAccesses.push_back("cmp " +st + ", $" + common.getHex(num1, 0, 0));

                    int16_t num2, num3;
                    uint16_t num4;

                    num2 = memories16bit[st]; // R/M
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + st);

                    num3 = num2 - num1;
                    num4 = unsigned(num3);


                    memoryAccesses.push_back("write " + to_string(num3) + " to " + st);

                    if (num3 > 0) {
                        cout << "First operand is greater than second operand " << endl; 
                    }
                    else {
                        cout << "First operand is less than second operand " << endl;
                    }

                    common.setOverflow16bit(num1, num2, num3, registers);
                    common.setCarry16bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    dispWithoutSIB =  st + ", $" + common.getHex(num1, 0, 0) +"\n";
                }
                else if (s == 1)
                {
                    int8_t num = common.assemble_bits(1, instruction, registers); // Reg
                    int16_t num1, num2, num3;
                    uint16_t num4;

                    num1 = num; // Reg
                    num2 = memories16bit[st];  // R/M
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + st);

                    num3 = num1 - num2;
                    num4 = unsigned(num3);

                    if (num3 > 0) {
                        cout << "First operand is greater than second operand " << endl; 
                    }
                    else {
                        cout << "First operand is less than second operand " << endl;
                    }

                    memoryAccesses.push_back("write " + to_string(num3) + " to " + st);

                    common.setOverflow16bit(num1, num2, num3, registers);
                    common.setCarry16bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    dispWithoutSIB =   st + ", $" + common.getHex(num, 4, 'f') +"\n";
                }
            }
            else
            {
                if (s == 0)
                {
                    int num2 = common.assemble_bits(4, instruction, registers);  // Reg
                    memoryAccesses.push_back("cmp " + st + ", $" + common.getHex(num2, 0, 0));

                    int num1 = memories32bit[st];  // R/M
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + st);

                    int num3 = num1 - num2;
                    unsigned int num4 = unsigned(num3);
                            
                    if (num3 > 0) {
                        cout << "First operand is greater than second operand " << endl; 
                    }
                    else {
                        cout << "First operand is less than second operand " << endl;
                    }

                    common.setOverflow32bit(num1, num2, num3, registers);
                    common.setCarry32bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    memoryAccesses.push_back("write " + to_string(num3) + " to " + st);

                    dispWithoutSIB =  st + ", $" + common.getHex(num2, 0, 0) +"\n";
                }
                else
                {
                    int8_t num = common.assemble_bits(1, instruction, registers);  // Reg
                    int num1 = memories32bit[st]; // R/M

                    memoryAccesses.push_back("cmp " + st + ", $" + common.getHex(num, 8, 'f') );
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + st);

                    int num2 = num; // Reg
                    int num3 = num2 - num1;
                    unsigned int num4 = unsigned(num3);

                    if (num3 > 0) {
                        cout << "First operand is greater than second operand " << endl; 
                    }
                    else {
                        cout << "First operand is less than second operand " << endl;
                    }

                    common.setOverflow32bit(num1, num2, num3, registers);
                    common.setCarry32bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    memoryAccesses.push_back("write " + to_string(num3) + " to " + st);

                    dispWithoutSIB =  st + ", $" + common.getHex(num, 8, 'f') + "\n";
                }
            }
        }
    }
    else
    {
        if (w == 0)
        {
            int8_t num1 = common.assemble_bits(1, instruction, registers);
            memoryAccesses.push_back("cmp " + st + "(%" + regs_32[rm] + ")"+", $" + common.getHex(num1, 0, 0));

            int8_t num2, num3;
            uint8_t num4;

            num2 = memories8bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)];
            memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

            num3 = num1 - num2;
            num4 = unsigned(num3);

            memories8bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)] = num3;
            memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

            common.setOverflow8bit(num1, num2, num3, registers);
            common.setCarry8bit(num1, num4, registers);
            common.setSign(num3, registers);
            common.setZero(num3, registers);

            dispWithoutSIB =   st + "(%" + regs_32[rm] + ")"+", $" + common.getHex(num1, 0, 0) +" \n";
        }
        else if (w == 1)
        {
            if (opSize)
            {
                if (s == 0)
                {
                    int16_t num1 = common.assemble_bits(2, instruction, registers); // Reg
                    memoryAccesses.push_back("cmp " +  st + "(%" + regs_32[rm] + ") "+", $" + common.getHex(num1, 0, 0));

                    int16_t num2, num3;
                    uint16_t num4;

                    num2 = memories16bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)]; // R/M
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

                    num3 = num2 - num1;
                    num4 = unsigned(num3);

                    memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

                    if (num3 > 0) {
                        cout << "First operand is greater than second operand " << endl; 
                    }
                    else {
                        cout << "First operand is less than second operand " << endl;
                    }

                    common.setOverflow16bit(num1, num2, num3, registers);
                    common.setCarry16bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    dispWithoutSIB =    st + "(%" + regs_32[rm] + ") "+", $" + common.getHex(num1, 0, 0)+"\n";
                }
                else if (s == 1)
                {
                    int8_t num = common.assemble_bits(1, instruction, registers); // Reg
                    int16_t num1, num2, num3;
                    uint16_t num4;

                    memoryAccesses.push_back("cmp " + st + "(%" + regs_32[rm] + ")" + ", $" + common.getHex(num, 4, 'f'));

                    num1 = num; // Reg
                    num2 = memories16bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)];  // R/M
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

                    num3 = num1 - num2;
                    num4 = unsigned(num3);

                    if (num3 > 0) {
                        cout << "First operand is greater than second operand " << endl; 
                    }
                    else {
                        cout << "First operand is less than second operand " << endl;
                    }

                    memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

                    common.setOverflow16bit(num1, num2, num3, registers);
                    common.setCarry16bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    dispWithoutSIB =  st + "(%" + regs_32[rm] + ")" + ", $" + common.getHex(num, 4, 'f') + "\n";
                }
            }
            else
            {
                if (s == 0)
                {
                    int num2 = common.assemble_bits(4, instruction, registers); // Reg
                    int num1 = memories32bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)]; // R/M
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

                    int num3 = num1 - num2;
                    unsigned int num4 = unsigned(num3);

                    if (num3 > 0) {
                        cout << "First operand is greater than second operand " << endl; 
                    }
                    else {
                        cout << "First operand is less than second operand " << endl;
                    }

                    common.setOverflow32bit(num1, num2, num3, registers);
                    common.setCarry32bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

                    dispWithoutSIB =  st + "(%" + regs_32[rm] + ")"+", $" + common.getHex(num2, 0, 0) + " \n";
                }
                else
                {
                    int8_t num = common.assemble_bits(1, instruction, registers); // Reg
                    int num1 = memories32bit[common.getHex(registers[regs_32[rm]] + disp, 0, 0)]; // R/M
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

                    int num2 = num; // Reg
                    int num3 = num2 - num1;
                    unsigned int num4 = unsigned(num3);

                    if (num3 > 0) {
                        cout << "First operand is greater than second operand " << endl; 
                    }
                    else {
                        cout << "First operand is less than second operand " << endl;
                    }

                    common.setOverflow32bit(num1, num2, num3, registers);
                    common.setCarry32bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[rm]] + disp, 0, 0));

                    dispWithoutSIB =   st + "(%" + regs_32[rm] + ")" + ", $" + common.getHex(num, 8, 'f') +"\n";
                }
            }
        }
    }

    return dispWithoutSIB;
};

string Cmp_immediate::decode_SIB(int w, int s, int mod)
{
    string stringSib = "";

    int scale = common.get_bits(7, 2, instruction.front());
    int index = common.get_bits(4, 3, instruction.front());
    int base = common.get_bits(1, 3, instruction.front());

    instruction.pop();
    registers["EIP"] = registers["EIP"] + 1;

    //printf("scale:%d \n", scale);
    //printf("index:%d \n", index);
    //printf("base:%d \n", base);

    scale = pow(2, scale);

    stringSib = decode_displacement_with_SIB(w, s, mod, index, scale, base);
    return stringSib;
}

string Cmp_immediate::decode_mod_00(int w, int s, int rm)
{
    string string00 = "";
    if (rm == 4)
    {
        string00 = decode_SIB(w, s, 0);
    }
    else if (rm == 5)
    {
        string00 = decode_displacement_without_SIB(w, s, 0, 5);
    }
    else
    {
        if (w == 0)
        {
            int8_t num1 = common.assemble_bits(1, instruction, registers);
            memoryAccesses.push_back("cmp (%" + regs_32[rm] + "), $" + common.getHex(num1, 0, 0));
            string00 =   "(%" + regs_32[rm] + "), $" + common.getHex(num1, 0, 0)+"\n";

            int8_t num2, num3;
            uint8_t num4;
            num2 = memories8bit[common.getHex(registers[regs_32[rm]], 0, 0)];
            memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[rm]], 0, 0));

            num3 = num1 - num2;
            num4 = unsigned(num3);

            memories8bit[common.getHex(registers[regs_32[rm]], 0, 0)] = num3;
            memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[rm]], 0, 0));

            common.setOverflow8bit(num1, num2, num3, registers);
            common.setCarry8bit(num1, num4, registers);
            common.setSign(num3, registers);
            common.setZero(num3, registers);
        }
        else if (w == 1)
        {
            if (opSize)
            {
                if (s == 0)
                {
                    int16_t num1 = common.assemble_bits(2, instruction, registers); // Reg
                    memoryAccesses.push_back("cmp (%" + regs_32[rm] + "), $" + common.getHex(num1, 0, 0));

                    string00 =  "(%" + regs_32[rm] + "), $" + common.getHex(num1, 0, 0) +"\n";

                    int16_t num2, num3;
                    uint16_t num4;

                    num2 = memories16bit[common.getHex(registers[regs_32[rm]], 0, 0)];  // R/M
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[rm]], 0, 0));

                    num3 = num2 - num1;
                    num4 = unsigned(num3);

                    if (num3 > 0) {
                        cout << "First operand is greater than second operand " << endl; 
                    }
                    else {
                        cout << "First operand is less than second operand " << endl;
                    }

                    memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[rm]], 0, 0));

                    common.setOverflow16bit(num1, num2, num3, registers);
                    common.setCarry16bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);
                }
                else if (s == 1)
                {
                    int8_t num = common.assemble_bits(1, instruction, registers);  // Reg
                    string00 =  "(%" + regs_32[rm] + "), $" + common.getHex(num, 4, 'f') +"\n";

                    int16_t num1, num2, num3;
                    uint16_t num4;

                    num1 = num; // Reg
                    num2 = memories16bit[common.getHex(registers[regs_32[rm]], 0, 0)]; // R/M
                    memoryAccesses.push_back("Read " + to_string(num2) + " from " + common.getHex(registers[regs_32[rm]], 0, 0));

                    num3 = num1 - num2;
                    num4 = unsigned(num3);

                    memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[rm]], 0, 0));

                    if (num3 > 0) {
                        cout << "First operand is greater than second operand " << endl; 
                    }
                    else {
                        cout << "First operand is less than second operand " << endl;
                    }

                    common.setOverflow16bit(num1, num2, num3, registers);
                    common.setCarry16bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);
                }
            }
            else
            {
                if (s == 0)
                {
                    int num2 = common.assemble_bits(4, instruction, registers); // Reg
                    memoryAccesses.push_back("cmp (%" + regs_32[rm] + "), $" + common.getHex(num2, 0, 0));

                    int num1 = memories32bit[common.getHex(registers[regs_32[rm]], 0, 0)]; // R/M
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[rm]], 0, 0));

                    //registers["ECX"]
                    int num3 = num1 - num2;
                    unsigned int num4 = unsigned(num3);

                    if (num3 > 0) {
                        cout << "First operand is greater than second operand " << endl; 
                    }
                    else {
                        cout << "First operand is less than second operand " << endl;
                    }

                    common.setOverflow32bit(num1, num2, num3, registers);
                    common.setCarry32bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    string00 =  "(%" + regs_32[rm] + "), $" + common.getHex(num2, 0, 0) +"\n";
                    memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[rm]], 0, 0));
                }
                else
                {
                    int8_t num = common.assemble_bits(1, instruction, registers); // Reg
                    int num1 = memories32bit[common.getHex(registers[regs_32[rm]], 0, 0)]; // R/M
                    memoryAccesses.push_back("Read " + to_string(num1) + " from " + common.getHex(registers[regs_32[rm]], 0, 0));

                    int num2 = num2; /// ****************************************************************************************************
                    int num3 = num1 - num2;
                    unsigned int num4 = unsigned(num3);

                    common.setOverflow32bit(num1, num2, num3, registers);
                    common.setCarry32bit(num1, num4, registers);
                    common.setSign(num3, registers);
                    common.setZero(num3, registers);

                    string00 = "(%" + regs_32[rm] + "), $" + common.getHex(num, 8, 'f') + "\n";
                    memories32bit[common.getHex(registers[regs_32[rm]], 0, 0)] = num3;
                    memoryAccesses.push_back("write " + to_string(num3) + " to " + common.getHex(registers[regs_32[rm]], 0, 0));
                }
            }
        }
    }

    return string00;
}

string Cmp_immediate::decode_mod_01(int w, int s, int rm)
{
    string string01 = "";
    if (rm == 4)
    {
        string01 = decode_SIB(w, s, 1);
    }
    else
    {
        string01 = decode_displacement_without_SIB(w, s, 1, rm);
    }
    return string01;
}

string Cmp_immediate::decode_mod_10(int w, int s, int rm)
{
    string string10 = "";
    if (rm == 4)
    {
        string10 = decode_SIB(w, s, 2);
    }
    else
    {
        string10 = decode_displacement_without_SIB(w, s, 2, rm);
    }
    return string10;
}

string Cmp_immediate::decode_mod_11(int w, int s, int rm)
{
    string string11 = "";
    if (w == 0)
    {

        //printf("w:0 and d:0 test make \n");

        int8_t num1, num2, num3;
        uint8_t num4;

        num1 = common.assemble_bits(1, instruction, registers);

        if (rm < 4)
        {
            num2 = common.get_bits(1, 8, registers[regs_32[rm]]);
            num3 = num1 - num2;
            num4 = unsigned(num3);


            if (num3 > 0) {
                cout << "First operand is greater than second operand " << endl; 
            }
            else {
                cout << "First operand is less than second operand " << endl;
            }
        }
        else
        {
            num2 = common.get_bits(9, 8, registers[regs_32[rm % 4]]);
            num3 = num1 - num2;
            num4 = unsigned(num3);

            if (num3 > 0) {
                cout << "First operand is greater than second operand " << endl; 
            }
            else {
                cout << "First operand is less than second operand " << endl;
            }
        }

        //cout << "num1: " << dec << signed(num1) << ", num2: " << dec << signed(num2) << ", num3: " << dec << signed(num3) << "\n";

        common.setOverflow8bit(num1, num2, num3, registers);
        common.setCarry8bit(num1, num4, registers);
        common.setSign(num3, registers);
        common.setZero(num3, registers);

        string11 = "%" + regs_8[rm] + ",$" + common.getHex(num1, 0, 0)+"\n";
    }
    else if (w == 1)
    {
        if (opSize)
        {
            if (s == 0)
            {
                //printf("w:0 and d:0 \n");

                int16_t num1, num2, num3;
                uint16_t num4;

                num1 = common.assemble_bits(2, instruction, registers); // Reg
                num2 = common.get_bits(1, 16, registers[regs_32[rm]]); // R/M
                num3 = num2 - num1;
                num4 = unsigned(num3);

                if (num3 > 0) {
                    cout << "First operand is greater than second operand " << endl; 
                }
                else {
                    cout << "First operand is less than second operand " << endl;
                }

                common.setOverflow16bit(num1, num2, num3, registers);
                common.setCarry16bit(num1, num4, registers);
                common.setSign(num3, registers);
                common.setZero(num3, registers);

                string11 = "%" + regs_16[rm] + ",$" + common.getHex(num1, 0, 0)+"\n";
            }
            else if (s == 1)
            {
                //printf("w:0 and d:1 \n");
                int16_t num1, num2, num3;
                uint16_t num4;

                int8_t num = common.assemble_bits(1, instruction, registers); // Reg
                num1 = num; // Reg - immediate
                num2 = common.get_bits(1, 16, registers[regs_32[rm]]); // R/M
                num3 = num2 - num1;
                num4 = unsigned(num3);

                //cout << "num1: " << dec << signed(num1) << ", num2: " << dec << signed(num2) << ", num3: " << dec << signed(num3) << "\n";

                if (num3 > 0) {
                    cout << "First operand is greater than second operand " << endl; 
                }
                else {
                    cout << "First operand is less than second operand " << endl;
                }

                common.setOverflow16bit(num1, num2, num3, registers);
                common.setCarry16bit(num1, num4, registers);
                common.setSign(num3, registers);
                common.setZero(num3, registers);

                string11 = "%" + regs_16[rm] +",$" + common.getHex(num, 4, 'f') +  "\n";
            }
        }
        else
        {
            if (s == 0)
            {
                //printf("w:1 and d:0 \n");
                int num1 = common.assemble_bits(4, instruction, registers); // Reg immediate
                int num2 = registers[regs_32[rm]]; // R/M
                int num3 = num2 - num1;
                unsigned int num4 = unsigned(num3);

                if (num3 > 0) {
                    cout << "First operand is greater than second operand " << endl; 
                }
                else {
                    cout << "First operand is less than second operand " << endl;
                }

                common.setOverflow32bit(num1, num2, num3, registers);
                common.setCarry32bit(num1, num4, registers);
                common.setSign(num3, registers);
                common.setZero(num3, registers);

                string11 = "%" + regs_32[rm] + ",$" + common.getHex(num1, 0, 0) + "\n";
            }
            else
            {
                //printf("w:1 and d:1 \n");
                int8_t num = common.assemble_bits(1, instruction, registers); // Reg
                int num1 = num; // Reg

                int num2 = registers[regs_32[rm]]; // R/M
                int num3 = num2 - num1;

                unsigned int num4 = unsigned(num3);

                if (num3 > 0) {
                    cout << "First operand is greater than second operand " << endl; 
                }
                else {
                    cout << "First operand is less than second operand " << endl;
                }

                common.setOverflow32bit(num1, num2, num3, registers);
                common.setCarry32bit(num1, num4, registers);
                common.setSign(num3, registers);
                common.setZero(num3, registers);

                string11 =  "%" + regs_32[rm] +",$" + common.getHex(num, 8, 'f') + "\n";
            }
        }
    }

    return string11;
}

string Cmp_immediate::decode_imm(short prefixes[4], int w, int s, int mod, int rm)
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
        decoded_bytes = decode_mod_00(w, s, rm);
    }
    else if (mod == 1)
    {
        decoded_bytes = decode_mod_01(w, s, rm);
    }
    else if (mod == 2)
    {
        decoded_bytes = decode_mod_10(w, s, rm);
    }
    else
    {
        decoded_bytes = decode_mod_11(w, s, rm);
        
    }

    return decoded_bytes;
}
