#include <stdio.h>
#include <queue>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <map>
#include <list>

#include "adder.h"

using namespace std;

void listQueue(queue<short> myQueue);

int main()
{
    list<string> memoryAccesses;
    map<string, int> memories32bit;
    map<string, int16_t> memories16bit;
    map<string, int8_t> memories8bit;
    queue<short> encoded_instructions;
    short prefixes[4]={0,0,0,0};

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
    Adder adder(common,encoded_instructions, registers, memories32bit, memories16bit, memories8bit, memoryAccesses);

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

    while (!encoded_instructions.empty())
    {

        nextOpcode = encoded_instructions.front();
        //cout << "Next opcode : " << nextOpcode << "\n";

        listQueue(encoded_instructions);
        if(nextOpcode==0xf0 or nextOpcode==0xf2 or nextOpcode==0xf3){
            prefixes[0]=nextOpcode;
            encoded_instructions.pop();
        }
        else if(nextOpcode==0x2e or nextOpcode==0x36 or nextOpcode==0x3e or nextOpcode==0x26 or nextOpcode==0x64 or nextOpcode==0x65){
            prefixes[1]=nextOpcode;
            encoded_instructions.pop();
        }
        else if(nextOpcode==0x66){
            prefixes[2]=nextOpcode;
            encoded_instructions.pop();
        }
        else if(nextOpcode==0x67){
            prefixes[3]=nextOpcode;
            encoded_instructions.pop();
        }
        else if (nextOpcode == 0x0 or nextOpcode == 0x1 or nextOpcode == 0x2 or nextOpcode == 0x3 or nextOpcode == 0x4 or nextOpcode == 0x5 or nextOpcode == 0x80 or nextOpcode == 0x81 or nextOpcode == 0x82 or nextOpcode == 0x83)
        {
            adder.decode_add(prefixes);
            prefixes[0]=0;
            prefixes[1]=0;
            prefixes[2]=0;
            prefixes[3]=0;
        }
    }

    for(auto it=memoryAccesses.begin();it!=memoryAccesses.end();++it){
        cout << *it <<"\n";
    }

    myfile.close();

    return 0;
}

void listQueue(queue<short> myQueue)
{
    //function to print the whole queue
    while (!myQueue.empty())
    {
        cout << ' ' << std::setw(2) << std::setfill('0') << hex << myQueue.front();
        myQueue.pop();
    }
    cout << "\n";
}

