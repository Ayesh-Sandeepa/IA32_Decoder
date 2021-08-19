#include <stdio.h>
#include <queue>
#include <iostream>
#include <fstream>
#include <sstream>

#include "adder.h"

using namespace std;

void listQueue(queue<short> myQueue);

int main()
{
    ifstream myfile;
    myfile.open("test.txt");

    Adder adder;

    queue<short> encoded_instructions;

    stringstream ss;
    string test_data, word;
    int num, nextOpcode;
    while (myfile >> word)
    {
        ss << hex << word;
        ss >> num;
        ss.clear();
        encoded_instructions.push(num);
    }

    

    while (!encoded_instructions.empty())
    {

        nextOpcode = encoded_instructions.front();
        //cout << "Next opcode : " << nextOpcode << "\n";

        //listQueue(encoded_instructions);

        if (nextOpcode == 0 or nextOpcode == 1 or nextOpcode == 2 or nextOpcode == 3)
        {
            adder.decode_add(encoded_instructions);
        }
    }

    myfile.close();
    return 0;
}

void listQueue(queue<short> myQueue){
    //function to print the whole queue
    while (!myQueue.empty()){
        cout << ' ' << std::setw(2) << std::setfill('0') << hex << myQueue.front();
        myQueue.pop();
    }
    cout << "\n";
}