#include "1_Assembler.cpp"
#include "2_Simulator.cpp"
using namespace std;

int main()
{
    map<string, ins *> find_ins; // add -> machine code format
    map<string, int> find_reg;   // $s0 -> register number
    define_instruction(find_ins);
    define_register(find_reg);

    // command input
    string s0, s1, s2, s3;
    cin >> s0 >> s1 >> s2 >> s3;
    ifstream input_MIPS, input_syscall;
    ofstream outFile;
    input_MIPS.open(s0 + "/" + s1, fstream::in);
    input_syscall.open(s0 + "/" + s2, fstream::in);
    outFile.open(s0 + "/" + s3, fstream::out);

    // memory simulation
    memory_simulation();

    // loading static data
    map<string, int> varname_address;
    load_static(input_MIPS, end_static, varname_address);
    end_dynamic = end_static;

    // loading machine code
    vector<uint32_t> machine_code;
    assemble_code(input_MIPS, find_ins, find_reg, machine_code, varname_address);
    input_MIPS.close();
    load_text(machine_code, end_text);

    // print machine code in terminal
    cout << "---------machine code-----------" << endl;
    for (int i = 0; i < machine_code.size(); ++i)
        cout << bitset<32>(machine_code[i]) << endl;

    // simulation
    simulate(outFile, input_syscall);

    input_syscall.close();
    outFile.close();
    return 0;
}