#include <stdio.h>
#include <iostream>
#include <map>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

string getHex(long int num, int width, char fill);
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

  long num1=0x26a40cec23757d34;
  int first=get_bits(33,32,num1);


   cout << getHex((unsigned long)first,0,0)<<"\n";

   return 0;
}

string getHex(long int num, int width = 0, char fill = '0')
{

    if (width == 0)
    {
        stringstream ms;
        ms << "0x"<< hex << num;
        string rs(ms.str());
        return rs;
    }
    else
    {
        stringstream ms;
        ms << "0x" << setw(width) << setfill(fill) << hex << num;
        string rs(ms.str());
        return rs;
    }
}

int get_bits(int pos, int noOfBits, long number)
{
      long one=1;
      long num2=number >> (pos - 1) ;
      cout << num2<< "\n";
    return (((one << noOfBits) - 1) & (number >> (pos - 1)));
}

