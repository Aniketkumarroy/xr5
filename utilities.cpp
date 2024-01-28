#include<string.h>
#include<ctype.h>
#include<stdlib.h>
#include"hex.h"

char *COMMENT = ";";

void RemoveSpaces(char **line){
    // removing leading spaces
    while(isspace(**line))
    (*line)++;
    // removing trailing spaces
    char *end = *line + strlen(*line) - 1;
    if(isspace(*end)){
        while(isspace(*end))
        end--;
        *(end+1) = '\0';
    }
}
void RemoveComments(char *line, char *comment){
    char *found = strstr(line, comment); // found points to the address of the comment symbol(s)
    if(found != NULL)
    *found = '\0';
}
void Preprocess(char **line){
    RemoveSpaces(line);             // Removing leading and trailing spaces
    RemoveComments(*line, COMMENT); // Removing comments
    char *c = *line;
    int i = 0;
    while(*c != '\0'){
        if(*c == ',')
        *c = ' '; // removing ',' with ' '

        // ensuring that there should be only one space separating two mnemonics
        (*line)[i] = tolower(*c);
        i++;
        if(*c == ' '){
            while(*(c+1) == ' ')
            c++;
        }
        c++;
    }
    (*line)[i] = '\0';
}
void RegisterCode(char* r, char *binary, void (*error)(char*) = nullptr){
    if(!isdecimal(r+1) && error != nullptr) error("Invalid Register\n");
    if((atoi(r+1) > 31 || atoi(r+1) < 0) && error != nullptr){
        error("Register does not exist in RV32M\n");
    }
    ValueInterpreter(r+1, binary, 5);
}