#include <string.h>

#ifndef __TOKEN_H_INCLUDED__
#define __TOKEN_H_INCLUDED__

typedef enum{

  // Reserved Keywords.
  //Must be Kept at the top in enums, this results in a smaller resrv_kwrd array size
  AND, CLASS, ELSE, FALSE, FUNC, FOR, IF, NONE, OR, 
  PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE, IMPORT,

  // Single-character tokens.
  LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
  COMMA, DOT, SEMICOLON, MODULO,

  // One or two character tokens.
  NOT, NOT_EQUAL,
  EQUAL, EQUAL_EQUAL,
  GREATER, GREATER_EQUAL,
  LESS, LESS_EQUAL,
  PLUS, PLUS_PLUS, PLUS_EQUAL,
  MINUS, MINUS_MINUS, MINUS_EQUAL,
  STAR, STAR_EQUAL,
  SLASH, SLASH_EQUAL,

  // Literals.
  IDENTIFIER, STRING, NUMBER,

  EOFI, NO_TOK, BAD_TOK
}TokenType;


typedef struct{
    TokenType type;
    char*  value;
}Token;

typedef struct{
    int size;
    Token* arr;
    int current;

}TokensList;

Token new_token(TokenType type, const char* data, int start, int end);

TokenType match_kwrd(char* keyword);

#endif