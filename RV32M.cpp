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
        std::string instruction = "ffffffff";
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

        int32_t data = PC.first + BinaryToSignedDecimal(ptr);
        
        value = BinaryCode.substr(20, 5);
        uint32_t reg = BinaryToDecimal(ptr);

        WriteRegister(reg, data);
    }
    
    void ImmediateArithmetic()
    {
        std::string funct3 = BinaryCode.substr(17, 3);

        std::string r = BinaryCode.substr(12, 5);
        char *ptr = (char*)r.c_str();
        uint32_t rs1 = BinaryToDecimal(ptr);

        r = BinaryCode.substr(20, 5);
        uint32_t rd = BinaryToDecimal(ptr);
        if(funct3 == "000") // addi
        {
            std::string &Immediatevalue = funct3; // since funct3 is no longer in use, we will use its resources for oher purposes
            Immediatevalue = BinaryCode.substr(0, 12);
            SignExtend(Immediatevalue, 32, true);
            ptr = (char*)Immediatevalue.c_str();
            int32_t data = Register[rs1].first + BinaryToSignedDecimal(ptr);
            WriteRegister(rd, data);
        }
        else if(funct3 == "010") //slti
        {
            std::string &Immediatevalue = funct3; // since funct3 is no longer in use, we will use its resources for oher purposes
            Immediatevalue = BinaryCode.substr(0, 12);
            SignExtend(Immediatevalue, 32, true);
            ptr = (char*)Immediatevalue.c_str();
            int32_t data = Register[rs1].first < BinaryToSignedDecimal(ptr);
            WriteRegister(rd, data);
        }
        else if(funct3 == "011") //sltiu
        {
            std::string &Immediatevalue = funct3; // since funct3 is no longer in use, we will use its resources for oher purposes
            Immediatevalue = BinaryCode.substr(0, 12);
            SignExtend(Immediatevalue, 32, true);
            ptr = (char*)Immediatevalue.c_str();
            uint32_t unsigned_rs1 = hexaDecimalToDecimal(Register[rs1].second);
            int32_t data = unsigned_rs1 < BinaryToDecimal(ptr);
            WriteRegister(rd, data);
        }
        else if(funct3 == "100") // xori
        {
            std::string &Immediatevalue = funct3; // since funct3 is no longer in use, we will use its resources for oher purposes
            Immediatevalue = BinaryCode.substr(0, 12);
            SignExtend(Immediatevalue, 32, true);
            ptr = (char*)Immediatevalue.c_str();
            int32_t data = Register[rs1].first ^ BinaryToSignedDecimal(ptr);
            WriteRegister(rd, data);
        }
        else if(funct3 == "110") // ori
        {
            std::string &Immediatevalue = funct3; // since funct3 is no longer in use, we will use its resources for oher purposes
            Immediatevalue = BinaryCode.substr(0, 12);
            SignExtend(Immediatevalue, 32, true);
            ptr = (char*)Immediatevalue.c_str();
            int32_t data = Register[rs1].first | BinaryToSignedDecimal(ptr);
            WriteRegister(rd, data);
        }
        else if(funct3 == "111") // andi
        {
            std::string &Immediatevalue = funct3; // since funct3 is no longer in use, we will use its resources for oher purposes
            Immediatevalue = BinaryCode.substr(0, 12);
            SignExtend(Immediatevalue, 32, true);
            ptr = (char*)Immediatevalue.c_str();
            int32_t data = Register[rs1].first & BinaryToSignedDecimal(ptr);
            WriteRegister(rd, data);
        }
        else if(funct3 == "001") // slli
        {
            std::string &Immediatevalue = funct3; // since funct3 is no longer in use, we will use its resources for oher purposes
            Immediatevalue = BinaryCode.substr(7, 5);
            ptr = (char*)Immediatevalue.c_str();
            int32_t data = Register[rs1].first << BinaryToDecimal(ptr);
            WriteRegister(rd, data);
        }
        else if(funct3 == "101") // right shift
        {
            if(BinaryCode[1] == '1') // srai
            {
                std::string &Immediatevalue = funct3; // since funct3 is no longer in use, we will use its resources for oher purposes
                Immediatevalue = BinaryCode.substr(7, 5);
                ptr = (char*)Immediatevalue.c_str();
                uint32_t shift = BinaryToDecimal(ptr);
                int32_t data = (Register[rs1].first >> shift) | (~((~0) >> shift)); // from chatgpt
                // int32_t data = Register[rs1].first >> BinaryToDecimal(ptr);
                WriteRegister(rd, data);
            }
            else // srli
            {
                std::string &Immediatevalue = funct3; // since funct3 is no longer in use, we will use its resources for oher purposes
                Immediatevalue = BinaryCode.substr(7, 5);
                ptr = (char*)Immediatevalue.c_str();
                uint32_t data = static_cast<unsigned int>(Register[rs1].first) >> BinaryToDecimal(ptr); // from chatgpt
                // uint32_t data = Register[rs1].first >> BinaryToDecimal(ptr);
                WriteRegister(rd, data);
            }
        }
    }
    
    void RegisterArithmetic()
    {
        std::string funct3 = BinaryCode.substr(17, 3);

        std::string r = BinaryCode.substr(7, 5);
        char *ptr = (char*)r.c_str();
        uint32_t rs2 = BinaryToDecimal(ptr);

        r = BinaryCode.substr(12, 5);
        uint32_t rs1 = BinaryToDecimal(ptr);
        
        r = BinaryCode.substr(20, 5);
        uint32_t rd = BinaryToDecimal(ptr);
        if(funct3 == "000") // add, sub, mul
        {
            if(BinaryCode[6] == '0')
            {
                if(BinaryCode[1] == '1') // add
                {
                    int32_t data = Register[rs1].first - Register[rs2].first;
                    WriteRegister(rd, data);
                }
                else // sub
                {
                    int32_t data = Register[rs1].first + Register[rs2].first;
                    WriteRegister(rd, data);
                }
            }
            else // mul
            {
                int32_t product = static_cast<int32_t>(static_cast<int64_t>(Register[rs1].first) * static_cast<int64_t>(Register[rs2].first));
                WriteRegister(rd, product);
            }
        }
        else if(funct3 == "010") // slt, mulhsu
        {
            if(BinaryCode[6] == '0')
            {
                int32_t data = Register[rs1].first < Register[rs2].first;
                WriteRegister(rd, data);
            }
            else // mulhsu
            {
                uint32_t unsigned_rs2 = hexaDecimalToDecimal(Register[rs2].second);
                int64_t product = static_cast<int64_t>(Register[rs1].first) * static_cast<uint64_t>(unsigned_rs2);
                int32_t result = static_cast<int32_t>(product >> 32);
                WriteRegister(rd, result);
            }
        }
        else if(funct3 == "011") // sltu, mulhu
        {
            if(BinaryCode[6] == '0')
            {
                uint32_t unsigned_rs1 = hexaDecimalToDecimal(Register[rs1].second);
                uint32_t unsigned_rs2 = hexaDecimalToDecimal(Register[rs2].second);
                int32_t data = unsigned_rs1 < unsigned_rs2;
                WriteRegister(rd, data);
            }
            else // mulhu
            {
                uint32_t unsigned_rs1 = hexaDecimalToDecimal(Register[rs1].second);
                uint32_t unsigned_rs2 = hexaDecimalToDecimal(Register[rs2].second);
                int64_t product = static_cast<uint64_t>(unsigned_rs1) * static_cast<uint64_t>(unsigned_rs2);
                int32_t result = static_cast<int32_t>(product >> 32);
                WriteRegister(rd, result);
            }
        }
        else if(funct3 == "100") // xor, div
        {
            if(BinaryCode[6] == '0')
            {
                int32_t data = Register[rs1].first ^ Register[rs2].first;
                WriteRegister(rd, data);
            }
            else // div
            {
                int32_t data = Register[rs1].first / Register[rs2].first;
                WriteRegister(rd, data);
            }
        }
        else if(funct3 == "110") // or, rem
        {
            if(BinaryCode[6] == '0')
            {
                int32_t data = Register[rs1].first | Register[rs2].first;
                WriteRegister(rd, data);
            }
            else // rem
            {
                int32_t data = Register[rs1].first % Register[rs2].first;
                WriteRegister(rd, data);
            }
        }
        else if(funct3 == "111") // and, remu
        {
            if(BinaryCode[6] == '0')
            {
                int32_t data = Register[rs1].first & Register[rs2].first;
                WriteRegister(rd, data);
            }
            else // remu
            {
                uint32_t unsigned_rs1 = hexaDecimalToDecimal(Register[rs1].second);
                uint32_t unsigned_rs2 = hexaDecimalToDecimal(Register[rs2].second);
                int32_t data = unsigned_rs1 % unsigned_rs2;
                WriteRegister(rd, data);
            }
        }
        else if(funct3 == "001") // sll, mulh
        {
            if(BinaryCode[6] == '0')
            {
                int shift = Register[rs2].first & 31; // getting the lower 5 bits
                int32_t data = Register[rs1].first << shift;
                WriteRegister(rd, data);
            }
            else // mulh
            {
                int64_t product = static_cast<int64_t>(Register[rs1].first) * static_cast<int64_t>(Register[rs2].first);
                int32_t result = static_cast<int32_t>(product >> 32);
                WriteRegister(rd, result);
            }
        }
        else if(funct3 == "101") // right shift, divu
        {
            if(BinaryCode[6] == '0')
            {
                if(BinaryCode[1] == '1') // sra
                {
                    int shift = Register[rs2].first & 31; // getting the lower 5 bits
                    int32_t data = (Register[rs1].first >> shift) | (~((~0) >> shift)); // from chatgpt
                    // int32_t data = Register[rs1].first >> BinaryToDecimal(ptr);
                    WriteRegister(rd, data);
                }
                else // srl
                {
                    int shift = Register[rs2].first & 31; // getting the lower 5 bits
                    uint32_t data = static_cast<unsigned int>(Register[rs1].first) >> shift; // from chatgpt
                    // uint32_t data = Register[rs1].first >> BinaryToDecimal(ptr);
                    WriteRegister(rd, data);
                }
            }
            else // divu
            {
                uint32_t unsigned_rs1 = hexaDecimalToDecimal(Register[rs1].second);
                uint32_t unsigned_rs2 = hexaDecimalToDecimal(Register[rs2].second);
                int32_t data = unsigned_rs1 / unsigned_rs2;
                WriteRegister(rd, data);
            }
        }
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

    void SignExtend(std::string &value, int size, bool sign)
    {
        int s = value.size();
        while(s++ < size)
        {
            if(sign)
            value = value[0] + value;
            else
            value = "0" + value;
        }
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
    
    void WriteWordToCodeMemory(uint32_t addr, char data[9])
    {
        if(addr > CODE_size - 4 || addr < 0)
        {
            Error("the provided address is out of range of code memory");
            return;
        }
        std::string str;
        if(strlen(data) < 8)
        {
            char d[9];
            d[0] = '\0';
            int s = strlen(data);
            while(s++ < 8)
            strcat(d, "0");

            strcat(d, data);
            str = d;
        }else str = data;

        strcpy(code_memory[addr], str.substr(0, 2).c_str());
        strcpy(code_memory[addr + 1], str.substr(2, 2).c_str());
        strcpy(code_memory[addr + 2], str.substr(4, 2).c_str());
        strcpy(code_memory[addr + 3], str.substr(6, 2).c_str());
    }
    
    void WriteWordToCodeMemory(char addr[9], char data[9])
    {
        if(strlen(addr) > 8)
        {
            Error("the address should be 8 or less then 8 characters long");
            return;
        }
        uint32_t int_addr = hexaDecimalToDecimal(addr);
        if(int_addr > CODE_size - 4 || int_addr < 0)
        {
            Error("the provided address is out of range of code memory");
            return;
        }
        std::string str;
        if(strlen(data) < 8)
        {
            char d[9];
            d[0] = '\0';
            int s = strlen(data);
            while(s++ < 8)
            strcat(d, "0");

            strcat(d, data);
            str = d;
        }else str = data;

        strcpy(code_memory[int_addr], str.substr(0, 2).c_str());
        strcpy(code_memory[int_addr + 1], str.substr(2, 2).c_str());
        strcpy(code_memory[int_addr + 2], str.substr(4, 2).c_str());
        strcpy(code_memory[int_addr + 3], str.substr(6, 2).c_str());
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

    void WriteWordToMemory(uint32_t addr, char data[9])
    {
        if(addr > MEM_size - 4 || addr < 0)
        {
            Error("the provided address is out of range of code memory");
            return;
        }
        std::string str;
        if(strlen(data) < 8)
        {
            char d[9];
            d[0] = '\0';
            int s = strlen(data);
            while(s++ < 8)
            strcat(d, "0");

            strcat(d, data);
            str = d;
        }else str = data;

        strcpy(memory[addr], str.substr(0, 2).c_str());
        strcpy(memory[addr + 1], str.substr(2, 2).c_str());
        strcpy(memory[addr + 2], str.substr(4, 2).c_str());
        strcpy(memory[addr + 3], str.substr(6, 2).c_str());
    }
    
    void WriteWordToMemory(char addr[9], char data[9])
    {
        if(strlen(addr) > 8)
        {
            Error("the address should be 8 or less then 8 characters long");
            return;
        }
        uint32_t int_addr = hexaDecimalToDecimal(addr);
        if(int_addr > MEM_size - 4 || int_addr < 0)
        {
            Error("the provided address is out of range of code memory");
            return;
        }
        std::string str;
        if(strlen(data) < 8)
        {
            char d[9];
            d[0] = '\0';
            int s = strlen(data);
            while(s++ < 8)
            strcat(d, "0");

            strcat(d, data);
            str = d;
        }else str = data;

        strcpy(memory[int_addr], str.substr(0, 2).c_str());
        strcpy(memory[int_addr + 1], str.substr(2, 2).c_str());
        strcpy(memory[int_addr + 2], str.substr(4, 2).c_str());
        strcpy(memory[int_addr + 3], str.substr(6, 2).c_str());
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
    
    void WriteRegister(int reg, int32_t data)
    {
        if(reg > 31 || reg < 0)
        {
            Error("the provided register does not exist in RV32M");
            return;
        }
        if(reg != 0)
        {
            Register[reg].first = data;
            char binary[33], hex[9];
            decimalToBinary(data, binary, 32);
            BinarytoHexadecimal(binary, hex);
            strcpy(Register[reg].second, hex);
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