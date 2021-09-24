#include <stdio.h>
#include <string>
#include <map>
#include <queue>

using namespace std;

class Common{
    private:

    public:
        int get_bits(int pos, int noOfBits, int number);
        int assemble_bits(int bytes, bool SIB, queue<short> &instruction, map<string, int> &registers);
        void setZero(int number, map<string, int> &registers);
        void setSign(int number, map<string, int> &registers);
        void setCarry8bit(int8_t num1, uint8_t num4, map<string, int> &registers);
        void setCarry16bit(int16_t num1, uint16_t num4, map<string, int> &registers);
        void setCarry32bit(int num1, unsigned int num4, map<string, int> &registers);
        void setOverflow8bit(int8_t num1, int8_t num2, int8_t num3, map<string, int> &registers);
        void setOverflow16bit(int16_t num1, int16_t num2, int16_t num3, map<string, int> &registers);
        void setOverflow32bit(int num1, int num2, int num3, map<string, int> &registers);
};