#include <stdio.h>
#include <string>
#include <map>
#include <queue>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "common.h"

int Common::get_bits(int pos, int noOfBits, long number)
{
    long one = 1;
    return (((one << noOfBits) - 1) & (number >> (pos - 1)));
}

int Common::assemble_bits(int bytes, queue<short> &instruction, map<string, int> &registers)
{
    int offset;
    long x;
    short displacement[4];

    stringstream stream, comb_no;

    for (int count = 0; count < bytes; count++)
    {
        displacement[count] = instruction.front();
        instruction.pop();
        registers["EIP"] = registers["EIP"] + 1;

        //cout << "Add to displacement:"<<displacement[count] <<"\n";
    }
    for (int i = 0; i < bytes; i++)
    {
        stream << setw(2) << setfill('0') << hex << displacement[bytes - 1 - i];
        //cout << "Add to string stream:"<<displacement[bytes - 1 - i]<<"\n";
    }

    string result(stream.str());

    // cout << "combined bytes:" << result << "\n";

    comb_no << hex << result;
    comb_no >> x;

     //cout << "result:"<<result << "\n";
     //cout << "result int:"<<x << "\n";
    return (int)x;
}

void Common::setZero(int number, map<string, int> &registers)
{
    // set or unset the sign flag
    if (number = 0)
    {
        registers["EFLAGS"] = registers["EFLAGS"] | 0x40;
    }
    else
    {
        registers["EFLAGS"] = registers["EFLAGS"] & 0xffffffbf;
    }
}

void Common::setSign(int number, map<string, int> &registers)
{
    // set or unset the sign flag
    if (number < 0)
    {
        registers["EFLAGS"] = registers["EFLAGS"] | 0x80;
    }
    else
    {
        registers["EFLAGS"] = registers["EFLAGS"] & 0xffffff7f;
    }
}

void Common::setCarry8bit(int8_t num1, uint8_t num4, map<string, int> &registers)
{
    // set or unset carry flag
    if (num4 < unsigned(num1))
    {
        registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
    }
    else
    {
        registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
    }
}

void Common::setCarry16bit(int16_t num1, uint16_t num4, map<string, int> &registers)
{
    // set or unset carry flag
    if (num4 < unsigned(num1))
    {
        registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
    }
    else
    {
        registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
    }
}

void Common::setCarry32bit(int num1, unsigned int num4, map<string, int> &registers)
{
    // set or unset carry flag
    if (num4 < unsigned(num1))
    {
        registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
    }
    else
    {
        registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
    }
}

void Common::setOverflow8bit(int8_t num1, int8_t num2, int8_t num3, map<string, int> &registers)
{
    // set or unset overflow flag
    if (num1 > 0 and num2 > 0 and num3 < 0)
    {
        registers["EFLAGS"] = registers["EFLAGS"] | 0x800;
    }
    else if (num1 < 0 and num2 < 0 and num3 > 0)
    {
        registers["EFLAGS"] = registers["EFLAGS"] | 0x800;
    }
    else
    {
        registers["EFLAGS"] = registers["EFLAGS"] & 0xfffff7ff;
    }
}

void Common::setOverflow16bit(int16_t num1, int16_t num2, int16_t num3, map<string, int> &registers)
{
    // set or unset overflow flag
    if (num1 > 0 and num2 > 0 and num3 < 0)
    {
        registers["EFLAGS"] = registers["EFLAGS"] | 0x800;
    }
    else if (num1 < 0 and num2 < 0 and num3 > 0)
    {
        registers["EFLAGS"] = registers["EFLAGS"] | 0x800;
    }
    else
    {
        registers["EFLAGS"] = registers["EFLAGS"] & 0xfffff7ff;
    }
}

void Common::setOverflow32bit(int num1, int num2, int num3, map<string, int> &registers)
{
    // set or unset overflow flag
    if (num1 > 0 and num2 > 0 and num3 < 0)
    {
        registers["EFLAGS"] = registers["EFLAGS"] | 0x800;
    }
    else if (num1 < 0 and num2 < 0 and num3 > 0)
    {
        registers["EFLAGS"] = registers["EFLAGS"] | 0x800;
    }
    else
    {
        registers["EFLAGS"] = registers["EFLAGS"] & 0xfffff7ff;
    }
}

void Common::listQueue(queue<short> myQueue)
{
    // function to print the whole queue
    while (!myQueue.empty())
    {
        cout << ' ' << std::setw(2) << std::setfill('0') << hex << myQueue.front();
        myQueue.pop();
    }
    cout << "\n";
}

string Common::getHex(int num, int width = 0, char fill='f')
{

    if (width == 0)
    {
        stringstream ms;
        ms << "0x" << hex << num;
        string rs(ms.str());
        return rs;
    }
    else
    {
        stringstream ms;
        if (width == 2)
        {
            int8_t numtemp = num;
            if (numtemp > 0)
            {
                ms << "0x" << setw(width) << setfill('0') << hex << (int)numtemp;
            }
            else
            {
                ms << "0x" << setw(width) << setfill('f') << hex << (int)numtemp;
            }
        }
        else if (width == 4)
        {
            int16_t numtemp = num;
            if (numtemp > 0)
            {
                ms << "0x" << setw(width) << setfill('0') << hex << numtemp;
            }
            else
            {
                ms << "0x" << setw(width) << setfill('f') << hex << numtemp;
            }
        }
        else if (width == 8)
        {
            if (num > 0)
            {
                ms << "0x" << setw(width) << setfill('0') << hex << num;
            }
            else
            {
                ms << "0x" << setw(width) << setfill('f') << hex << num;
            }
        }
        string rs(ms.str());
        return rs;
    }
}

string Common::getHexLong(long num, int width = 0, char fill='f')
{

    if (width == 0)
    {
        stringstream ms;
        ms << "0x" << hex << num;
        string rs(ms.str());
        return rs;
    }
    else
    {
        //stringstream ms;
        //ms << "0x" << setw(width) << setfill(fill) << hex << num;
        //string rs(ms.str());
        //return rs;
        return "0";
    }
}
