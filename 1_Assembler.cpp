#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include <typeinfo>
#include <cstdio>
#include <bitset>
#define starting_address 0x400000
#define mask16 65535
#define mask26 67108863
#define mask6 63
using namespace std;

class ins
{
public:
    int op;
    string catagory;
    virtual string get_class_type()
    {
        return "?";
    }
};

class R_ins : public ins
{
public:
    int rs, rt, rd, shamt, funct;
    R_ins(int op, int rs, int rt, int rd, int shamt, int funct, string catagory)
    {
        this->op = op;
        this->rs = rs;
        this->rt = rt;
        this->rd = rd;
        this->shamt = shamt;
        this->funct = funct;
        this->catagory = catagory;
    }
    R_ins(const R_ins &i1)
    {
        this->op = i1.op;
        this->rs = i1.rs;
        this->rt = i1.rt;
        this->rd = i1.rd;
        this->shamt = i1.shamt;
        this->funct = i1.funct;
        this->catagory = i1.catagory;
    }
    string get_class_type()
    {
        return "R";
    }
};

class I_ins : public ins
{
public:
    int rs, rt, imm;
    I_ins(int op, int rs, int rt, int imm, string catagory)
    {
        this->op = op;
        this->rs = rs;
        this->rt = rt;
        this->imm = imm;
        this->catagory = catagory;
    }
    I_ins(const I_ins &i1)
    {
        this->op = i1.op;
        this->rs = i1.rs;
        this->rt = i1.rt;
        this->imm = i1.imm;
        this->catagory = i1.catagory;
    }
    string get_class_type()
    {
        return "I";
    }
};

class J_ins : public ins
{
public:
    int address;
    J_ins(int op, int address, string catagory)
    {
        this->op = op;
        this->address = address;
        this->catagory = catagory;
    }
    J_ins(const J_ins &i1)
    {
        this->op = i1.op;
        this->address = i1.address;
        this->catagory = i1.catagory;
    }
    string get_class_type()
    {
        return "J";
    }
};

// defininition

void define_instruction(map<string, ins *> &find_ins)
{
    int n = 0; // unknown value

    find_ins["add"] = new R_ins(0, n, n, n, 0, 0x20, "Arithmetic_Logical");
    find_ins["addu"] = new R_ins(0, n, n, n, 0, 0x21, "Arithmetic_Logical");
    find_ins["and"] = new R_ins(0, n, n, n, 0, 0x24, "Arithmetic_Logical");
    find_ins["mul"] = new R_ins(0x1c, n, n, n, 0, 2, "Arithmetic_Logical");
    find_ins["nor"] = new R_ins(0, n, n, n, 0, 0x27, "Arithmetic_Logical");
    find_ins["or"] = new R_ins(0, n, n, n, 0, 0x25, "Arithmetic_Logical");
    find_ins["sllv"] = new R_ins(0, n, n, n, 0, 4, "Arithmetic_Logical");
    find_ins["srav"] = new R_ins(0, n, n, n, 0, 7, "Arithmetic_Logical");
    find_ins["srlv"] = new R_ins(0, n, n, n, 0, 6, "Arithmetic_Logical");
    find_ins["sub"] = new R_ins(0, n, n, n, 0, 0x22, "Arithmetic_Logical");
    find_ins["subu"] = new R_ins(0, n, n, n, 0, 0x23, "Arithmetic_Logical");
    find_ins["xor"] = new R_ins(0, n, n, n, 0, 0x26, "Arithmetic_Logical");
    find_ins["slt"] = new R_ins(0, n, n, n, 0, 0x2a, "Comparison");
    find_ins["sltu"] = new R_ins(0, n, n, n, 0, 0x2b, "Comparison");
    find_ins["clo"] = new R_ins(0x1c, n, 0, n, 0, 0x21, "Arithmetic_Logical");
    find_ins["clz"] = new R_ins(0x1c, n, 0, n, 0, 0x20, "Arithmetic_Logical");
    find_ins["jalr"] = new R_ins(0, n, 0, n, 0, 9, "Jump");
    find_ins["div"] = new R_ins(0, n, n, 0, 0, 0x1a, "Arithmetic_Logical");
    find_ins["divu"] = new R_ins(0, n, n, 0, 0, 0x1b, "Arithmetic_Logical");
    find_ins["mult"] = new R_ins(0, n, n, 0, 0, 0x18, "Arithmetic_Logical");
    find_ins["multu"] = new R_ins(0, n, n, 0, 0, 0x19, "Arithmetic_Logical");
    find_ins["madd"] = new R_ins(0x1c, n, n, 0, 0, 0, "Arithmetic_Logical");
    find_ins["msub"] = new R_ins(0x1c, n, n, 0, 0, 4, "Arithmetic_Logical");
    find_ins["maddu"] = new R_ins(0x1c, n, n, 0, 0, 1, "Arithmetic_Logical");
    find_ins["msubu"] = new R_ins(0x1c, n, n, 0, 0, 5, "Arithmetic_Logical");
    find_ins["teq"] = new R_ins(0, n, n, 0, 0, 0x34, "Trap");
    find_ins["tne"] = new R_ins(0, n, n, 0, 0, 0x36, "Trap");
    find_ins["tge"] = new R_ins(0, n, n, 0, 0, 0x30, "Trap");
    find_ins["tgeu"] = new R_ins(0, n, n, 0, 0, 0x31, "Trap");
    find_ins["tlt"] = new R_ins(0, n, n, 0, 0, 0x32, "Trap");
    find_ins["tltu"] = new R_ins(0, n, n, 0, 0, 0x33, "Trap");
    find_ins["sll"] = new R_ins(0, n, n, n, n, 0, "Arithmetic_Logical");
    find_ins["srl"] = new R_ins(0, n, n, n, n, 2, "Arithmetic_Logical");
    find_ins["sra"] = new R_ins(0, n, n, n, n, 3, "Arithmetic_Logical");
    find_ins["jr"] = new R_ins(0, n, 0, 0, 0, 8, "Jump");
    find_ins["mthi"] = new R_ins(0, n, 0, 0, 0, 0x11, "Data_Movement");
    find_ins["mtlo"] = new R_ins(0, n, 0, 0, 0, 0x13, "Data_Movement");
    find_ins["mfhi"] = new R_ins(0, 0, 0, n, 0, 0x10, "Data_Movement");
    find_ins["mflo"] = new R_ins(0, 0, 0, n, 0, 0x12, "Data_Movement");
    find_ins["syscall"] = new R_ins(0, 0, 0, 0, 0, 0xc, "Exception");
    find_ins["addi"] = new I_ins(8, n, n, n, "Arithmetic_Logical");
    find_ins["addiu"] = new I_ins(9, n, n, n, "Arithmetic_Logical");
    find_ins["andi"] = new I_ins(0xc, n, n, n, "Arithmetic_Logical");
    find_ins["ori"] = new I_ins(0xd, n, n, n, "Arithmetic_Logical");
    find_ins["xori"] = new I_ins(0xe, n, n, n, "Arithmetic_Logical");
    find_ins["lb"] = new I_ins(0x20, n, n, n, "Load");
    find_ins["lbu"] = new I_ins(0x24, n, n, n, "Load");
    find_ins["lh"] = new I_ins(0x21, n, n, n, "Load");
    find_ins["lhu"] = new I_ins(0x25, n, n, n, "Load");
    find_ins["lw"] = new I_ins(0x23, n, n, n, "Load");
    find_ins["lwl"] = new I_ins(0x22, n, n, n, "Load");
    find_ins["lwr"] = new I_ins(0x26, n, n, n, "Load");
    find_ins["ll"] = new I_ins(0x30, n, n, n, "Load");
    find_ins["sb"] = new I_ins(0x28, n, n, n, "store");
    find_ins["sh"] = new I_ins(0x29, n, n, n, "store");
    find_ins["sw"] = new I_ins(0x2b, n, n, n, "store");
    find_ins["swl"] = new I_ins(0x2a, n, n, n, "store");
    find_ins["swr"] = new I_ins(0x2e, n, n, n, "store");
    find_ins["sc"] = new I_ins(0x38, n, n, n, "store");
    find_ins["slti"] = new I_ins(0xa, n, n, n, "Comparison");
    find_ins["sltiu"] = new I_ins(0xb, n, n, n, "Comparison");
    find_ins["beq"] = new I_ins(4, n, n, n, "Branch");
    find_ins["bne"] = new I_ins(5, n, n, n, "Branch");
    find_ins["lui"] = new I_ins(0xf, 0, n, n, "Constant_Manipulating");
    find_ins["bgez"] = new I_ins(1, n, 1, n, "Branch");
    find_ins["bgezal"] = new I_ins(1, n, 0x11, n, "Branch");
    find_ins["bgtz"] = new I_ins(7, n, 0, n, "Branch");
    find_ins["blez"] = new I_ins(6, n, 0, n, "Branch");
    find_ins["bltzal"] = new I_ins(1, n, 0x10, n, "Branch");
    find_ins["bltz"] = new I_ins(1, n, 0, n, "Branch");
    find_ins["teqi"] = new I_ins(1, n, 0xc, n, "Trap");
    find_ins["tnei"] = new I_ins(1, n, 0xe, n, "Trap");
    find_ins["tgei"] = new I_ins(1, n, 8, n, "Trap");
    find_ins["tgeiu"] = new I_ins(1, n, 9, n, "Trap");
    find_ins["tlti"] = new I_ins(1, n, 0xa, n, "Trap");
    find_ins["tltiu"] = new I_ins(1, n, 0xb, n, "Trap");
    find_ins["j"] = new J_ins(2, n, "Jump");
    find_ins["jal"] = new J_ins(3, n, "Jump");
}

void define_register(map<string, int> &find_reg)
{
    find_reg["$zero"] = 0;
    find_reg["$at"] = 1;
    find_reg["$v0"] = 2;
    find_reg["$v1"] = 3;
    find_reg["$a0"] = 4;
    find_reg["$a1"] = 5;
    find_reg["$a2"] = 6;
    find_reg["$a3"] = 7;
    find_reg["$t0"] = 8;
    find_reg["$t1"] = 9;
    find_reg["$t2"] = 10;
    find_reg["$t3"] = 11;
    find_reg["$t4"] = 12;
    find_reg["$t5"] = 13;
    find_reg["$t6"] = 14;
    find_reg["$t7"] = 15;
    find_reg["$s0"] = 16;
    find_reg["$s1"] = 17;
    find_reg["$s2"] = 18;
    find_reg["$s3"] = 19;
    find_reg["$s4"] = 20;
    find_reg["$s5"] = 21;
    find_reg["$s6"] = 22;
    find_reg["$s7"] = 23;
    find_reg["$t8"] = 24;
    find_reg["$t9"] = 25;
    find_reg["$k0"] = 26;
    find_reg["$k1"] = 27;
    find_reg["$gp"] = 28;
    find_reg["$sp"] = 29;
    find_reg["$fp"] = 30;
    find_reg["$ra"] = 31;
}

// assembling

void read_lines(ifstream &inFile, map<string, int> &label_address, vector<string> &code)
{
    int cur_address = starting_address;
    string line;
    while (getline(inFile, line))
    {
        int comment_pos = line.find("#"); // delete comment
        if (comment_pos != string::npos)
            line = line.substr(0, comment_pos);
        line = line.substr(0, line.find_last_not_of(' ') + 1);
        if (line.find(":") != string::npos) // labels have :
        {
            int pos1 = line.find(":");
            string label = line.substr(0, pos1);
            label_address[label] = cur_address;
            line = line.substr(pos1 + 1);
        }
        if (line.find_first_not_of(' ') == string::npos) // skip blank lines
            continue;
        else
            line = line.substr(line.find_first_not_of(' '));
        code.push_back(line); // store code
        cur_address += 4;
    }
}

void assemble_code(ifstream &inFile,
                   const map<string, ins *> &find_ins, const map<string, int> &find_reg,
                   vector<uint32_t> &machine_code, const map<string, int> &varname_address)
{
    // round 1
    vector<string> code;
    map<string, int> label_address; // label
    read_lines(inFile, label_address, code);
    // round 2
    int cur_address = starting_address; // fiction.
    for (int k = 0; k < code.size(); ++k, cur_address += 4)
    {
        stringstream ss;
        string xxx;
        ss << code[k];
        ss >> xxx;
        ins *i1 = find_ins.at(xxx);
        if (i1->get_class_type() == "R")
        {
            R_ins i2 = R_ins(*(R_ins *)i1);
            if (xxx == "add" || xxx == "addu" || xxx == "and" || xxx == "mul" || xxx == "nor" || xxx == "or" || xxx == "sub" || xxx == "subu" || xxx == "xor" || xxx == "slt" || xxx == "sltu")
            {
                // xxx rd rs rt
                string s1, s2, s3;
                ss >> s1 >> s2 >> s3;
                i2.rd = find_reg.at(s1.substr(0, s1.find(",")));
                i2.rs = find_reg.at(s2.substr(0, s2.find(",")));
                i2.rt = find_reg.at(s3);
            }
            else if (xxx == "sllv" || xxx == "srav" || xxx == "srlv")
            {
                // xxx rd rt rs
                string s1, s2, s3;
                ss >> s1 >> s2 >> s3;
                i2.rd = find_reg.at(s1.substr(0, s1.find(",")));
                i2.rt = find_reg.at(s2.substr(0, s2.find(",")));
                i2.rs = find_reg.at(s3);
            }
            else if (xxx == "clo" || xxx == "clz")
            {
                // xxx rd rs
                string s1, s2;
                ss >> s1 >> s2;
                i2.rd = find_reg.at(s1.substr(0, s1.find(",")));
                i2.rs = find_reg.at(s2);
            }
            else if (xxx == "jalr")
            {
                // xxx rs rd
                string s1, s2;
                ss >> s1 >> s2;
                i2.rs = find_reg.at(s1.substr(0, s1.find(",")));
                i2.rd = find_reg.at(s2);
            }
            else if (xxx == "div" || xxx == "divu" || xxx == "mult" || xxx == "multu" || xxx == "madd" || xxx == "msub" || xxx == "maddu" || xxx == "msubu" || xxx == "teq" || xxx == "tne" || xxx == "tge" || xxx == "tgeu" || xxx == "tlt" || xxx == "tltu")
            {
                // xxx rs rt
                string s1, s2;
                ss >> s1 >> s2;
                i2.rs = find_reg.at(s1.substr(0, s1.find(",")));
                i2.rt = find_reg.at(s2);
            }
            else if (xxx == "sll" || xxx == "srl" || xxx == "sra")
            {
                // xxx rd rt shamt
                string s1, s2;
                int s3;
                ss >> s1 >> s2 >> s3;
                i2.rd = find_reg.at(s1.substr(0, s1.find(",")));
                i2.rt = find_reg.at(s2.substr(0, s2.find(",")));
                i2.shamt = s3;
            }
            else if (xxx == "jr" || xxx == "mthi" || xxx == "mtlo")
            {
                // xxx rs
                string s1;
                ss >> s1;
                i2.rs = find_reg.at(s1);
            }
            else if (xxx == "mfhi" || xxx == "mflo")
            {
                // xxx rd
                string s1;
                ss >> s1;
                i2.rd = find_reg.at(s1);
            }
            else if (xxx == "syscall")
            {
                // xxx
            }
            machine_code.push_back((i2.op << 26) + (i2.rs << 21) + (i2.rt << 16) + (i2.rd << 11) + (i2.shamt << 6) + (i2.funct & mask6));
        }
        else if (i1->get_class_type() == "I")
        {
            I_ins i2 = I_ins(*(I_ins *)i1);
            if (xxx == "addi" || xxx == "addiu" || xxx == "andi" || xxx == "ori" || xxx == "xori" || xxx == "slti" || xxx == "sltiu")
            {
                // xxx rt rs imm
                string s1, s2;
                int s3;
                ss >> s1 >> s2 >> s3;
                i2.rt = find_reg.at(s1.substr(0, s1.find(",")));
                i2.rs = find_reg.at(s2.substr(0, s2.find(",")));
                i2.imm = s3;
            }
            else if (xxx == "beq" || xxx == "bne")
            {
                // xxx rs rt label
                string s1, s2;
                int s3;
                ss >> s1 >> s2;
                ss >> s3;
                if (ss.fail())
                {
                    ss.clear();
                    string s4;
                    ss >> s4;
                    s3 = (label_address.at(s4) - cur_address) >> 2;
                    // cout << s1 << s2 << s3 << endl;
                }
                i2.rs = find_reg.at(s1.substr(0, s1.find(",")));
                i2.rt = find_reg.at(s2.substr(0, s2.find(",")));
                i2.imm = s3 - 1; // offset 存字长, 涉及编码内存
            }
            else if (xxx == "bgez" || xxx == "bgezal" || xxx == "bgtz" || xxx == "blez" || xxx == "bltzal" || xxx == "bltz")
            {
                // xxx rs label
                string s1, s2;
                ss >> s1 >> s2;
                i2.rs = find_reg.at(s1.substr(0, s1.find(",")));
                i2.imm = ((label_address.at(s2) - cur_address) >> 2) - 1;
            }
            else if (xxx == "lui")
            {
                //xxx rt imm ()
                string s1;
                int s2;
                ss >> s1 >> s2;
                i2.rt = find_reg.at(s1.substr(0, s1.find(",")));
                i2.imm = s2;
            }
            else if (xxx == "lb" || xxx == "lbu" || xxx == "lh" || xxx == "lhu" || xxx == "lw" || xxx == "lwl" || xxx == "lwr" || xxx == "ll" || xxx == "sb" || xxx == "sh" || xxx == "sw" || xxx == "swl" || xxx == "swr" || xxx == "sc")
            {
                // xxx rt imm(rs) -> xxx rt rs imm
                string s1, s2;
                int s3;
                ss >> s1 >> s2;
                if (s2.find('(') != string::npos)
                {
                    string s4 = s2;
                    int pos1 = s4.find('('), pos2 = s4.find(')');
                    s2 = s4.substr(pos1 + 1, pos2 - pos1 - 1);
                    s3 = stoi(s4.substr(0, pos1), nullptr, 10);
                }
                i2.rt = find_reg.at(s1.substr(0, s1.find(",")));
                i2.rs = find_reg.at(s2);
                i2.imm = s3; // immediate 存字节, 涉及计算机内存
            }
            else if (xxx == "teqi" || xxx == "tnei" || xxx == "tgei" || xxx == "tgeiu" || xxx == "tlti" || xxx == "tltiu")
            {
                // xxx rs imm
                string s1;
                int s2;
                ss >> s1 >> s2;
                i2.rs = find_reg.at(s1.substr(0, s1.find(",")));
                i2.imm = s2;
            }
            machine_code.push_back((i2.op << 26) + (i2.rs << 21) + (i2.rt << 16) + (i2.imm & mask16));
        }
        else // "j", "jal"
        {
            J_ins i2 = J_ins(*(J_ins *)i1);
            int s1;
            ss >> s1;
            if (ss.fail())
            {
                ss.clear();
                string s4;
                ss >> s4;
                s1 = label_address.at(s4);
            }
            i2.address = s1 >> 2;
            machine_code.push_back((i2.op << 26) + (i2.address & mask26));
        }
    }
}
