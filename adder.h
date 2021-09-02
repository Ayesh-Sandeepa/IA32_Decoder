#include <stdio.h>
#include <bitset>
#include <iostream>
#include <string>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <queue>
#include <list>


using namespace std;
class Adder{
    private:
        
    public:
        //map<string, int> &registers;
        //Adder(map<string, int> &registers);

        string decode_add(queue<short>& instruction, map<string, int> &registers, map<string, int> &memories32bit, map<string, int8_t> memories8bit, list<string> &memoryAccesses);
};