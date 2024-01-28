#include<utility>
#include<cstdint>
#include<math.h>
#include<stdlib.h>
#include<unordered_map>
#include"hex.h"
#include"utilities.h"

class RV32M
{
    private:
        uint32_t MEM_size = pow(2, 16) - 1;
        uint32_t CODE_size = pow(2, 16) - 1;
        char CODE_MEMORY_ADDRESS[9];
        std::string instruction = "00009F97";
        std::string BinaryCode = "11111111111111111111111111111111";

        // std::unordered_map<std::string, void(RV32M::*)(char*)> InstructionExecutor;
        void (RV32M::*Executor)() = nullptr;
        void (*ErrorHandler)(char*);

    void LoadUpperImmediate()
    {
        std::string value = BinaryCode.substr(0, 20);
        value += "000000000000";

        char *ptr = (char*)value.c_str();

        char hex[9];
        BinarytoHexadecimal(ptr, hex);

        value = BinaryCode.substr(20, 5);
        int reg = BinaryToDecimal(ptr);

        WriteRegister(reg, hex);
    }
    
    void AddUpperImmediateToPC()
    {
        std::string value = BinaryCode.substr(0, 20);
        value += "000000000000";

        char *ptr = (char*)value.c_str();

        char hex[9], binary[33];
        int32_t data = PC.first + BinaryToSignedDecimal(ptr);
        decimalToBinary(data, binary, 32);
        BinarytoHexadecimal(binary, hex);

        value = BinaryCode.substr(20, 5);
        uint32_t reg = BinaryToDecimal(ptr);

        WriteRegister(reg, hex);
    }
    
    void ImmediateArithmetic()
    {
        std::string funct3 = BinaryCode.substr(17, 3);
        if(funct3 == "001")
        {
            // std::string value = 
        }
    }
    
    void RegisterArithmetic()
    {
        
    }
    
    void LoadFromMemory()
    {
        
    }
    
    void StoreToMemory()
    {
        
    }
    
    void JumpAndLink()
    {
        
    }
    
    void JumpRelativeAndLink()
    {
        
    }
    
    void ConditionalBranch()
    {
        
    }
    
    public:
        std::pair<int32_t, char[9]> Register[32];
        std::pair<uint32_t, char[9]> PC;

        char **memory, **code_memory;

    void Error(char *msg)
    {
        if(ErrorHandler != nullptr)
        ErrorHandler(msg);
    }

    void Reset(uint32_t memory_size = 65536, uint32_t code_memory_size = 65536)
    {
        if(memory_size != MEM_size) MEM_size = memory_size;
        if(code_memory_size != CODE_size) CODE_size = code_memory_size;

        memory = new char*[MEM_size]; // (char**)malloc(MEM_size*sizeof(char*));
        if(memory == nullptr) exit(0);
        code_memory = new char*[CODE_size]; // (char**)malloc(CODE_size*sizeof(char*));
        if(code_memory == nullptr) exit(0);

        for (uint32_t i = 0; i < MEM_size; i++)
        {
            memory[i] = new char [3]; // (char*)malloc(3*sizeof(char));
            if(memory[i] == nullptr) exit(0);
            strcpy(memory[i], "ff");
        }
        for (uint32_t i = 0; i < CODE_size; i++)
        {
            code_memory[i] = new char [3]; // (char*)malloc(3*sizeof(char));
            if(memory[i] == nullptr) exit(0);
            strcpy(code_memory[i], "ff");
        }

        PC.first = 0;
        strcpy(PC.second, "00000000");

        for(uint16_t i = 0; i< 32 ; i++)
        {
            Register[i].first = 0;
            strcpy(Register[i].second, "00000000");
        }
    }

    RV32M(uint32_t memory_size = 65536, uint32_t code_memory_size = 65536, void (*error)(char*) = nullptr)
    {
        ErrorHandler = error;
        Reset(memory_size, code_memory_size);
        
    }

    ~RV32M()
    {
        for (uint32_t i = 0; i < MEM_size; i++)
        delete(memory[i]);
        for (uint32_t i = 0; i < CODE_size; i++)
        delete(code_memory[i]);

        delete(memory);
        delete(code_memory);
    }

    void WriteByteToCodeMemory(uint32_t addr, char data[3])
    {
        if(addr > CODE_size - 1 || addr < 0)
        {
            Error("the provided address is out of range of code memory");
            return;
        }
        if(strlen(data) == 2)
        strcpy(code_memory[addr], data);
        else
        {
            char d[3];
            d[3] = '\0';
            if (strlen(data) == 1)
            {
                d[0] = '0';
                d[1] = data[0];
            }else
            {
                d[0] = '0';
                d[1] = '0';
            }
            strcpy(code_memory[addr], d);
        }
    }

    void WriteByteToCodeMemory(char addr[9], char data[3])
    {
        if(strlen(addr) > 8)
        {
            Error("the address should be 8 or less then 8 characters long");
            return;
        }
        uint32_t int_addr = hexaDecimalToDecimal(addr);
        if(int_addr > CODE_size - 1 || int_addr < 0)
        {
            Error("the provided address is out of range of code memory");
            return;
        }
        if(strlen(data) == 2)
        strcpy(code_memory[int_addr], data);
        else
        {
            char d[3];
            d[3] = '\0';
            if (strlen(data) == 1)
            {
                d[0] = '0';
                d[1] = data[0];
            }else
            {
                d[0] = '0';
                d[1] = '0';
            }
            strcpy(code_memory[int_addr], d);
        }
    }

    void WriteByteToMemory(uint32_t addr, char data[3])
    {
        if(addr > MEM_size - 1 || addr < 0)
        {
            Error("the provided address is out of range of code memory");
            return;
        }
        if(strlen(data) == 2)
        strcpy(memory[addr], data);
        else
        {
            char d[3];
            d[3] = '\0';
            if (strlen(data) == 1)
            {
                d[0] = '0';
                d[1] = data[0];
            }else
            {
                d[0] = '0';
                d[1] = '0';
            }
            strcpy(memory[addr], d);
        }
    }

    void WriteByteToMemory(char addr[9], char data[3])
    {
        if(strlen(addr) > 8)
        {
            Error("the address should be 8 or less then 8 characters long");
            return;
        }
        uint32_t int_addr = hexaDecimalToDecimal(addr);
        if(int_addr > MEM_size - 1 || int_addr < 0)
        {
            Error("the provided address is out of range of code memory");
            return;
        }
        if(strlen(data) == 2)
        strcpy(memory[int_addr], data);
        else
        {
            char d[3];
            d[3] = '\0';
            if (strlen(data) == 1)
            {
                d[0] = '0';
                d[1] = data[0];
            }else
            {
                d[0] = '0';
                d[1] = '0';
            }
            strcpy(memory[int_addr], d);
        }
    }

    void WriteRegister(int reg, char data[9])
    {
        if(reg > 31 || reg < 0)
        {
            Error("the provided register does not exist in RV32M");
            return;
        }
        if(reg != 0)
        {
            if(strlen(data) == 8)
            {
                strcpy(Register[reg].second, data);
                Register[reg].first = hexaDecimalToSignedDecimal(data, 32);
            }
            else
            {
                char d[9];
                d[0] = '\0';
                int s = strlen(data);
                while(s++ < 8)
                strcat(d, "0");

                strcat(d, data);
                strcpy(Register[reg].second, d);
                Register[reg].first = hexaDecimalToSignedDecimal(d, 32);
            }
        }
    }

    void PC_Increment(uint32_t bytes)
    {
        PC.first += bytes;
        char binary[33];
        decimalToBinary(PC.first, binary, 32);
        BinarytoHexadecimal(binary, PC.second);
    }


    void InstructionExecute()
    {
        if(Executor != nullptr)
        (this->*Executor)();
    }
    void InstructionDecode()
    {
        char binary[33];
        char *hex = (char*)instruction.c_str();
        HexadecimalToBinary(hex, binary, 32);
        BinaryCode = binary;
        const char *opcode = BinaryCode.c_str() + 25;

        if(strcmp(opcode,"0110111") == 0) Executor = &RV32M::LoadUpperImmediate;
        else if(strcmp(opcode,"0010111") == 0) Executor = &RV32M::AddUpperImmediateToPC;
        else if(strcmp(opcode,"0010011") == 0) Executor = &RV32M::ImmediateArithmetic;
        else if(strcmp(opcode,"0110011") == 0) Executor = &RV32M::RegisterArithmetic;
        else if(strcmp(opcode,"0000011") == 0) Executor = &RV32M::LoadFromMemory;
        else if(strcmp(opcode,"0100011") == 0) Executor = &RV32M::StoreToMemory;
        else if(strcmp(opcode,"1101111") == 0) Executor = &RV32M::JumpAndLink;
        else if(strcmp(opcode,"1100111") == 0) Executor = &RV32M::JumpRelativeAndLink;
        else if(strcmp(opcode,"1100011") == 0) Executor = &RV32M::ConditionalBranch;
        else Executor = nullptr;
    }

    void InstructionFetch()
    {
        instruction  = code_memory[PC.first];
        instruction += code_memory[PC.first + 1];
        instruction += code_memory[PC.first + 2];
        instruction += code_memory[PC.first + 3];
        PC_Increment(4);
    }
};