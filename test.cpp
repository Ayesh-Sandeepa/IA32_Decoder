#include <stdio.h>
#include <iostream>
#include <map>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;
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

   int num1=0xa4c256;

   stringstream ms;
   ms << "0x" <<hex << num1;
   string rs(ms.str());

   string ss = "num1:"+rs+"\n";
   cout << ss;

   return 0;
}