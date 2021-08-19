#include <stdio.h>
#include <queue>
#include <iostream>
#include <fstream>
#include <sstream>

#include "adder.h"

using namespace std;

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

    /*function to print the whole queue
    while (!encoded_instructions.empty()){
        cout << ' ' << encoded_instructions.front();
        encoded_instructions.pop();
    }
    */

    nextOpcode = encoded_instructions.front();

    if (nextOpcode == 0 or nextOpcode == 1 or nextOpcode == 2 or nextOpcode == 3)
    {
        adder.decode_add(encoded_instructions);
    }

    myfile.close();
    return 0;
}