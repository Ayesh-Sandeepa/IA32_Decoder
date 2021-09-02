#include <stdio.h>
#include <iostream>


using namespace std;
int main(){

   /* int a=0xbf8db144;
    int b=0x88c5cffb;

   int p=-70;
   int8_t p1=80;
   int8_t p2=69;

   int8_t pr=p1+p2;

    
   int num=0x4853813f;
   int8_t num1=0xa3;
   int num3=(num & 0xffffff00) | (num1 & 0x000000ff);

   cout << hex << num3<<"\n"; 


   */

   int num1=0xbf8db144;
   int num2=0x88c5cffb;
   unsigned int nums1=num1;
   unsigned int nums2=num2;
   int tot1=num1+num2;
   int tot2=(num1+num2);
   int tot3=tot1>>1;


/*
   if num1 and num2 are in same sign and the tot in other sign, there is overflow
   it tot2<0, there is a carry
*/
   cout << dec << "num1: " <<num1 << "    num2: "<<num2<< "     tot1: "<<tot1<<"     tot2: "<<tot2<<"     tot3: "<<tot3<<"\n";
   cout << dec << "nums1: " <<nums1 << "    nums2: "<<nums2<<"\n";
    return 0;
}