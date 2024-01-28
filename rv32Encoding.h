#ifndef ENCODING
#define ENCODING
void U_Encoding(char *opcode, char *code, char *binary, void (*error)(char*) = nullptr);
void I_Encoding(char *opcode, char *code, char *binary, void (*error)(char*) = nullptr);
void R_Encoding(char *opcode, char *code, char *binary, void (*error)(char*) = nullptr);
void S_Encoding(char *opcode, char *code, char *binary, void (*error)(char*) = nullptr);
void J_Encoding(char *opcode, char *code, char *binary, void (*error)(char*) = nullptr);
void B_Encoding(char *opcode, char *code, char *binary, void (*error)(char*) = nullptr);
#endif ENCODING