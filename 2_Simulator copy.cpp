#include <fstream>
#include <new>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <bitset>
#include <cstdio>
#define MB 0x100000
#define memory_size 6 * MB
#define mask32 4294967295
#define mask26 67108863
#define mask16 65535
#define mask8 255
#define mask6 63
#define mask5 31
#define uint64 uint64_t
#define starting_address 0x400000
using namespace std;

// real address
uint64 start_text, end_text;
uint64 end_static, end_dynamic; // end_static = start_dynamic
uint64 PC;

// register
vector<int> reg(32);
int lo, hi;

inline int real_fic(uint64 a)
{
    return (int)(a - start_text + starting_address);
}

inline uint64 fic_real(int b)
{
    return (uint64)(b - starting_address) + start_text;
}

// memory simulation

void memory_simulation()
{
    void *uninstantiated_memory = ::operator new(memory_size);
    start_text = reinterpret_cast<uint64>(uninstantiated_memory);
    reg[29] = real_fic(start_text + memory_size);
    end_static = start_text + 1 * MB;
    end_text = start_text;
}

// loading

void load_static(ifstream &inFile, uint64 &ptr, map<string, int> &varname_address)
{
    uint64 ptr_original = ptr;
    string line;
    bool flag = 1;
    while (getline(inFile, line))
    {
        int comment_pos = line.find("#"); // delete comment
        if (comment_pos != string::npos)
            line = line.substr(0, comment_pos);
        line = line.substr(0, line.find_last_not_of(' ') + 1);
        if (line == ".text")
            break;
        if (line == ".data" || flag) // skip lines before .data
        {
            if (line == ".data")
                flag = 0;
            continue;
        }
        if (line.find_first_not_of(' ') == string::npos) // skip empty lines
            continue;
        else
        {
            int pos1 = line.find(':');
            varname_address[line.substr(0, pos1)] = ptr - ptr_original + starting_address;
            line = line.substr(pos1 + 1);
            stringstream ss;
            ss << line;
            string s1;
            ss >> s1;
            if (s1 == ".ascii")
            {
                string s2;
                ss >> ws;
                getline(ss, s2); // read all remaining
                uint64 ptr_s = ptr;
                for (int i = 1; i < s2.size() - 1; ++i)
                {
                    if (i + 1 < s2.size() && s2[i] == '\\' && s2[i + 1] == 'n')
                    {
                        ++i;
                        new (reinterpret_cast<char *>(ptr)) char('\n');
                    }
                    else
                        new (reinterpret_cast<char *>(ptr)) char(s2[i]);
                    ptr = ptr + 1;
                }
                if ((ptr - ptr_s) % 4 != 0)
                    ptr = ptr + (4 - (ptr - ptr_s) % 4);
            }
            else if (s1 == ".asciiz")
            {
                string s2;
                ss >> ws;
                getline(ss, s2); // read all remaining
                uint64 ptr_s = ptr;
                cout << s2 << endl;
                for (int i = 1; i < s2.size() - 1; ++i)
                {
                    if (i + 1 < s2.size() && s2[i] == '\\' && s2[i + 1] == 'n')
                    {
                        ++i;
                        new (reinterpret_cast<char *>(ptr)) char('\n');
                    }
                    else
                        new (reinterpret_cast<char *>(ptr)) char(s2[i]);
                    ptr = ptr + 1;
                }
                new (reinterpret_cast<char *>(ptr)) char('\0');
                ptr = ptr + 1;
                if ((ptr - ptr_s) % 4 != 0)
                    ptr = ptr + (4 - (ptr - ptr_s) % 4);
            }
            else if (s1 == ".word")
            {
                int s2;
                ss >> s2;
                new (reinterpret_cast<int *>(ptr)) int(s2);
                ptr = ptr + 4;
            }
            else if (s1 == ".byte")
            {
                int s2;
                ss >> s2;
                new (reinterpret_cast<int8_t *>(ptr)) int(s2);
                ptr = ptr + 4;
            }
            else if (s1 == ".half")
            {
                int s2;
                ss >> s2;
                new (reinterpret_cast<int16_t *>(ptr)) int(s2);
                ptr = ptr + 4;
            }
        }
    }
}

void load_text(vector<uint32_t> &machine_code, uint64 &ptr)
{
    for (int i = 0; i < machine_code.size(); ++i)
    {
        new (reinterpret_cast<uint32_t *>(ptr)) uint32_t(machine_code[i]);
        ptr = ptr + 4;
    }
}

void output_register()
{
    cout << "--------------------------" << endl;
    cout << "[0]$zero=  " << hex << reg[0] << endl;
    cout << "[1]$at=  " << hex << reg[1] << endl;
    cout << "[2]$v0=  " << hex << reg[2] << endl;
    cout << "[3]$v1=  " << hex << reg[3] << endl;
    cout << "[4]$a0=  " << hex << reg[4] << endl;
    cout << "[5]$a1=  " << hex << reg[5] << endl;
    cout << "[6]$a2=  " << hex << reg[6] << endl;
    cout << "[7]$a3=  " << hex << reg[7] << endl;
    cout << "[8]$t0=  " << hex << reg[8] << endl;
    cout << "[9]$t1=  " << hex << reg[9] << endl;
    cout << "[10]$t2=  " << hex << reg[10] << endl;
    cout << "[11]$t3=  " << hex << reg[11] << endl;
    cout << "[12]$t4=  " << hex << reg[12] << endl;
    cout << "[13]$t5=  " << hex << reg[13] << endl;
    cout << "[14]$t6=  " << hex << reg[14] << endl;
    cout << "[15]$t7=  " << hex << reg[15] << endl;
    cout << "[16]$s0=  " << hex << reg[16] << endl;
    cout << "[17]$s1=  " << hex << reg[17] << endl;
    cout << "[18]$s2=  " << hex << reg[18] << endl;
    cout << "[19]$s3=  " << hex << reg[19] << endl;
    cout << "[20]$s4=  " << hex << reg[20] << endl;
    cout << "[21]$s5=  " << hex << reg[21] << endl;
    cout << "[22]$s6=  " << hex << reg[22] << endl;
    cout << "[23]$s7=  " << hex << reg[23] << endl;
    cout << "[24]$t8=  " << hex << reg[24] << endl;
    cout << "[25]$t9=  " << hex << reg[25] << endl;
    cout << "[26]$k0=  " << hex << reg[26] << endl;
    cout << "[27]$k1=  " << hex << reg[27] << endl;
    cout << "[28]$gp=  " << hex << reg[28] << endl;
    cout << "[29]$sp=  " << hex << reg[29] << endl;
    cout << "[30]$fp=  " << hex << reg[30] << endl;
    cout << "[31]$ra=  " << hex << reg[31] << endl;
    cout << endl;
}

void simulate(ofstream &outFile, ifstream &input_syscall)
{
    PC = start_text;
    uint32_t code;
    while (PC < end_text)
    {
        // fetch command
        code = *(reinterpret_cast<uint32_t *>(PC));
        PC = PC + 4;
        // execute
        uint32_t op = code >> 26;
        // cout << bitset<32>(code) << endl;
        if (op == 0)
        {
            uint32_t funct = code & mask6;
            uint32_t rd = (code >> 11) & mask5;
            uint32_t rs = (code >> 21) & mask5;
            uint32_t rt = (code >> 16) & mask5;
            uint32_t shamt = (code >> 6) & mask5;
            if (funct == 0x20)
            {
                // add ,overflow
                reg[rd] = reg[rs] + reg[rt];
            }
            else if (funct == 0x21)
            {
                // addu
                reg[rd] = (uint32_t)reg[rs] + (uint32_t)reg[rt];
            }
            else if (funct == 0x24)
            {
                // and
                reg[rd] = reg[rs] & reg[rt];
            }
            else if (funct == 0x27)
            {
                // nor
                reg[rd] = ~(reg[rs] | reg[rt]);
            }
            else if (funct == 0x25)
            {
                // or
                reg[rd] = reg[rs] | reg[rt];
            }
            else if (funct == 4)
            {
                // sllv
                reg[rd] = reg[rt] << reg[rs];
            }
            else if (funct == 7)
            {
                // srav
                reg[rd] = reg[rt] >> reg[rs];
                if (reg[rt] >> 31 == 1)
                {
                    if (reg[rs] < 32)
                        reg[rd] |= mask32 << (32 - reg[rs]);
                    else
                        reg[rd] = mask32;
                }
            }
            else if (funct == 6)
            {
                // srlv
                reg[rd] = reg[rt] >> reg[rs];
            }
            else if (funct == 0x22)
            {
                // sub
                reg[rd] = reg[rs] - reg[rt];
            }
            else if (funct == 0x23)
            {
                // subu
                reg[rd] = (uint32_t)reg[rs] - (uint32_t)reg[rt];
                // trap overflow
            }
            else if (funct == 0x26)
            {
                // xor
                reg[rd] = reg[rs] ^ reg[rt];
            }
            else if (funct == 0x2a)
            {
                // slt
                reg[rd] = reg[rs] < reg[rt];
            }
            else if (funct == 0x2b)
            {
                // sltu
                reg[rd] = (uint32_t)reg[rs] < (uint32_t)reg[rt];
            }
            else if (funct == 0x1a)
            {
                // div
                lo = reg[rs] / reg[rt];
                hi = reg[rs] % reg[rt];
            }
            else if (funct == 0x1b)
            {
                // divu
                lo = (uint32_t)reg[rs] / (uint32_t)reg[rt];
                hi = (uint32_t)reg[rs] % (uint32_t)reg[rt];
            }
            else if (funct == 0x18)
            {
                // mult
                reg[rd] = reg[rs] * reg[rt];
            }
            else if (funct == 0x19)
            {
                // multu
                reg[rd] = (uint32_t)reg[rs] * (uint32_t)reg[rt];
            }
            else if (funct == 9)
            {
                // jalr
                reg[31] = real_fic(PC);
                PC = fic_real(reg[rs]);
            }
            else if (funct == 0x34)
            {
                // teq
                if (reg[rs] == reg[rt])
                {
                    cout << "trap error" << endl;
                    return;
                }
            }
            else if (funct == 0x36)
            {
                // tne
                if (reg[rs] != reg[rt])
                {
                    cout << "trap error" << endl;
                    return;
                }
            }
            else if (funct == 0x30)
            {
                // tge
                if (reg[rs] >= reg[rt])
                {
                    cout << "trap error" << endl;
                    return;
                }
            }
            else if (funct == 0x31)
            {
                // tgeu
                if ((uint32_t)reg[rs] >= (uint32_t)reg[rt])
                {
                    cout << "trap error" << endl;
                    return;
                }
            }
            else if (funct == 0x32)
            {
                // tlt
                if (reg[rs] < reg[rt])
                {
                    cout << "trap error" << endl;
                    return;
                }
            }
            else if (funct == 0x33)
            {
                // tltu
                if ((uint32_t)reg[rs] < (uint32_t)reg[rt])
                {
                    cout << "trap error" << endl;
                    return;
                }
            }
            else if (funct == 0)
            {
                // sll
                reg[rd] = reg[rt] << shamt;
            }
            else if (funct == 2)
            {
                // srl
                reg[rd] = reg[rt] >> shamt;
            }
            else if (funct == 3)
            {
                // sra
                reg[rd] = reg[rt] >> shamt;
                if (reg[rt] >> 31 == 1)
                {
                    if (shamt < 32)
                        reg[rd] ^= mask32 << (32 - shamt);
                    else
                        reg[rd] = mask32;
                }
            }
            else if (funct == 8)
            {
                // jr
                PC = fic_real(reg[rs]);
            }
            else if (funct == 0x11)
            {
                // mthi
                hi = reg[rs];
            }
            else if (funct == 0x13)
            {
                // mtlo
                lo = reg[rs];
            }
            else if (funct == 0x10)
            {
                // mfhi
                reg[rd] = hi;
            }
            else if (funct == 0x12)
            {
                // mflo
                reg[rd] = lo;
            }
            else if (funct == 0xc)
            {
                // syscall
                if (reg[2] == 1)
                {
                    // print_int
                    outFile << reg[4] << flush;
                }
                else if (reg[2] == 4)
                {
                    // print_string
                    char *c1 = reinterpret_cast<char *>(fic_real(reg[4]));
                    string s1;
                    while (*c1 != '\0')
                    {
                        s1 = s1 + *c1;
                        c1++;
                    }
                    outFile << s1 << flush;
                }
                else if (reg[2] == 5)
                {
                    // real int
                    input_syscall >> reg[2];
                }
                else if (reg[2] == 8)
                {
                    // read_string
                    string str;
                    input_syscall >> str;
                    int buffer = reg[4];
                    int length = reg[5];
                    if (str.length() + 1 > length)
                        str = str.substr(0, length - 1);
                    uint64 cur_address = fic_real(buffer);
                    for (int i = 0; i < str.length(); ++i)
                    {
                        new (reinterpret_cast<char *>(cur_address)) char(str[i]);
                        cur_address++;
                    }
                }
                else if (reg[2] == 9)
                {
                    // sbrk
                    int amount = reg[4];
                    reg[2] = real_fic(end_dynamic);
                    end_dynamic += amount;
                }
                else if (reg[2] == 10)
                {
                    // exit
                    return;
                }
                else if (reg[2] == 11)
                {
                    // print_char
                    outFile << (char)reg[4] << flush;
                }
                else if (reg[2] == 12)
                {
                    // read_char
                    char c;
                    input_syscall >> c;
                    reg[2] = c;
                }
                else if (reg[2] == 13)
                {
                    // open
                    string filename = *(reinterpret_cast<string *>(fic_real(reg[4])));
                    int flags = reg[5];
                    int mode = reg[6];
                    // undone
                }
                else if (reg[2] == 14)
                {
                    // read
                    int file_descriptor = reg[4];
                    int buffer = reg[5];
                    int length = reg[6];
                    // undone
                }
                else if (reg[2] == 15)
                {
                    cout << "write" << endl;
                    // write
                    int file_descriptor = reg[4];
                    int buffer = reg[5];
                    int length = reg[6];
                    uint64 cur_address = fic_real(buffer);
                    string str;
                    for (int i = 0; i < length - 1; ++i)
                    {
                        char c = *(reinterpret_cast<char *>(cur_address));
                        if (c == '\0')
                            break;
                        str = str + c;
                        cur_address++;
                    }
                    outFile << str << flush;
                }
                else if (reg[2] == 16)
                {
                    // close
                    int file_descriptor = reg[4];
                    // undone
                }
                else if (reg[2] == 17)
                {
                    // exit2
                    int result = reg[4];
                    return;
                }
            }
        }
        else if (op == 0x1c)
        {
            int funct = code & mask6;
            int rd = (code >> 11) & mask5;
            int rs = (code >> 21) & mask5;
            int rt = (code >> 16) & mask5;
            int shamt = (code >> 6) & mask5;
            if (funct == 2)
            {
                // mul
                reg[rd] = reg[rs] * reg[rt];
            }
            else if (funct == 0x21)
            {
                // clo
                int i;
                for (i = 31; i >= 0; i--)
                    if ((reg[rs] >> i) & 1 == 0)
                        break;
                reg[rd] = 31 - i;
            }
            else if (funct == 0x20)
            {
                // clz
                int i;
                for (i = 31; i >= 0; i--)
                    if ((reg[rs] >> i) & 1 == 1)
                        break;
                reg[rd] = 31 - i;
            }
            else if (funct == 0)
            {
                // madd
                int64_t i1 = reg[rs] * reg[rt];
                int64_t i2 = ((int64_t)hi << 32) + lo;
                i2 = i2 + i1;
                lo = i2 & mask32;
                hi = (i2 >> 32) & mask32;
            }
            else if (funct == 4)
            {
                // msub
                int64_t i1 = reg[rs] * reg[rt];
                int64_t i2 = ((int64_t)hi << 32) + lo;
                i2 = i2 - i1;
                lo = i2 & mask32;
                hi = (i2 >> 32) & mask32;
            }
            else if (funct == 1)
            {
                // maddu
                uint64_t i1 = reg[rs] * reg[rt];
                uint64_t i2 = ((uint64_t)hi << 32) + lo;
                i2 = i2 + i1;
                lo = i2 & mask32;
                hi = (i2 >> 32) & mask32;
            }
            else if (funct == 5)
            {
                // msubu
                uint64_t i1 = reg[rs] * reg[rt];
                uint64_t i2 = ((uint64_t)hi << 32) + lo;
                i2 = i2 - i1;
                lo = i2 & mask32;
                hi = (i2 >> 32) & mask32;
            }
        }
        else if (op == 2)
        {
            // j
            int target_address = (code & mask26) << 2;
            PC = fic_real(target_address);
        }
        else if (op == 3)
        {
            // jal
            int target_address = (code & mask26) << 2;
            reg[31] = real_fic(PC);
            PC = fic_real(target_address);
        }
        else // I format
        {
            int imm = code & mask16;
            int rs = (code >> 21) & mask5;
            int rt = (code >> 16) & mask5;
            if (op == 8)
            {
                // addi
                reg[rt] = reg[rs] + (int16_t)imm;
            }
            else if (op == 9)
            {
                // addiu
                reg[rt] = (uint32_t)reg[rs] + (uint16_t)imm;
            }
            else if (op == 0xc)
            {
                // andi
                reg[rt] = reg[rs] & imm;
            }
            else if (op == 0xd)
            {
                // ori
                reg[rt] = reg[rs] | imm;
            }
            else if (op == 0xe)
            {
                // xori
                reg[rt] = reg[rs] ^ imm;
            }
            else if (op == 0x20)
            {
                // lb
                reg[rt] = *(reinterpret_cast<int8_t *>(fic_real(imm + reg[rs])));
            }
            else if (op == 0x24)
            {
                // lbu
                reg[rt] = *(reinterpret_cast<uint8_t *>(fic_real(imm + reg[rs])));
            }
            else if (op == 0x21)
            {
                // lh
                reg[rt] = *(reinterpret_cast<int16_t *>(fic_real(imm + reg[rs])));
            }
            else if (op == 0x25)
            {
                // lhu
                reg[rt] = *(reinterpret_cast<uint16_t *>(fic_real(imm + reg[rs])));
            }
            else if (op == 0x23)
            {
                // lw
                reg[rt] = *(reinterpret_cast<int32_t *>(fic_real(imm + reg[rs])));
            }
            else if (op == 0x22)
            {
                // lwl
                int tmp = *(reinterpret_cast<int32_t *>(fic_real(imm + reg[rs])));
                int tmp_bytes = 4 - (imm + reg[rs] - 1) % 4;
                int tmp_mask = (1 << ((4 - tmp_bytes) << 2)) - 1;
                reg[rt] = (reg[rt] & tmp_mask) | (tmp & (~tmp_mask));
            }
            else if (op == 0x26)
            {
                // lwr
                int tmp = *(reinterpret_cast<int32_t *>(fic_real(imm + reg[rs] - 3)));
                int tmp_bytes = (imm + reg[rs]) % 4;
                int tmp_mask = (1 << (tmp_bytes << 2)) - 1;
                reg[rt] = (reg[rt] & (~tmp_mask)) | (tmp & tmp_mask);
            }
            else if (op == 0x30)
            {
                // ll
                reg[rt] = *(reinterpret_cast<int32_t *>(fic_real(imm + reg[rs])));
            }
            else if (op == 0x28)
            {
                // sb
                *(reinterpret_cast<int8_t *>(fic_real(imm + reg[rs]))) = (int8_t)(reg[rt] & mask8);
            }
            else if (op == 0x29)
            {
                // sh
                *(reinterpret_cast<int16_t *>(fic_real(imm + reg[rs]))) = (int16_t)(reg[rt] & mask16);
            }
            else if (op == 0x2b)
            {
                // sw
                *(reinterpret_cast<int32_t *>(fic_real(imm + reg[rs]))) = (int32_t)(reg[rt]);
            }
            else if (op == 0x2a)
            {
                // swl
                int cur_address = imm + reg[rs];
                for (int a = cur_address, b = 0; a < ((cur_address >> 2) + 1) << 2; ++a, b += 4)
                    *(reinterpret_cast<int8_t *>(fic_real(a))) = (int8_t)(reg[rt] >> (24 - b));
            }
            else if (op == 0x2e)
            {
                // swr
                int cur_address = imm + reg[rs];
                for (int a = cur_address, b = 0; a >= (cur_address >> 2) << 2; --a, b += 4)
                    *(reinterpret_cast<int8_t *>(fic_real(a))) = (int8_t)(reg[rt] >> (b));
            }
            else if (op == 0x38)
            {
                // sc
                *(reinterpret_cast<int32_t *>(fic_real(imm + reg[rs]))) = (int32_t)(reg[rt]);
            }
            else if (op == 0xa)
            {
                // slti
                reg[rt] = reg[rs] < (int16_t)imm;
            }
            else if (op == 0xb)
            {
                // sltiu
                reg[rt] = (uint32_t)reg[rs] < (uint16_t)imm;
            }
            else if (op == 4)
            {
                // beq
                if (reg[rs] == reg[rt])
                    PC += imm << 2;
            }
            else if (op == 5)
            {
                // bne
                if (reg[rs] != reg[rt])
                    PC += imm << 2;
            }
            else if (op == 7)
            {
                // bgtz
                if (reg[rs] > 0)
                    PC += imm << 2;
            }
            else if (op == 6)
            {
                // blez
                if (reg[rs] <= 0)
                    PC += imm << 2;
            }
            else if (op == 0xf)
            {
                // lui
                reg[rt] = imm << 16;
            }
            else if (op == 1) //  !!!!!
            {
                if (rt == 1)
                {
                    // bgez
                    if (reg[rs] >= 0)
                        PC += imm << 2;
                }
                else if (rt == 0x11)
                {
                    // bgezal
                    reg[31] = PC;
                    if (reg[rs] >= 0)
                        PC += imm << 2;
                }
                else if (rt == 0x10)
                {
                    // bltzal
                    reg[31] = PC;
                    if (reg[rs] < 0)
                        PC += imm << 2;
                }
                else if (rt == 0)
                {
                    // bltz
                    if (reg[rs] < 0)
                        PC += imm << 2;
                }
                else if (rt == 0xc)
                {
                    // teqi
                    if (reg[rs] == (int16_t)imm)
                    {
                        cout << "trap error" << endl;
                        return;
                    }
                }
                else if (rt == 0xe)
                {
                    // tnei
                    if (reg[rs] != (int16_t)imm)
                    {
                        cout << "trap error" << endl;
                        return;
                    }
                }
                else if (rt == 8)
                {
                    // tgei
                    if (reg[rs] >= (int16_t)imm)
                    {
                        cout << "trap error" << endl;
                        return;
                    }
                }
                else if (rt == 9)
                {
                    // tgeiu
                    if ((uint32_t)reg[rs] >= (uint16_t)imm)
                    {
                        cout << "trap error" << endl;
                        return;
                    }
                }
                else if (rt == 0xa)
                {
                    // tlti
                    if (reg[rs] < (int16_t)imm)
                    {
                        cout << "trap error" << endl;
                        return;
                    }
                }
                else if (rt == 0xb)
                {
                    // tltiu
                    if ((uint32_t)reg[rs] < (uint16_t)imm)
                    {
                        cout << "trap error" << endl;
                        return;
                    }
                }
            }
        }
        // output_register();
    }
}
