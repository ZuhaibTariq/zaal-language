#include "Includes/token.h"
#include "Includes/utils.h"

char* resrv_kwrd[] = {
    [AND] = "and",
    [CLASS] = "class",
    [ELSE] = "else", 
    [FALSE] = "false",
    [FUNC] =  "fn",
    [FOR] =  "for",
    [IF] = "if",
    [NONE] = "None",
    [OR] = "or",
    [PRINT] = "print",
    [RETURN] = "return",
    [SUPER] = "super",
    [THIS] = "this",
    [TRUE] = "true",
    [VAR] = "var",
    [WHILE] = "while",
    [IMPORT] = "import"
};

Token new_token(TokenType type, const char* data, int start, int end)
{   
    //Remove Quotes from string before saving its value
    if (type == STRING){
        start++;
        end--;
    }

    Token t;
    t.type = type;
    t.value = slice(data, start, end); // Responsiblity of the caller to free this memory
    return t;
}

TokenType match_kwrd(char* keyword)
{
    int length = sizeof(resrv_kwrd) / sizeof(resrv_kwrd[0]);
    for(int i=0;i < length; i++)
    { 
        if (!strcmp(resrv_kwrd[i], keyword)) return (TokenType) i;
    }
    return NO_TOK;
}