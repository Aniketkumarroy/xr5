#include<utility>
#include<cstdint>
#include<math.h>
#include<stdlib.h>
#include<string>
#include<string.h>
#include"hex.h"
#include"utilities.h"

#include"RV32M.h"

void RV32M::LoadUpperImmediate()
{
    std::string value = RV32M::BinaryCode.substr(0, 20);
    value += "000000000000";

    char *ptr = (char*)value.c_str();

    char hex[9];
    BinarytoHexadecimal(ptr, hex);

    value = RV32M::BinaryCode.substr(20, 5);
    int reg = BinaryToDecimal(ptr);

    RV32M::WriteRegister(reg, hex);
}

void RV32M::AddUpperImmediateToPC()
{
    std::string value = RV32M::BinaryCode.substr(0, 20);
    value += "000000000000";

    char *ptr = (char*)value.c_str();

    int32_t data = PC.first + BinaryToSignedDecimal(ptr);
    
    value = RV32M::BinaryCode.substr(20, 5);
    uint32_t reg = BinaryToDecimal(ptr);

    RV32M::WriteRegister(reg, data);
}

void RV32M::ImmediateArithmetic()
{
    std::string funct3 = RV32M::BinaryCode.substr(17, 3);

    std::string r = RV32M::BinaryCode.substr(12, 5);
    char *ptr = (char*)r.c_str();
    uint32_t rs1 = BinaryToDecimal(ptr);

    r = RV32M::BinaryCode.substr(20, 5);
    uint32_t rd = BinaryToDecimal(ptr);
    if(funct3 == "000") // addi
    {
        std::string &Immediatevalue = funct3; // since funct3 is no longer in use, we will use its resources for oher purposes
        Immediatevalue = RV32M::BinaryCode.substr(0, 12);
        RV32M::SignExtend(Immediatevalue, 32, true);
        ptr = (char*)Immediatevalue.c_str();
        int32_t data = RV32M::Register[rs1].first + BinaryToSignedDecimal(ptr);
        RV32M::WriteRegister(rd, data);
    }
    else if(funct3 == "010") //slti
    {
        std::string &Immediatevalue = funct3; // since funct3 is no longer in use, we will use its resources for oher purposes
        Immediatevalue = RV32M::BinaryCode.substr(0, 12);
        RV32M::SignExtend(Immediatevalue, 32, true);
        ptr = (char*)Immediatevalue.c_str();
        int32_t data = RV32M::Register[rs1].first < BinaryToSignedDecimal(ptr);
        RV32M::WriteRegister(rd, data);
    }
    else if(funct3 == "011") //sltiu
    {
        std::string &Immediatevalue = funct3; // since funct3 is no longer in use, we will use its resources for oher purposes
        Immediatevalue = RV32M::BinaryCode.substr(0, 12);
        RV32M::SignExtend(Immediatevalue, 32, true);
        ptr = (char*)Immediatevalue.c_str();
        uint32_t unsigned_rs1 = hexaDecimalToDecimal(Register[rs1].second);
        int32_t data = unsigned_rs1 < BinaryToDecimal(ptr);
        RV32M::WriteRegister(rd, data);
    }
    else if(funct3 == "100") // xori
    {
        std::string &Immediatevalue = funct3; // since funct3 is no longer in use, we will use its resources for oher purposes
        Immediatevalue = RV32M::BinaryCode.substr(0, 12);
        RV32M::SignExtend(Immediatevalue, 32, true);
        ptr = (char*)Immediatevalue.c_str();
        int32_t data = RV32M::Register[rs1].first ^ BinaryToSignedDecimal(ptr);
        RV32M::WriteRegister(rd, data);
    }
    else if(funct3 == "110") // ori
    {
        std::string &Immediatevalue = funct3; // since funct3 is no longer in use, we will use its resources for oher purposes
        Immediatevalue = RV32M::BinaryCode.substr(0, 12);
        RV32M::SignExtend(Immediatevalue, 32, true);
        ptr = (char*)Immediatevalue.c_str();
        int32_t data = RV32M::Register[rs1].first | BinaryToSignedDecimal(ptr);
        RV32M::WriteRegister(rd, data);
    }
    else if(funct3 == "111") // andi
    {
        std::string &Immediatevalue = funct3; // since funct3 is no longer in use, we will use its resources for oher purposes
        Immediatevalue = RV32M::BinaryCode.substr(0, 12);
        RV32M::SignExtend(Immediatevalue, 32, true);
        ptr = (char*)Immediatevalue.c_str();
        int32_t data = RV32M::Register[rs1].first & BinaryToSignedDecimal(ptr);
        RV32M::WriteRegister(rd, data);
    }
    else if(funct3 == "001") // slli
    {
        std::string &Immediatevalue = funct3; // since funct3 is no longer in use, we will use its resources for oher purposes
        Immediatevalue = RV32M::BinaryCode.substr(7, 5);
        ptr = (char*)Immediatevalue.c_str();
        int32_t data = RV32M::Register[rs1].first << BinaryToDecimal(ptr);
        RV32M::WriteRegister(rd, data);
    }
    else if(funct3 == "101") // right shift
    {
        if(RV32M::BinaryCode[1] == '1') // srai
        {
            std::string &Immediatevalue = funct3; // since funct3 is no longer in use, we will use its resources for oher purposes
            Immediatevalue = RV32M::BinaryCode.substr(7, 5);
            ptr = (char*)Immediatevalue.c_str();
            uint32_t shift = BinaryToDecimal(ptr);
            int32_t data = (RV32M::Register[rs1].first >> shift) | (~((~0) >> shift)); // from chatgpt
            // int32_t data = Register[rs1].first >> BinaryToDecimal(ptr);
            RV32M::WriteRegister(rd, data);
        }
        else // srli
        {
            std::string &Immediatevalue = funct3; // since funct3 is no longer in use, we will use its resources for oher purposes
            Immediatevalue = RV32M::BinaryCode.substr(7, 5);
            ptr = (char*)Immediatevalue.c_str();
            uint32_t data = static_cast<unsigned int>(RV32M::Register[rs1].first) >> BinaryToDecimal(ptr); // from chatgpt
            // uint32_t data = Register[rs1].first >> BinaryToDecimal(ptr);
            RV32M::WriteRegister(rd, data);
        }
    }
}

void RV32M::RegisterArithmetic()
{
    std::string funct3 = RV32M::BinaryCode.substr(17, 3);

    std::string r = RV32M::BinaryCode.substr(7, 5);
    char *ptr = (char*)r.c_str();
    uint32_t rs2 = BinaryToDecimal(ptr);

    r = RV32M::BinaryCode.substr(12, 5);
    uint32_t rs1 = BinaryToDecimal(ptr);
    
    r = RV32M::BinaryCode.substr(20, 5);
    uint32_t rd = BinaryToDecimal(ptr);
    if(funct3 == "000") // add, sub, mul
    {
        if(RV32M::BinaryCode[6] == '0')
        {
            if(RV32M::BinaryCode[1] == '1') // add
            {
                int32_t data = RV32M::Register[rs1].first - RV32M::Register[rs2].first;
                RV32M::WriteRegister(rd, data);
            }
            else // sub
            {
                int32_t data = RV32M::Register[rs1].first + RV32M::Register[rs2].first;
                RV32M::WriteRegister(rd, data);
            }
        }
        else // mul
        {
            int32_t product = static_cast<int32_t>(static_cast<int64_t>(RV32M::Register[rs1].first) * static_cast<int64_t>(RV32M::Register[rs2].first));
            RV32M::WriteRegister(rd, product);
        }
    }
    else if(funct3 == "010") // slt, mulhsu
    {
        if(RV32M::BinaryCode[6] == '0')
        {
            int32_t data = RV32M::Register[rs1].first < RV32M::Register[rs2].first;
            RV32M::WriteRegister(rd, data);
        }
        else // mulhsu
        {
            uint32_t unsigned_rs2 = hexaDecimalToDecimal(Register[rs2].second);
            int64_t product = static_cast<int64_t>(RV32M::Register[rs1].first) * static_cast<uint64_t>(unsigned_rs2);
            int32_t result = static_cast<int32_t>(product >> 32);
            RV32M::WriteRegister(rd, result);
        }
    }
    else if(funct3 == "011") // sltu, mulhu
    {
        if(RV32M::BinaryCode[6] == '0')
        {
            uint32_t unsigned_rs1 = hexaDecimalToDecimal(RV32M::Register[rs1].second);
            uint32_t unsigned_rs2 = hexaDecimalToDecimal(RV32M::Register[rs2].second);
            int32_t data = unsigned_rs1 < unsigned_rs2;
            RV32M::WriteRegister(rd, data);
        }
        else // mulhu
        {
            uint32_t unsigned_rs1 = hexaDecimalToDecimal(RV32M::Register[rs1].second);
            uint32_t unsigned_rs2 = hexaDecimalToDecimal(RV32M::Register[rs2].second);
            int64_t product = static_cast<uint64_t>(unsigned_rs1) * static_cast<uint64_t>(unsigned_rs2);
            int32_t result = static_cast<int32_t>(product >> 32);
            RV32M::WriteRegister(rd, result);
        }
    }
    else if(funct3 == "100") // xor, div
    {
        if(RV32M::BinaryCode[6] == '0')
        {
            int32_t data = RV32M::Register[rs1].first ^ RV32M::Register[rs2].first;
            RV32M::WriteRegister(rd, data);
        }
        else // div
        {
            int32_t data = RV32M::Register[rs1].first / RV32M::Register[rs2].first;
            RV32M::WriteRegister(rd, data);
        }
    }
    else if(funct3 == "110") // or, rem
    {
        if(RV32M::BinaryCode[6] == '0')
        {
            int32_t data = RV32M::Register[rs1].first | RV32M::Register[rs2].first;
            RV32M::WriteRegister(rd, data);
        }
        else // rem
        {
            int32_t data = RV32M::Register[rs1].first % RV32M::Register[rs2].first;
            RV32M::WriteRegister(rd, data);
        }
    }
    else if(funct3 == "111") // and, remu
    {
        if(RV32M::BinaryCode[6] == '0')
        {
            int32_t data = RV32M::Register[rs1].first & RV32M::Register[rs2].first;
            RV32M::WriteRegister(rd, data);
        }
        else // remu
        {
            uint32_t unsigned_rs1 = hexaDecimalToDecimal(RV32M::Register[rs1].second);
            uint32_t unsigned_rs2 = hexaDecimalToDecimal(RV32M::Register[rs2].second);
            int32_t data = unsigned_rs1 % unsigned_rs2;
            RV32M::WriteRegister(rd, data);
        }
    }
    else if(funct3 == "001") // sll, mulh
    {
        if(RV32M::BinaryCode[6] == '0')
        {
            int shift = RV32M::Register[rs2].first & 31; // getting the lower 5 bits
            int32_t data = RV32M::Register[rs1].first << shift;
            RV32M::WriteRegister(rd, data);
        }
        else // mulh
        {
            int64_t product = static_cast<int64_t>(RV32M::Register[rs1].first) * static_cast<int64_t>(RV32M::Register[rs2].first);
            int32_t result = static_cast<int32_t>(product >> 32);
            RV32M::WriteRegister(rd, result);
        }
    }
    else if(funct3 == "101") // right shift, divu
    {
        if(RV32M::BinaryCode[6] == '0')
        {
            if(RV32M::BinaryCode[1] == '1') // sra
            {
                int shift = RV32M::Register[rs2].first & 31; // getting the lower 5 bits
                int32_t data = (RV32M::Register[rs1].first >> shift) | (~((~0) >> shift)); // from chatgpt
                // int32_t data = Register[rs1].first >> BinaryToDecimal(ptr);
                RV32M::WriteRegister(rd, data);
            }
            else // srl
            {
                int shift = RV32M::Register[rs2].first & 31; // getting the lower 5 bits
                uint32_t data = static_cast<unsigned int>(RV32M::Register[rs1].first) >> shift; // from chatgpt
                // uint32_t data = Register[rs1].first >> BinaryToDecimal(ptr);
                RV32M::WriteRegister(rd, data);
            }
        }
        else // divu
        {
            uint32_t unsigned_rs1 = hexaDecimalToDecimal(RV32M::Register[rs1].second);
            uint32_t unsigned_rs2 = hexaDecimalToDecimal(RV32M::Register[rs2].second);
            int32_t data = unsigned_rs1 / unsigned_rs2;
            RV32M::WriteRegister(rd, data);
        }
    }
}

void RV32M::LoadFromMemory()
{
    std::string funct3 = RV32M::BinaryCode.substr(17, 3);

    std::string r = RV32M::BinaryCode.substr(12, 5);
    char *ptr = (char*)r.c_str();
    uint32_t rs1 = BinaryToDecimal(ptr);

    r = BinaryCode.substr(20, 5);
    uint32_t rd = BinaryToDecimal(ptr);

    std::string offset = RV32M::BinaryCode.substr(0, 12);
    RV32M::SignExtend(offset, 32, true);
    ptr = (char*)offset.c_str();
    uint32_t addr = hexaDecimalToDecimal(RV32M::Register[rs1].second) + BinaryToSignedDecimal(ptr);
    if(funct3 == "000")
    {
        std::string value = RV32M::ReadByteFromMemory(addr);
        
    }
    else if(funct3 == "001")
    {
        
    }
    else if(funct3 == "010")
    {

    }
    else if(funct3 == "100")
    {

    }
    else if(funct3 == "101")
    {

    }     
}

void RV32M::StoreToMemory()
{
    
}

void RV32M::JumpAndLink()
{
    
}

void RV32M::JumpRelativeAndLink()
{
    
}

void RV32M::ConditionalBranch()
{
    
}

void RV32M::Error(char *msg)
{
    if(RV32M::ErrorHandler != nullptr)
    RV32M::ErrorHandler(msg);
}

void RV32M::Reset(uint32_t memory_size, uint32_t code_memory_size)
{
    if(memory_size != MEM_size) RV32M::MEM_size = memory_size;
    if(code_memory_size != CODE_size) RV32M::CODE_size = code_memory_size;

    RV32M::memory = new char*[RV32M::MEM_size]; // (char**)malloc(MEM_size*sizeof(char*));
    if(RV32M::memory == nullptr) exit(0);
    RV32M::code_memory = new char*[RV32M::CODE_size]; // (char**)malloc(CODE_size*sizeof(char*));
    if(RV32M::code_memory == nullptr) exit(0);

    for (uint32_t i = 0; i < RV32M::MEM_size; i++)
    {
        RV32M::memory[i] = new char [3]; // (char*)malloc(3*sizeof(char));
        if(RV32M::memory[i] == nullptr) exit(0);
        strcpy(RV32M::memory[i], "ff");
    }
    for (uint32_t i = 0; i < RV32M::CODE_size; i++)
    {
        RV32M::code_memory[i] = new char [3]; // (char*)malloc(3*sizeof(char));
        if(RV32M::memory[i] == nullptr) exit(0);
        strcpy(RV32M::code_memory[i], "ff");
    }

    RV32M::PC.first = 0;
    strcpy(RV32M::PC.second, "00000000");

    for(uint16_t i = 0; i< 32 ; i++)
    {
        RV32M::Register[i].first = 0;
        strcpy(RV32M::Register[i].second, "00000000");
    }
}

RV32M::RV32M(uint32_t memory_size, uint32_t code_memory_size, void (*error)(char*))
{
    RV32M::ErrorHandler = error;
    Reset(memory_size, code_memory_size);
    
}

RV32M::~RV32M()
{
    for (uint32_t i = 0; i < RV32M::MEM_size; i++)
    delete(RV32M::memory[i]);
    for (uint32_t i = 0; i < RV32M::CODE_size; i++)
    delete(RV32M::code_memory[i]);

    delete(RV32M::memory);
    delete(RV32M::code_memory);
}

void RV32M::SignExtend(std::string &value, int size, bool sign)
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

void RV32M::WriteByteToCodeMemory(uint32_t addr, char data[3])
{
    if(addr > RV32M::CODE_size - 1 || addr < 0)
    {
        RV32M::Error("the provided address is out of range of code memory");
        return;
    }
    if(strlen(data) == 2)
    strcpy(RV32M::code_memory[addr], data);
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
        strcpy(RV32M::code_memory[addr], d);
    }
}

void RV32M::WriteByteToCodeMemory(char addr[9], char data[3])
{
    if(strlen(addr) > 8)
    {
        RV32M::Error("the address should be 8 or less then 8 characters long");
        return;
    }
    uint32_t int_addr = hexaDecimalToDecimal(addr);
    if(int_addr > RV32M::CODE_size - 1 || int_addr < 0)
    {
        RV32M::Error("the provided address is out of range of code memory");
        return;
    }
    if(strlen(data) == 2)
    strcpy(RV32M::code_memory[int_addr], data);
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
        strcpy(RV32M::code_memory[int_addr], d);
    }
}

void RV32M::WriteWordToCodeMemory(uint32_t addr, char data[9])
{
    if(addr > RV32M::CODE_size - 4 || addr < 0)
    {
        RV32M::Error("the provided address is out of range of code memory");
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

    strcpy(RV32M::code_memory[addr], str.substr(0, 2).c_str());
    strcpy(RV32M::code_memory[addr + 1], str.substr(2, 2).c_str());
    strcpy(RV32M::code_memory[addr + 2], str.substr(4, 2).c_str());
    strcpy(RV32M::code_memory[addr + 3], str.substr(6, 2).c_str());
}

void RV32M::WriteWordToCodeMemory(char addr[9], char data[9])
{
    if(strlen(addr) > 8)
    {
        RV32M::Error("the address should be 8 or less then 8 characters long");
        return;
    }
    uint32_t int_addr = hexaDecimalToDecimal(addr);
    if(int_addr > RV32M::CODE_size - 4 || int_addr < 0)
    {
        RV32M::Error("the provided address is out of range of code memory");
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

    strcpy(RV32M::code_memory[int_addr], str.substr(0, 2).c_str());
    strcpy(RV32M::code_memory[int_addr + 1], str.substr(2, 2).c_str());
    strcpy(RV32M::code_memory[int_addr + 2], str.substr(4, 2).c_str());
    strcpy(RV32M::code_memory[int_addr + 3], str.substr(6, 2).c_str());
}

void RV32M::WriteByteToMemory(uint32_t addr, char data[3])
{
    if(addr > RV32M::MEM_size - 1 || addr < 0)
    {
        RV32M::Error("the provided address is out of range of code memory");
        return;
    }
    if(strlen(data) == 2)
    strcpy(RV32M::memory[addr], data);
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
        strcpy(RV32M::memory[addr], d);
    }
}

void RV32M::WriteByteToMemory(char addr[9], char data[3])
{
    if(strlen(addr) > 8)
    {
        RV32M::Error("the address should be 8 or less then 8 characters long");
        return;
    }
    uint32_t int_addr = hexaDecimalToDecimal(addr);
    if(int_addr > RV32M::MEM_size - 1 || int_addr < 0)
    {
        RV32M::Error("the provided address is out of range of code memory");
        return;
    }
    if(strlen(data) == 2)
    strcpy(RV32M::memory[int_addr], data);
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
        strcpy(RV32M::memory[int_addr], d);
    }
}

void RV32M::WriteWordToMemory(uint32_t addr, char data[9])
{
    if(addr > RV32M::MEM_size - 4 || addr < 0)
    {
        RV32M::Error("the provided address is out of range of code memory");
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

    strcpy(RV32M::memory[addr], str.substr(0, 2).c_str());
    strcpy(RV32M::memory[addr + 1], str.substr(2, 2).c_str());
    strcpy(RV32M::memory[addr + 2], str.substr(4, 2).c_str());
    strcpy(RV32M::memory[addr + 3], str.substr(6, 2).c_str());
}

void RV32M::WriteWordToMemory(char addr[9], char data[9])
{
    if(strlen(addr) > 8)
    {
        RV32M::Error("the address should be 8 or less then 8 characters long");
        return;
    }
    uint32_t int_addr = hexaDecimalToDecimal(addr);
    if(int_addr > RV32M::MEM_size - 4 || int_addr < 0)
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

    strcpy(RV32M::memory[int_addr], str.substr(0, 2).c_str());
    strcpy(RV32M::memory[int_addr + 1], str.substr(2, 2).c_str());
    strcpy(RV32M::memory[int_addr + 2], str.substr(4, 2).c_str());
    strcpy(RV32M::memory[int_addr + 3], str.substr(6, 2).c_str());
}

std::string RV32M::ReadByteFromCodeMemory(uint32_t addr)
{
    if(addr > RV32M::CODE_size - 4 || addr < 0)
    {
        RV32M::Error("the provided address is out of range of code memory");
        return std::string("");
    }
    std::string byte;
    byte  = RV32M::code_memory[addr];

    return byte;
}

std::string RV32M::ReadByteFromCodeMemory(char addr[9])
{
    if(strlen(addr) > 8)
    {
        RV32M::Error("the address should be 8 or less then 8 characters long");
        return std::string("");
    }
    uint32_t int_addr = hexaDecimalToDecimal(addr);
    if(int_addr > RV32M::CODE_size - 4 || int_addr < 0)
    {
        Error("the provided address is out of range of code memory");
        return std::string("");
    }
    std::string byte;
    byte  = RV32M::code_memory[int_addr];

    return byte;
}

std::string RV32M::ReadWordFromCodeMemory(uint32_t addr)
{
    if(addr > RV32M::CODE_size - 4 || addr < 0)
    {
        RV32M::Error("the provided address is out of range of code memory");
        return std::string("");
    }
    std::string word;
    word  = RV32M::code_memory[addr];
    word += RV32M::code_memory[addr + 1];
    word += RV32M::code_memory[addr + 2];
    word += RV32M::code_memory[addr + 3];

    return word;
}

std::string RV32M::ReadWordFromCodeMemory(char addr[9])
{
    if(strlen(addr) > 8)
    {
        RV32M::Error("the address should be 8 or less then 8 characters long");
        return std::string("");
    }
    uint32_t int_addr = hexaDecimalToDecimal(addr);
    if(int_addr > RV32M::CODE_size - 4 || int_addr < 0)
    {
        Error("the provided address is out of range of code memory");
        return std::string("");
    }
    std::string word;
    word  = RV32M::code_memory[int_addr];
    word += RV32M::code_memory[int_addr + 1];
    word += RV32M::code_memory[int_addr + 2];
    word += RV32M::code_memory[int_addr + 3];

    return word;
}

std::string RV32M::ReadByteFromMemory(uint32_t addr)
{
    if(addr > RV32M::MEM_size - 4 || addr < 0)
    {
        RV32M::Error("the provided address is out of range of code memory");
        return std::string("");
    }
    std::string byte;
    byte  = RV32M::memory[addr];

    return byte;
}

std::string RV32M::ReadByteFromMemory(char addr[9])
{
    if(strlen(addr) > 8)
    {
        RV32M::Error("the address should be 8 or less then 8 characters long");
        return std::string("");
    }
    uint32_t int_addr = hexaDecimalToDecimal(addr);
    if(int_addr > RV32M::MEM_size - 4 || int_addr < 0)
    {
        Error("the provided address is out of range of code memory");
        return std::string("");
    }
    std::string byte;
    byte  = RV32M::memory[int_addr];

    return byte;
}

std::string RV32M::ReadWordFromMemory(uint32_t addr)
{
    if(addr > RV32M::MEM_size - 4 || addr < 0)
    {
        Error("the provided address is out of range of code memory");
        return std::string("");
    }
    std::string word;
    word  = RV32M::memory[addr];
    word += RV32M::memory[addr + 1];
    word += RV32M::memory[addr + 2];
    word += RV32M::memory[addr + 3];

    return word;
}

std::string RV32M::ReadWordFromMemory(char addr[9])
{
    if(strlen(addr) > 8)
    {
        RV32M::Error("the address should be 8 or less then 8 characters long");
        return std::string("");
    }
    uint32_t int_addr = hexaDecimalToDecimal(addr);
    if(int_addr > RV32M::MEM_size - 4 || int_addr < 0)
    {
        Error("the provided address is out of range of code memory");
        return std::string("");
    }
    std::string word;
    word  = RV32M::memory[int_addr];
    word += RV32M::memory[int_addr + 1];
    word += RV32M::memory[int_addr + 2];
    word += RV32M::memory[int_addr + 3];

    return word;
}

void RV32M::WriteRegister(int reg, char data[9])
{
    if(reg > 31 || reg < 0)
    {
        RV32M::Error("the provided register does not exist in RV32M");
        return;
    }
    if(reg != 0)
    {
        if(strlen(data) == 8)
        {
            strcpy(RV32M::Register[reg].second, data);
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
            strcpy(RV32M::Register[reg].second, d);
            RV32M::Register[reg].first = hexaDecimalToSignedDecimal(d, 32);
        }
    }
}

void RV32M::WriteRegister(int reg, int32_t data)
{
    if(reg > 31 || reg < 0)
    {
        RV32M::Error("the provided register does not exist in RV32M");
        return;
    }
    if(reg != 0)
    {
        RV32M::Register[reg].first = data;
        char binary[33], hex[9];
        decimalToBinary(data, binary, 32);
        BinarytoHexadecimal(binary, hex);
        strcpy(RV32M::Register[reg].second, hex);
    }
}

void RV32M::PC_Increment(uint32_t bytes)
{
    RV32M::PC.first += bytes;
    char binary[33];
    decimalToBinary(RV32M::PC.first, binary, 32);
    BinarytoHexadecimal(binary, RV32M::PC.second);
}


void RV32M::InstructionExecute()
{
    if(RV32M::Executor != nullptr)
    (this->*Executor)();
}
void RV32M::InstructionDecode()
{
    char binary[33];
    char *hex = (char*)RV32M::instruction.c_str();
    HexadecimalToBinary(hex, binary, 32);
    RV32M::BinaryCode = binary;
    const char *opcode = RV32M::BinaryCode.c_str() + 25;

    if(strcmp(opcode,"0110111") == 0) RV32M::Executor = &RV32M::LoadUpperImmediate;
    else if(strcmp(opcode,"0010111") == 0) RV32M::Executor = &RV32M::AddUpperImmediateToPC;
    else if(strcmp(opcode,"0010011") == 0) RV32M::Executor = &RV32M::ImmediateArithmetic;
    else if(strcmp(opcode,"0110011") == 0) RV32M::Executor = &RV32M::RegisterArithmetic;
    else if(strcmp(opcode,"0000011") == 0) RV32M::Executor = &RV32M::LoadFromMemory;
    else if(strcmp(opcode,"0100011") == 0) RV32M::Executor = &RV32M::StoreToMemory;
    else if(strcmp(opcode,"1101111") == 0) RV32M::Executor = &RV32M::JumpAndLink;
    else if(strcmp(opcode,"1100111") == 0) RV32M::Executor = &RV32M::JumpRelativeAndLink;
    else if(strcmp(opcode,"1100011") == 0) RV32M::Executor = &RV32M::ConditionalBranch;
    else RV32M::Executor = nullptr;
}

void RV32M::InstructionFetch()
{
    RV32M::instruction  = RV32M::code_memory[RV32M::PC.first];
    RV32M::instruction += RV32M::code_memory[RV32M::PC.first + 1];
    RV32M::instruction += RV32M::code_memory[RV32M::PC.first + 2];
    RV32M::instruction += RV32M::code_memory[RV32M::PC.first + 3];
    RV32M::PC_Increment(4);
}