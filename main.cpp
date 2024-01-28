#include <ncurses.h>
#include<iostream>
#include<math.h>
#include<string.h>
#include"RV32M.cpp"
using namespace std;

int main(int argc, char **argv) {
    RV32M rv;
    
    // printf("%s", rv.memory[0]);
    // printf("%s\n", rv.code_memory[0]);
    // rv.InstructionFetch();
    // rv.InstructionDecode();
    // rv.WriteRegister(21, "f");
    // printf("%s\n", rv.Register[21].second);
    // printf("%d\n", rv.Register[21].first);
    // rv.InstructionDecode();
    // rv.InstructionExecute();
    // printf("%d\n", BinaryToDecimal("10000000"));

    // std::string s = "110110101";
    // int si = s.size();
    // while(si++<32)
    // s = s[0] + s;

    // std::cout<<s;

    rv.InstructionDecode();
    rv.InstructionExecute();
    std::cout<<rv.Register[31].first;

    return 0;
}
