#include <stdio.h>
#include <queue>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <map>

#include "adder.h"

using namespace std;

void listQueue(queue<short> myQueue);

// parity flag should be used .......................................................//
// does carry and overflow are same

int main()
{
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

    Adder adder;

    queue<short> encoded_instructions;

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

        //listQueue(encoded_instructions);

        if (nextOpcode == 0x0 or nextOpcode == 0x1 or nextOpcode == 0x2 or nextOpcode == 0x3 or nextOpcode == 0x4 or nextOpcode == 0x5 or nextOpcode == 0x80 or nextOpcode == 0x81 or nextOpcode == 0x82 or nextOpcode == 0x83)
        {
            adder.decode_add(encoded_instructions, registers);
        }
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