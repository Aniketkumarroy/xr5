#ifndef RV32M_COMPUTER
#define RV32M_COMPUTER

#include<utility>
#include<cstdint>
#include<math.h>
#include<string>

class RV32M
{
    private:
        uint32_t MEM_size = pow(2, 16) - 1;
        uint32_t CODE_size = pow(2, 16) - 1;
        char CODE_MEMORY_ADDRESS[9];
        std::string instruction = "ffffffff";
        std::string BinaryCode = "11111111111111111111111111111111";
        void (RV32M::*Executor)() = nullptr;
        void (*ErrorHandler)(char*);

    void LoadUpperImmediate();
    void AddUpperImmediateToPC();
    void ImmediateArithmetic();
    void RegisterArithmetic();
    void LoadFromMemory();
    void StoreToMemory();
    void JumpAndLink();
    void JumpRelativeAndLink();
    void ConditionalBranch();
    public:
        std::pair<int32_t, char[9]> Register[32];
        std::pair<uint32_t, char[9]> PC;
        char **memory, **code_memory;

    RV32M(uint32_t memory_size = 65536, uint32_t code_memory_size = 65536, void (*error)(char*) = nullptr);
    ~RV32M();
    void Error(char *msg);
    void Reset(uint32_t memory_size = 65536, uint32_t code_memory_size = 65536) ;
    void SignExtend(std::string &value, int size, bool sign);
    void WriteByteToCodeMemory(uint32_t addr, char data[3]);
    void WriteByteToCodeMemory(char addr[9], char data[3]);
    void WriteWordToCodeMemory(uint32_t addr, char data[9]);
    void WriteWordToCodeMemory(char addr[9], char data[9]);
    void WriteByteToMemory(uint32_t addr, char data[3]);
    void WriteByteToMemory(char addr[9], char data[3]);
    void WriteWordToMemory(uint32_t addr, char data[9]);
    void WriteWordToMemory(char addr[9], char data[9]);
    std::string ReadByteFromCodeMemory(uint32_t addr);
    std::string ReadByteFromCodeMemory(char addr[9]);
    std::string ReadWordFromCodeMemory(uint32_t addr);
    std::string ReadWordFromCodeMemory(char addr[9]);
    std::string ReadByteFromMemory(uint32_t addr);
    std::string ReadByteFromMemory(char addr[9]);
    std::string ReadWordFromMemory(uint32_t addr);
    std::string ReadWordFromMemory(char addr[9]);
    void WriteRegister(int reg, char data[9]);
    void WriteRegister(int reg, int32_t data);
    void PC_Increment(uint32_t bytes);
    void InstructionExecute();
    void InstructionDecode();
    void InstructionFetch();
};
#endif