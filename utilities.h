#ifndef	UTIL
#define UTIL
#include<string.h>

void RemoveSpaces(char **line);
void RemoveComments(char *line, char *comment);
void Preprocess(char **line);
void RegisterCode(char* r, char *binary, void (*error)(char*) = nullptr);
#endif
