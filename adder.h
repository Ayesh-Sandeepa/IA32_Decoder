#include <stdio.h>
#include <bitset>
#include <iostream>
#include <string>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <queue>


using namespace std;
class Adder{
    private:

    public:
        string decode_add(queue<short>& instruction, map<string, int> &registers);
};