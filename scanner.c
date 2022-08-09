#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "source.c"

typedef enum TokenType TokenType;
typedef struct Token Token;
// typedef struct TokenList TokenList;
#define MIN_LIST_VALUE 500

enum TokenType {

  // Single-character tokens.
  LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
  COMMA, DOT, SEMICOLON,

  // One or two character tokens.
  NOT, NOT_EQUAL,
  EQUAL, EQUAL_EQUAL,
  GREATER, GREATER_EQUAL,
  LESS, LESS_EQUAL,
  PLUS, PLUS_PLUS,
  MINUS, MINUS_MINUS,
  STAR, STAR_EQUAL,
  SLASH, SLASH_EQUAL,

  // Literals.
  IDENTIFIER, STRING, NUMBER,

  // Keywords.
  AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NONE, OR,
  PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE, IMPORT,

  EOFI
};

struct Token{
    enum TokenType type;
    int s_offset;
    int e_offset;
};

typedef struct TokensList{
    int size;
    Token* arr;

}TokenList;


Token new_token(TokenType type, int start, int end)
{
    Token t;
    t.type = type;
    t.s_offset = start;
    t.e_offset = end;
    return t;
}

char next(Source* source)
{
    return (source->current >= source->size) ? '\0' : source->data[source->current++];
}

bool next_if(Source* source, char expected)
{

    if (source->current >= source->size) return false;
    
    else if ( source->data[source->current] == expected ) {
        source->current++;
        return true;
    }
    
    else return false;
}

Token scan_next_token(Source* source)
{
    int start = source->current;
    char* value;
    char ch = next(source);
    int end = source->current;
    switch (ch) {

    case '\0': return new_token(EOFI, start, end); break;

    case '(': return new_token(LEFT_PAREN, start, end); break;
    case ')': return new_token(RIGHT_PAREN, start, end); break;
    case '{': return new_token(RIGHT_BRACE, start, end); break;
    case '}': return new_token(LEFT_BRACE, start, end); break;
    case ',': return new_token(COMMA, start, end); break;
    case '.': return new_token(DOT, start, end); break;
    case ';': return new_token(SEMICOLON, start, end); break;

    case '!': return next_if(source, '=') ? new_token(NOT_EQUAL, start, source->current) : new_token(NOT, start, end);
    case '=': return next_if(source, '=') ? new_token(EQUAL_EQUAL, start, source->current) : new_token(EQUAL, start, end);

    default: 
        return new_token(EOFI, start, end);
    }
    
}

bool append_token(TokenList* tokens, Token new_token)
{
    return true;
}

TokenList* scan_tokens(Source* source){
    int current = 0;
    TokenList* tokens = calloc(1, sizeof(TokenList));
    tokens->size = MIN_LIST_VALUE;
    tokens->arr = (Token*) calloc(tokens->size, sizeof(Token));

    while (current < source->size) {
        // Getting next Token
        Token new_token = scan_next_token(source);
        append_token(tokens, new_token);

    }

    // Once offset calculation is implemented uncomment the line below
    // create_token(EOFI, "", offset)

    return tokens;
}