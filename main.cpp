#include <ncurses.h>
#include<iostream>
#include<math.h>
#include<string.h>
#include"RV32M.h"
using namespace std;

int main(int argc, char **argv) {
    RV32M rv;

    uint32_t addr = 0;
    rv.WriteWordToCodeMemory(addr, "300093");
    rv.WriteWordToCodeMemory(addr+4, "fff00113");
    rv.WriteWordToCodeMemory(addr+8, "401151b3");

    addr = 0;

    std::cout<<rv.ReadWordFromCodeMemory(addr)<<"\n";
    std::cout<<rv.ReadWordFromCodeMemory(addr + 4)<<"\n";
    std::cout<<rv.ReadWordFromCodeMemory(addr + 8)<<"\n";

    rv.InstructionFetch();
    rv.InstructionDecode();
    rv.InstructionExecute();
    
    rv.InstructionFetch();
    rv.InstructionDecode();
    rv.InstructionExecute();
    
    rv.InstructionFetch();
    rv.InstructionDecode();
    rv.InstructionExecute();

    for(int i = 0; i< 32; i++)
    printf("r%d: %s, %d\n", i, rv.Register[i].second, rv.Register[i].first);

    return 0;
}
