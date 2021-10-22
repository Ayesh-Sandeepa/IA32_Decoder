#include <stdio.h>
#include <iostream>
#include <map>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

string getHex(int num, int width);
int get_bits(int pos, int noOfBits, long number);

int main()
{
   map<string, int> memories32bit;

  int8_t num1=0x53;
  int8_t num2=0x34;

  int16_t num=(num1<<8) | (uint8_t)num2;

  cout << getHex((uint16_t)num,0)<<"\n";

   return 0;
}

string getHex(int num, int width = 0)
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

int get_bits(int pos, int noOfBits, long number)
{
   long one = 1;
   long num2 = number >> (pos - 1);
   cout << num2 << "\n";
   return (((one << noOfBits) - 1) & (number >> (pos - 1)));
}
