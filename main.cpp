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

using namespace std;

void listQueue(queue<short> myQueue);

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

    /*
    memories32bit[to_string(0xbf8db144)]=0;
    memories32bit[to_string(0x88c5cffb)]=0;
    memories32bit[to_string(0x1)]=0;
    memories32bit[to_string(0xae5ff4)]=0;
    memories32bit[to_string(0xbf8db118)]=0;
    memories32bit[to_string(0x9a0ca0)]=0;
    memories32bit[to_string(0x0)]=0;

    memories16bit[to_string(0xbf8db144)]=0;
    memories16bit[to_string(0x88c5cffb)]=0;
    memories16bit[to_string(0x1)]=0;
    memories16bit[to_string(0xae5ff4)]=0;
    memories16bit[to_string(0xbf8db118)]=0;
    memories16bit[to_string(0x9a0ca0)]=0;
    memories16bit[to_string(0x0)]=0;

    memories8bit[to_string(0xbf8db144)]=0;
    memories8bit[to_string(0x88c5cffb)]=0;
    memories8bit[to_string(0x1)]=0;
    memories8bit[to_string(0xae5ff4)]=0;
    memories8bit[to_string(0xbf8db118)]=0;
    memories8bit[to_string(0x9a0ca0)]=0;
    memories8bit[to_string(0x0)]=0;
*/

    ifstream myfile;
    myfile.open("test.txt");

    Common common;
    Adder adder(common, encoded_instructions, registers, memories32bit, memories16bit, memories8bit, memoryAccesses);
    Bitset bitset(common, encoded_instructions, registers, memories32bit, memories16bit, memories8bit, memoryAccesses);
    Mov mov(common,encoded_instructions, registers, memories32bit, memories16bit, memories8bit, memoryAccesses);
    Inc inc(common,encoded_instructions, registers, memories32bit, memories16bit, memories8bit, memoryAccesses);
    Dec dec(common,encoded_instructions, registers, memories32bit, memories16bit, memories8bit, memoryAccesses);

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

        //cout << "Next opcode : " << nextOpcode << "\n";

        //common.listQueue(encoded_instructions);
        //cout << "Next opcode:" << common.getHex(nextOpcode, 0, 0) << "\n";
        if (nextOpcode == 0xf0 or nextOpcode == 0xf2 or nextOpcode == 0xf3)
        {
            //printf("Prefix 1 set\n");
            prefixes[0] = nextOpcode;
            encoded_instructions.pop();
        }
        else if (nextOpcode == 0x2e or nextOpcode == 0x36 or nextOpcode == 0x3e or nextOpcode == 0x26 or nextOpcode == 0x64 or nextOpcode == 0x65)
        {
            //printf("Prefix 2 set\n");
            prefixes[1] = nextOpcode;
            encoded_instructions.pop();
        }
        else if (nextOpcode == 0x66)
        {
            //printf("Prefix 3 set\n");
            prefixes[2] = nextOpcode;
            encoded_instructions.pop();
        }
        else if (nextOpcode == 0x67)
        {
            //printf("Prefix 4 set\n");
            prefixes[3] = nextOpcode;
            encoded_instructions.pop();
        }
        else if (nextOpcode == 0x0f)
        {
            twoBytesOpcode = true;
            //printf("Two byte opcode enables\n");
            encoded_instructions.pop();
        }
        else if (twoBytesOpcode == true and nextOpcode == 0x38)
        {
            threeBytesOpcode = true;
            //printf("Three byte opcode enables\n");
            encoded_instructions.pop();
        }
        else
        {
            if (threeBytesOpcode)
            {
                //printf("Three bytes opcode \n");
            }
            else if (twoBytesOpcode)
            {
                //printf("Two bytes opcode \n");
                if (nextOpcode == 0xa3 or nextOpcode == 0xba)
                {
                    //printf("Opcode a3 or ba \n");
                    bitset.decode_bt(prefixes);
                }
            }
            else
            {
                //printf("One byte opcode \n");
                if (nextOpcode == 0x0 or nextOpcode == 0x1 or nextOpcode == 0x2 or nextOpcode == 0x3 or nextOpcode == 0x4 or nextOpcode == 0x5 or nextOpcode == 0x80 or nextOpcode == 0x81 or nextOpcode == 0x83)
                {
                    adder.decode_add(prefixes);
                }
                else if (nextOpcode == 0x88 or nextOpcode == 0x89 or nextOpcode == 0x8a or nextOpcode == 0x8b or nextOpcode == 0xb0 or nextOpcode == 0xb8 or nextOpcode == 0xc6 or nextOpcode == 0xc7)
                {
                    mov.decode_mov(prefixes);
                }
                else if (nextOpcode == 0xfe or nextOpcode == 0xff or nextOpcode == 0x40 or nextOpcode == 0x48)
                {
                    inc.decode_inc(prefixes);
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
    }

    myfile.close();

    return 0;
}
