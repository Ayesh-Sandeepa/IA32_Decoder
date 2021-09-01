#include <stdio.h>
#include <bitset>
#include <iostream>
#include <string>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <queue>
#include <map>

#include "adder.h"

using namespace std;

string regs_32[8] = {"EAX", "ECX", "EDX", "EBX", "ESP", "EBP", "ESI", "EDI"};
string regs_8[8] = {"AL", "CL", "DL", "BL", "AH", "CH", "DH", "BH"};

short enc[] = {0x03, 0x44, 0xcd, 0x80, 0x00, 0x00, 0x00};

void convert_binary(short *encodings)
{
    string num1 = bitset<8>(encodings[1]).to_string();
    cout << num1 << "\n";
}

int get_bits(int pos, int noOfBits, int number)
{
    return (((1 << noOfBits) - 1) & (number >> (pos - 1)));
}

int assemble_bits(int bytes, bool SIB, queue<short> &instruction)
{
    int x, offset;
    short displacement[4];

    stringstream stream, comb_no;

    for (int count = 0; count < bytes; count++)
    {
        displacement[count] = instruction.front();
        instruction.pop();
    }
    for (int i = 0; i < bytes; i++)
    {
        stream << setw(2) << setfill('0') << hex << displacement[3 - i];
    }

    string result(stream.str());

    comb_no << hex << result;
    comb_no >> x;

    //cout << result << "\n";
    //cout << x << "\n";
    return x;
}

string decode_displacement_with_SIB(int w, int d, int mod, int reg, int index, int scale, int base, queue<short> &instruction, map<string, int> &registers)
{
    int disp_bytes[] = {4, 1, 4};
    int bytes = disp_bytes[mod];

    int disp;

    if (mod == 0 and base != 5)
    {
    }
    else
    {
        disp = assemble_bits(bytes, true, instruction);
    }

    if (mod == 0)
    {
        if (base == 5)
        {
            if (w == 0 and d == 0)
            {
                if (index == 4)
                {
                    return "%" + regs_8[reg] + "," + to_string(disp) + "\n";
                }
                else
                {
                    return "%" + regs_8[reg] + "," + to_string(disp) + "(,%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                }
            }
            else if (w == 0 and d == 1)
            {
                if (index == 4)
                {
                    return to_string(disp) + ",%" + regs_8[reg] + "\n";
                }
                else
                {
                    return to_string(disp) + "(,%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_8[reg] + "\n";
                }
            }
            else if (w == 1 and d == 0)
            {
                if (index == 4)
                {
                    return "%" + regs_32[reg] + "," + to_string(disp) + "\n";
                }
                else
                {
                    return "%" + regs_32[reg] + "," + to_string(disp) + "(,%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                }
            }
            else
            {
                if (index == 4)
                {
                    return to_string(disp) + ",%" + regs_32[reg] + "\n";
                }
                else
                {
                    return to_string(disp) + "(,%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_32[reg] + "\n";
                }
            }
        }
        else
        {
            if (w == 0 and d == 0)
            {
                if (index == 4)
                {
                    return "%" + regs_8[reg] + "," + "(%" + regs_32[base] + ")" + "\n";
                }
                else
                {
                    return "%" + regs_8[reg] + "," + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                }
            }
            else if (w == 0 and d == 1)
            {
                if (index == 4)
                {
                    return "(%" + regs_32[base] + "),%" + regs_8[reg] + "\n";
                }
                else
                {
                    return "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_8[reg] + "\n";
                }
            }
            else if (w == 1 and d == 0)
            {
                if (index == 4)
                {
                    return "%" + regs_32[reg] + "," + "(%" + regs_32[base] + ")" + "\n";
                }
                else
                {
                    return "%" + regs_32[reg] + "," + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
                }
            }
            else
            {
                if (index == 4)
                {
                    return "(%" + regs_32[base] + ")" + ",%" + regs_32[reg] + "\n";
                }
                else
                {
                    return "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_32[reg] + "\n";
                }
            }
        }
        //printf("mod equals 0");
    }
    else
    {
        //printf("mod not equals 0");
        if (w == 0 and d == 0)
        {
            if (index == 4)
            {
                return "%" + regs_8[reg] + "," + to_string(disp) + "(%" + regs_32[base] + ")" + "\n";
            }
            else
            {
                return "%" + regs_8[reg] + "," + to_string(disp) + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
            }
        }
        else if (w == 0 and d == 1)
        {
            if (index == 4)
            {
                return to_string(disp) + "(%" + regs_32[base] + "),%" + regs_8[reg] + "\n";
            }
            else
            {
                return to_string(disp) + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_8[reg] + "\n";
            }
        }
        else if (w == 1 and d == 0)
        {
            if (index == 4)
            {
                return "%" + regs_32[reg] + "," + to_string(disp) + "(%" + regs_32[base] + ")" + "\n";
            }
            else
            {
                return "%" + regs_32[reg] + "," + to_string(disp) + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + ")" + "\n";
            }
        }
        else
        {
            if (index == 4)
            {
                return to_string(disp) + "(%" + regs_32[base] + ")" + ",%" + regs_32[reg] + "\n";
            }
            else
            {
                return to_string(disp) + "(%" + regs_32[base] + ",%" + regs_32[index] + "," + to_string(scale) + "),%" + regs_32[reg] + "\n";
            }
        }
    }
}

string decode_displacement_without_SIB(int w, int d, int mod, int reg, int rm, queue<short> &instruction, map<string, int> &registers)
{
    int disp_bytes[] = {4, 1, 4};
    int bytes = disp_bytes[mod];

    int disp = assemble_bits(bytes, false, instruction);

    if (mod == 0)
    {
        if (w == 0 and d == 0)
        {
            return "%" + regs_8[reg] + "," + to_string(disp) + "\n";
        }
        else if (w == 0 and d == 1)
        {
            return to_string(disp) + ",%" + regs_8[reg] + "\n";
        }
        else if (w == 1 and d == 0)
        {
            return "%" + regs_32[reg] + "," + to_string(disp) + "\n";
        }
        else
        {
            return to_string(disp) + ",%" + regs_32[reg] + "\n";
        }
    }
    else
    {
        if (w == 0 and d == 0)
        {
            return "%" + regs_8[reg] + "," + to_string(disp) + "(%" + regs_32[rm] + ") \n";
        }
        else if (w == 0 and d == 1)
        {
            return to_string(disp) + "(%" + regs_32[rm] + "),%" + regs_8[reg] + "\n";
        }
        else if (w == 1 and d == 0)
        {
            return "%" + regs_32[reg] + "," + to_string(disp) + "(%" + regs_32[rm] + ") \n";
        }
        else
        {
            return to_string(disp) + "(%" + regs_32[rm] + "),%" + regs_32[reg] + "\n";
        }
    }
};

string decode_SIB(int w, int d, int mod, int reg, queue<short> &instruction, map<string, int> &registers)
{
    string stringSib;

    int scale = get_bits(7, 2, instruction.front());
    int index = get_bits(4, 3, instruction.front());
    int base = get_bits(1, 3, instruction.front());

    instruction.pop();

    //printf("scale:%d \n", scale);
    //printf("index:%d \n", index);
    //printf("base:%d \n", base);

    scale = pow(2, scale);

    stringSib = decode_displacement_with_SIB(w, d, mod, reg, index, scale, base, instruction, registers);
    return stringSib;
}

string decode_mod_00(int w, int d, int reg, int rm, queue<short> &instruction, map<string, int> &registers)
{
    string string00;
    if (rm == 4)
    {
        string00 = decode_SIB(w, d, 0, reg, instruction, registers);
    }
    else if (rm == 5)
    {
        string00 = decode_displacement_without_SIB(w, d, 0, reg, 5, instruction, registers);
    }
    else
    {
        if (w == 0 and d == 0)
        {
            string00 = "%" + regs_8[reg] + ",(%" + regs_32[rm] + ")\n";
        }
        else if (w == 0 and d == 1)
        {
            string00 = "(%" + regs_32[rm] + "),%" + regs_8[reg] + "\n";
        }
        else if (w == 1 and d == 0)
        {
            string00 = "%" + regs_32[reg] + ",(%" + regs_32[rm] + ")\n";
        }
        else
        {
            string00 = "(%" + regs_32[rm] + "),%" + regs_32[reg] + "\n";
        }
    }

    return string00;
}

string decode_mod_01(int w, int d, int reg, int rm, queue<short> &instruction, map<string, int> &registers)
{
    string string11;
    if (rm == 4)
    {
        string11 = decode_SIB(w, d, 1, reg, instruction, registers);
    }
    else
    {
        string11 = decode_displacement_without_SIB(w, d, 1, reg, rm, instruction, registers);
    }
    return string11;
}

string decode_mod_10(int w, int d, int reg, int rm, queue<short> &instruction, map<string, int> &registers)
{
    string string10;
    if (rm == 4)
    {
        string10 = decode_SIB(w, d, 2, reg, instruction, registers);
    }
    else
    {
        string10 = decode_displacement_without_SIB(w, d, 2, reg, rm, instruction, registers);
    }
    return string10;
}

string decode_mod_11(int w, int d, int reg, int rm, queue<short> &instruction, map<string, int> &registers)
{
    if (w == 0 and d == 0)
    {
        printf("w:0 and d:0 \n");

        int8_t num1 = get_bits(1, 8, registers[regs_32[reg]]);
        int8_t num2 = get_bits(1, 8, registers[regs_32[rm]]);
        int8_t num3 = num1 + num2;
        uint8_t num4 = unsigned(num1)+unsigned(num2);
        registers[regs_32[rm]] = ((registers[regs_32[rm]]) & 0xffffff00) | (num3 & 0x000000ff);

        cout << "num1: " << dec << signed(num1) << ", num2: " << dec << signed(num2) << ", num3: " << dec << signed(num3) << "\n";


        //set or unset overflow flag
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

        //set or unset carry flag
        if(num4<unsigned(num1)){
            registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
        }else{
            registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
        }

        return "%" + regs_8[reg] + ",%" + regs_8[rm] + "\n";
    }
    else if (w == 0 and d == 1)
    {
        printf("w:0 and d:1 \n");
        int8_t num1 = get_bits(1, 8, registers[regs_32[reg]]);
        int8_t num2 = get_bits(1, 8, registers[regs_32[rm]]);
        int8_t num3 = num1 + num2;
        uint8_t num4 = unsigned(num1)+unsigned(num2);
        registers[regs_32[reg]] = ((registers[regs_32[reg]]) & 0xffffff00) | (num3 & 0x000000ff);

        cout << "num1: " << dec << signed(num1) << ", num2: " << dec << signed(num2) << ", num3: " << dec << signed(num3) << "\n";

        //set or unset overflow flag
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

        //set or unset carry flag
        if(num4<unsigned(num1)){
            registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
        }else{
            registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
        }
        return "%" + regs_8[rm] + ",%" + regs_8[reg] + "\n";
    }
    else if (w == 1 and d == 0)
    {
        printf("w:1 and d:0 \n");
        int num1 = registers[regs_32[reg]];
        int num2 = registers[regs_32[rm]];
        int num3 = num1 + num2;
        unsigned int num4 = unsigned(num1)+unsigned(num2);
        registers[regs_32[rm]] = num3;

        cout << "num1: " << num1 << ", num2: " << num2 << ", num3: " << num3 << ", num4: " << num4 <<"\n";

        //set or unset overflow flag
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

        //set or unset carry flag
        if(num4<unsigned(num1)){
            registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
        }else{
            registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
        }
        return "%" + regs_32[reg] + ",%" + regs_32[rm] + "\n";
    }
    else
    {
        printf("w:1 and d:1 \n");
        int num1 = registers[regs_32[reg]];
        int num2 = registers[regs_32[rm]];
        int num3 = num1 + num2;
        unsigned int num4 = unsigned(num1)+unsigned(num2);
        registers[regs_32[reg]] = num3;

        cout << "num1: " << dec << num1 << ", num2: " << dec << num2 << ", num3: " << dec << num3 << "\n";

        //set or unset overflow flag
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

        //set or unset carry flag
        if(num4<unsigned(num1)){
            registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
        }else{
            registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
        }
        return "%" + regs_32[rm] + ",%" + regs_32[reg] + "\n";
    }
}

/*
int main()
{
    //printf ("Typical Hello World!");
    cout << enc[1] << " " << enc[0] << "\n";

    bool d = get_bits(2, 1, enc[0]);
    bool w = get_bits(1, 1, enc[0]);

    int mod = enc[1] >> 6;
    int reg = get_bits(4, 3, enc[1]);
    int rm = get_bits(1, 3, enc[1]);

    printf("d:%d \n", d);
    printf("w:%d \n", w);
    printf("mod:%d \n", mod);
    printf("reg:%d \n", reg);
    printf("rm:%d \n", rm);

    if (mod == 0)
    {
        decode_mod_00(w, d, reg, rm);
    }
    else if (mod == 1)
    {
        decode_mod_01(w, d, reg, rm);
    }
    else if (mod == 2)
    {
        decode_mod_10(w, d, reg, rm);
    }
    else
    {
        decode_mod_11(w, d, reg, rm);
    }

    //convert_binary(enc);

    //printf("%d \n", get_bits(8,enc[1]));
    return 0;
}
*/

string Adder::decode_add(queue<short> &instruction, map<string, int> &registers)
{

    bool d = get_bits(2, 1, instruction.front());
    bool w = get_bits(1, 1, instruction.front());

    instruction.pop();

    int mod = instruction.front() >> 6;
    int reg = get_bits(4, 3, instruction.front());
    int rm = get_bits(1, 3, instruction.front());

    instruction.pop();

    //printf("d:%d \n", d);
    //printf("w:%d \n", w);
    //printf("mod:%d \n", mod);
    //printf("reg:%d \n", reg);
    //printf("rm:%d \n", rm);

    string decoded_bytes;

    if (mod == 0)
    {
        decoded_bytes = decode_mod_00(w, d, reg, rm, instruction, registers);
    }
    else if (mod == 1)
    {
        decoded_bytes = decode_mod_01(w, d, reg, rm, instruction, registers);
    }
    else if (mod == 2)
    {
        decoded_bytes = decode_mod_10(w, d, reg, rm, instruction, registers);
    }
    else
    {
        decoded_bytes = decode_mod_11(w, d, reg, rm, instruction, registers);
    }

    cout << "add " << decoded_bytes;

    cout << "EAX: " << hex << registers["EAX"] << "\n";
    cout << "ECX: " << hex << registers["ECX"] << "\n";
    cout << "EFLAGS: " << hex << registers["EFLAGS"] << "\n";

    return "Adder instantiated and done";
}
