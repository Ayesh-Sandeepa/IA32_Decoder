#include <stdio.h>
#include <bitset>
#include <iostream>
#include <string>
#include <cmath>
#include <sstream>
#include <iomanip>

std::string regs_32[8] = {"EAX", "ECX", "EDX", "EBX", "ESP", "EBP", "ESI", "EDI"};
std::string regs_8[8] = {"AL", "CL", "DL", "BL", "AH", "CH", "DH", "BH"};

short enc[] = {0x03, 0x44, 0xcd, 0x80, 0x00, 0x00, 0x00};

void convert_binary(short *encodings)
{
    std::string num1 = std::bitset<8>(encodings[1]).to_string();
    std::cout << num1 << "\n";
}

int get_bits(int pos, int noOfBits, int number)
{
    return (((1 << noOfBits) - 1) & (number >> (pos - 1)));
}

int assemble_bits(int bytes, bool SIB)
{
    int x, offset;

    if (SIB)
    {
        offset = 2 + bytes;
    }
    else
    {
        offset = 1 + bytes;
    }

    std::stringstream stream, comb_no;
    for (int i = 0; i < bytes; i++)
    {
        stream << std::setw(2) << std::setfill('0') << std::hex << enc[offset - i];
    }

    std::string result(stream.str());

    comb_no << std::hex << result;
    comb_no >> x;

    std::cout << result << "\n";
    std::cout << x << "\n";
    return x;
}

void decode_displacement_with_SIB(int w, int d, int mod, int reg, int index, int scale, int base)
{
    int disp_bytes[] = {4, 1, 4};
    int bytes = disp_bytes[mod];

    int disp = assemble_bits(bytes, true);

    if (mod == 0)
    {
        if (base == 5)
        {
            if (w == 0 and d == 0)
            {
                if (index == 4)
                {
                    std::cout << "add %" << regs_8[reg] << "," << disp << "\n";
                }
                else
                {
                    std::cout << "add %" << regs_8[reg] << "," << disp << "(,%" << regs_32[index] << "," << scale << ")"
                              << "\n";
                }
            }
            else if (w == 0 and d == 1)
            {
                if (index == 4)
                {
                    std::cout << "add " << disp << ",%" << regs_8[reg] << "\n";
                }
                else
                {
                    std::cout << "add " << disp << "(,%" << regs_32[index] << "," << scale << "),%" << regs_8[reg] << "\n";
                }
            }
            else if (w == 1 and d == 0)
            {
                if (index == 4)
                {
                    std::cout << "add %" << regs_32[reg] << "," << disp << "\n";
                }
                else
                {
                    std::cout << "add %" << regs_32[reg] << "," << disp << "(,%" << regs_32[index] << "," << scale << ")"
                              << "\n";
                }
            }
            else
            {
                if (index == 4)
                {
                    std::cout << "add " << disp << ",%" << regs_32[reg] << "\n";
                }
                else
                {
                    std::cout << "add " << disp << "(,%" << regs_32[index] << "," << scale << "),%" << regs_32[reg] << "\n";
                }
            }
        }
        else
        {
            if (w == 0 and d == 0)
            {
                if (index == 4)
                {
                    std::cout << "add %" << regs_8[reg] << ","
                              << "(%" << regs_32[base] << ")"
                              << "\n";
                }
                else
                {
                    std::cout << "add %" << regs_8[reg] << ","
                              << "(%" << regs_32[base] << ",%" << regs_32[index] << "," << scale << ")"
                              << "\n";
                }
            }
            else if (w == 0 and d == 1)
            {
                if (index == 4)
                {
                    std::cout << "add "
                              << "(%" << regs_32[base] << "),%" << regs_8[reg] << "\n";
                }
                else
                {
                    std::cout << "add "
                              << "(%" << regs_32[base] << ",%" << regs_32[index] << "," << scale << "),%" << regs_8[reg] << "\n";
                }
            }
            else if (w == 1 and d == 0)
            {
                if (index == 4)
                {
                    std::cout << "add %" << regs_32[reg] << ","
                              << "(%" << regs_32[base] << ")"
                              << "\n";
                }
                else
                {
                    std::cout << "add %" << regs_32[reg] << ","
                              << "(%" << regs_32[base] << ",%" << regs_32[index] << "," << scale << ")"
                              << "\n";
                }
            }
            else
            {
                if (index == 4)
                {
                    std::cout << "add "
                              << "(%" << regs_32[base] << ")"
                              << ",%" << regs_32[reg] << "\n";
                }
                else
                {
                    std::cout << "add "
                              << "(%" << regs_32[base] << ",%" << regs_32[index] << "," << scale << "),%" << regs_32[reg] << "\n";
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
                std::cout << "add %" << regs_8[reg] << "," << disp << "(%" << regs_32[base] << ")"
                          << "\n";
            }
            else
            {
                std::cout << "add %" << regs_8[reg] << "," << disp << "(%" << regs_32[base] << ",%" << regs_32[index] << "," << scale << ")"
                          << "\n";
            }
        }
        else if (w == 0 and d == 1)
        {
            if (index == 4)
            {
                std::cout << "add " << disp << "(%" << regs_32[base] << "),%" << regs_8[reg] << "\n";
            }
            else
            {
                std::cout << "add " << disp << "(%" << regs_32[base] << ",%" << regs_32[index] << "," << scale << "),%" << regs_8[reg] << "\n";
            }
        }
        else if (w == 1 and d == 0)
        {
            if (index == 4)
            {
                std::cout << "add %" << regs_32[reg] << "," << disp << "(%" << regs_32[base] << ")"
                          << "\n";
            }
            else
            {
                std::cout << "add %" << regs_32[reg] << "," << disp << "(%" << regs_32[base] << ",%" << regs_32[index] << "," << scale << ")"
                          << "\n";
            }
        }
        else
        {
            if (index == 4)
            {
                std::cout << "add " << disp << "(%" << regs_32[base] << ")"
                          << ",%" << regs_32[reg] << "\n";
            }
            else
            {
                std::cout << "add " << disp << "(%" << regs_32[base] << ",%" << regs_32[index] << "," << scale << "),%" << regs_32[reg] << "\n";
            }
        }
    }
}

void decode_displacement_without_SIB(int w, int d, int mod, int reg, int rm)
{
    int disp_bytes[] = {4, 1, 4};
    int bytes = disp_bytes[mod];

    int disp = assemble_bits(bytes, false);

    if (mod == 0)
    {
        if (w == 0 and d == 0)
        {
            std::cout << "add %" << regs_8[reg] << "," << disp << "\n";
        }
        else if (w == 0 and d == 1)
        {
            std::cout << "add " << disp << ",%" << regs_8[reg] << "\n";
        }
        else if (w == 1 and d == 0)
        {
            std::cout << "add %" << regs_32[reg] << "," << disp << "\n";
        }
        else
        {
            std::cout << "add " << disp << ",%" << regs_32[reg] << "\n";
        }
    }
    else
    {
        if (w == 0 and d == 0)
        {
            std::cout << "add %" << regs_8[reg] << "," << disp << "(%" << regs_32[rm] << ") \n";
        }
        else if (w == 0 and d == 1)
        {
            std::cout << "add " << disp << "(%" << regs_32[rm] << "),%" << regs_8[reg] << "\n"; 
        }
        else if (w == 1 and d == 0)
        {
            std::cout << "add %" << regs_32[reg] << "," << disp << "(%" << regs_32[rm] << ") \n";
        }
        else
        {
            std::cout << "add " << disp << "(%" << regs_32[rm] << "),%" << regs_32[reg] << "\n";
        }
    }
};

void decode_SIB(int w, int d, int mod, int reg)
{
    int scale = get_bits(7, 2, enc[2]);
    int index = get_bits(4, 3, enc[2]);
    int base = get_bits(1, 3, enc[2]);

    printf("scale:%d \n", scale);
    printf("index:%d \n", index);
    printf("base:%d \n", base);

    scale = pow(2, scale);

    decode_displacement_with_SIB(w, d, mod, reg, index, scale, base);

    /*
    if (base == 5)
    {
        
    }
    else
    {
        if (index == 4)
        {
            //not needed according to IA32
        }
        else
        {
            if (w == 0 and d == 0)
            {
                std::cout << "add %" << regs_8[reg] << ",(%" << regs_32[base] << ",%" << regs_32[index] << "," << scale << ")"
                          << "\n";
            }
            else if (w == 0 and d == 1)
            {
                std::cout << "add (%" << regs_32[base] << ",%" << regs_32[index] << "," << scale << "),%" << regs_8[reg] << "\n";
            }
            else if (w == 1 and d == 0)
            {
                std::cout << "add %" << regs_32[reg] << ",(%" << regs_32[base] << ",%" << regs_32[index] << "," << scale << ")"
                          << "\n";
            }
            else
            {
                std::cout << "add (%" << regs_32[base] << ",%" << regs_32[index] << "," << scale << "),%" << regs_32[reg] << "\n";
            }
        }
    }*/
}

void decode_mod_00(int w, int d, int reg, int rm)
{
    if (rm == 4)
    {
        decode_SIB(w, d, 0, reg);
    }
    else if (rm == 5)
    {
        decode_displacement_without_SIB(w, d, 0, reg, 5);
    }
    else
    {
        if (w == 0 and d == 0)
        {
            std::cout << "add %" << regs_8[reg] << ",(%" << regs_32[rm] << ")\n";
        }
        else if (w == 0 and d == 1)
        {
            std::cout << "add (%" << regs_32[rm] << "),%" << regs_8[reg] << "\n";
        }
        else if (w == 1 and d == 0)
        {
            std::cout << "add %" << regs_32[reg] << ",(%" << regs_32[rm] << ")\n";
        }
        else
        {
            std::cout << "add (%" << regs_32[rm] << "),%" << regs_32[reg] << "\n";
        }
    }
}

void decode_mod_01(int w, int d, int reg, int rm)
{
    if (rm == 4)
    {
        decode_SIB(w, d, 1, reg);
    }
    else
    {
        decode_displacement_without_SIB(w,d,1,reg,rm);
    }
}

void decode_mod_10(int w, int d, int reg, int rm)
{
    if (rm == 4)
    {
        decode_SIB(w, d, 2, reg);
    }
    else
    {
        decode_displacement_without_SIB(w,d,2,reg,rm);
    }
}

void decode_mod_11(int w, int d, int reg, int rm)
{
    if (w == 0 and d == 0)
    {
        std::cout << "add %" << regs_8[reg] << ",%" << regs_8[rm] << "\n";
    }
    else if (w == 0 and d == 1)
    {
        std::cout << "add %" << regs_8[rm] << ",%" << regs_8[reg] << "\n";
    }
    else if (w == 1 and d == 0)
    {
        std::cout << "add %" << regs_32[reg] << ",%" << regs_32[rm] << "\n";
    }
    else
    {
        std::cout << "add %" << regs_32[rm] << ",%" << regs_32[reg] << "\n";
    }
}

int main()
{
    //printf ("Typical Hello World!");

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
