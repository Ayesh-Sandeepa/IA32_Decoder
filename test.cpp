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

   /*
   int x = 0xBFEE19; // 0xBFEE19
   for (size_t i = 0; i < sizeof(x); ++i)
   {
      // Convert to unsigned char* because a char is 1 byte in size.
      // That is guaranteed by the standard.
      // Note that is it NOT required to be 8 bits in size.
      unsigned char byte = *((unsigned char *)&x + i);
      printf("Byte %d = %u\n", i, (unsigned)byte);
   }

   uint16_t *num=(uint16_t *)&x;
   uint8_t *num1=(uint8_t *)&x;

   printf("First 16 bits:%d\n", *num);
   printf("First 8 bits:%d\n", *num1);
   */

   int8_t num1 = 0x54;

   //stringstream ms;
    //ms << "0x"  << hex <<setw(4)<<setfill('0')<<  (int)num1;
   //cout << "0x" << std::hex << std::setw(2) << std::setfill('0')<< static_cast<unsigned int>(num1) << '\n';
   //string rs(ms.str());
   //cout << rs << "\n";

   //cout << getHex(num1, 8) << "\n";

   string result="81db283b";

   stringstream ss;
   long number;

   ss<<hex<<result;
   ss>>number;

   cout << hex<<number <<"\n";

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
