#include <stdio.h>
#include <queue>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <map>
#include <list>

#include "adder.h"
#include "bitset.h"
#include "mov.h"
#include "inc_dec.h"
#include "left_right_shift.h"
#include "And.h"
#include "mul.h"
#include "Cmp.h"
#include "Or.h"
#include "Xor.h"
#include "divider.h"
#include "not.h"
#include "Neg.h"

using namespace std;

void listQueue(queue<short> myQueue);

queue<short> getTempQueue(queue<short> Q)
{
    queue<short> Q2;
    for (int i = 0; i < Q.size(); i++)
    {
        short ele = Q.front();
        Q2.push(ele);
        Q.pop();
    }
    return Q2;
}

int main()
{
    list<string> memoryAccesses;
    map<string, int> memories32bit;
    map<string, int16_t> memories16bit;
    map<string, int8_t> memories8bit;
    queue<short> encoded_instructions;
    short prefixes[4] = {0, 0, 0, 0};

    map<string, int> registers;
    registers["EAX"] = 0xbf8db144;
    registers["ECX"] = 0x88c5cffb;
    registers["EDX"] = 0x1;
    registers["EBX"] = 0xae5ff4;
    registers["ESP"] = 0xbf8db0bc;
    registers["EBP"] = 0xbf8db118;
    registers["ESI"] = 0x9a0ca0;
    registers["EDI"] = 0x0;
    registers["EIP"] = 0x8048354;
    registers["EFLAGS"] = 0x246;
    registers["CS"] = 0x73;
    registers["SS"] = 0x7b;
    registers["DS"] = 0x7b;
    registers["ES"] = 0x7b;
    registers["FS"] = 0x0;
    registers["GS"] = 0x33;

  

    ifstream myfile;
    myfile.open("test.txt");

    ofstream myoutput;
    myoutput.open("ouput.txt");

    Common common;
    Adder adder(common, encoded_instructions, registers, memories32bit, memories16bit, memories8bit, memoryAccesses,myoutput);
    Bitset bitset(common, encoded_instructions, registers, memories32bit, memories16bit, memories8bit, memoryAccesses,myoutput);

    Mov mov(common,encoded_instructions, registers, memories32bit, memories16bit, memories8bit, memoryAccesses,myoutput);
    Inc incDec(common,encoded_instructions, registers, memories32bit, memories16bit, memories8bit, memoryAccesses,myoutput);
    Left_shift LeftRightShift(common,encoded_instructions, registers, memories32bit, memories16bit, memories8bit, memoryAccesses,myoutput);
    And and_(common, encoded_instructions, registers, memories32bit, memories16bit, memories8bit, memoryAccesses,myoutput);
    Multiplier multiplier(common, encoded_instructions, registers, memories32bit, memories16bit, memories8bit, memoryAccesses,myoutput);
    Cmp cmp(common, encoded_instructions, registers, memories32bit, memories16bit, memories8bit, memoryAccesses,myoutput);
    Or or_(common, encoded_instructions, registers, memories32bit, memories16bit, memories8bit, memoryAccesses,myoutput);
    Xor xor_(common, encoded_instructions, registers, memories32bit, memories16bit, memories8bit, memoryAccesses,myoutput);
    Divider divider(common, encoded_instructions, registers, memories32bit, memories16bit, memories8bit, memoryAccesses,myoutput);
    Not notobject(common, encoded_instructions, registers, memories32bit, memories16bit, memories8bit, memoryAccesses,myoutput);
    Neg negObject(common, encoded_instructions, registers, memories32bit, memories16bit, memories8bit, memoryAccesses,myoutput);


    stringstream sss;
    string test_data, word;
    int num, nextOpcode;
    while (myfile >> word)
    {
        sss << hex << word;
        sss >> num;
        sss.clear();
        encoded_instructions.push(num);
    }

    bool twoBytesOpcode = false;
    bool threeBytesOpcode = false;

    while (!encoded_instructions.empty())
    {

        nextOpcode = encoded_instructions.front();

        if (nextOpcode == 0xf0 or nextOpcode == 0xf2 or nextOpcode == 0xf3)
        {
            
            prefixes[0] = nextOpcode;
            encoded_instructions.pop();
        }
        else if (nextOpcode == 0x2e or nextOpcode == 0x36 or nextOpcode == 0x3e or nextOpcode == 0x26 or nextOpcode == 0x64 or nextOpcode == 0x65)
        {
            
            prefixes[1] = nextOpcode;
            encoded_instructions.pop();
        }
        else if (nextOpcode == 0x66)
        {
            
            prefixes[2] = nextOpcode;
            encoded_instructions.pop();
        }
        else if (nextOpcode == 0x67)
        {
            
            prefixes[3] = nextOpcode;
            encoded_instructions.pop();
        }
        else if (nextOpcode == 0x0f)
        {
            twoBytesOpcode = true;
            encoded_instructions.pop();
        }
        else if (twoBytesOpcode == true and nextOpcode == 0x38)
        {
            threeBytesOpcode = true;
            encoded_instructions.pop();
        }
        else
        {
            if (threeBytesOpcode)
            {
                
            }
            else if (twoBytesOpcode)
            {
                
                if (nextOpcode == 0xa3 or nextOpcode == 0xba)
                {
                    
                    bitset.decode_bt(prefixes);
                }
            }
            else
            {
                queue<short> encoded_instructions_temp = getTempQueue(encoded_instructions);
                encoded_instructions_temp.pop();
                short modrm = encoded_instructions_temp.front();
                int reg = common.get_bits(4, 3, modrm);

                
                if (nextOpcode == 0x0 or nextOpcode == 0x1 or nextOpcode == 0x2 or nextOpcode == 0x3 or nextOpcode == 0x4 or nextOpcode == 0x5 or ((nextOpcode == 0x80 or nextOpcode == 0x81 or nextOpcode == 0x83) and reg==0))
                {
                    adder.decode_add(prefixes);
                }
                else if ((nextOpcode == 0xf6 or nextOpcode == 0xf7) and reg==4)
                {
                    multiplier.decode_mul(prefixes);
                }
                else if ((nextOpcode == 0xf6 or nextOpcode == 0xf7) and reg==6)
                {
                    divider.decode_div(prefixes);
                }
                else if ((nextOpcode == 0xf6 or nextOpcode == 0xf7) and reg==2)
                {
                    notobject.decode_not(prefixes);
                }
                else if (nextOpcode == 0x88 or nextOpcode == 0x89 or nextOpcode == 0x8a or nextOpcode == 0x8b or nextOpcode == 0xb0 or nextOpcode == 0xb8 or nextOpcode == 0xc6 or nextOpcode == 0xc7)
                {
                    mov.decode_mov(prefixes);
                }

                else if (nextOpcode == 0xfe or nextOpcode == 0xff or nextOpcode == 0x40 or nextOpcode == 0x48)
                {
                    incDec.decode_inc(prefixes);
                }
                else if (nextOpcode == 0xc0 or nextOpcode == 0xc1 or nextOpcode == 0xd0 or nextOpcode == 0xd1 or nextOpcode == 0xd2 or nextOpcode == 0xd3)
                {
                    LeftRightShift.decode_shl(prefixes);
                }
                else if (nextOpcode == 0x20 or nextOpcode == 0x21 or nextOpcode == 0x22 or nextOpcode == 0x23 or nextOpcode == 0x24 or nextOpcode == 0x25 or ((nextOpcode == 0x80 or nextOpcode == 0x81 or nextOpcode == 0x83) and (reg == 4)))
                {
                    and_.decode_and(prefixes);
                }
                else if (nextOpcode == 0x3c or nextOpcode == 0x3d or nextOpcode == 0x38 or nextOpcode == 0x39 or nextOpcode == 0x3a or nextOpcode == 0x3b or ((nextOpcode == 0x80 or nextOpcode == 0x81 or nextOpcode == 0x83) and (reg == 7))) {
                    cmp.decode_cmp(prefixes);

                }
                else if (nextOpcode == 0x0c or nextOpcode == 0x0d or nextOpcode == 0x08 or nextOpcode == 0x09 or nextOpcode == 0x0a or nextOpcode == 0x0b or (nextOpcode == 0x80 or nextOpcode == 0x81 or nextOpcode == 0x83) and (reg == 1)) {
                    or_.decode_or(prefixes);
                }
                else if (nextOpcode == 0x34 or nextOpcode == 0x35 or nextOpcode == 0x30 or nextOpcode == 0x31 or nextOpcode == 0x32 or nextOpcode == 0x33 or (nextOpcode == 0x80 or nextOpcode == 0x81 or nextOpcode == 0x83) and (reg == 6)) {
                    xor_.decode_xor(prefixes);
                }
                else if (nextOpcode == 0xf6 or nextOpcode == 0xf7) {
                    negObject.decode_neg(prefixes);
                }
                else
                {
                    encoded_instructions.pop();
                }
            }
            prefixes[0] = 0;
            prefixes[1] = 0;
            prefixes[2] = 0;
            prefixes[3] = 0;
            twoBytesOpcode = false;
            threeBytesOpcode = false;
        }
    }

    for (auto it = memoryAccesses.begin(); it != memoryAccesses.end(); ++it)
    {
        cout << *it << "\n";
        myoutput << *it <<endl;
    }

    myfile.close();
    myoutput.close();
    return 0;
}
