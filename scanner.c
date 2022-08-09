#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "source.c"

#define MIN_LIST_VALUE 500

typedef enum{

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

  // Keywords.
  AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NONE, OR,
  PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE, IMPORT,

  EOFI, EMPTY_TOK, BAD_TOK
}TokenType;

typedef struct{
    TokenType type;
    int s_offset;
    int e_offset;
}Token;

typedef struct{
    int size;
    Token* arr;
    int current;

}TokensList;


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

Token slash_tokens(Source* source, int start)
{
    if (next_if(source, '=')) return new_token(SLASH_EQUAL, start, source->current);

    else if (next_if(source, '/')){
        
        //since it's a comment, eat charaters till '\n' or '\0'
        for(char ch = next(source); ch != '\n' && ch != '\0'; ch=next(source));
        return new_token(EMPTY_TOK, start, source->current);
    }
    else return new_token(SLASH, start, source->current);
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
    case '{': return new_token(LEFT_BRACE, start, end); break;
    case '}': return new_token(RIGHT_BRACE, start, end); break;
    case ',': return new_token(COMMA, start, end); break;
    case '.': return new_token(DOT, start, end); break;
    case ';': return new_token(SEMICOLON, start, end); break;
    case '%': return new_token(MODULO, start, end); break;

    case '!': return next_if(source, '=') ? new_token(NOT_EQUAL, start, source->current) : new_token(NOT, start, end);
    case '=': return next_if(source, '=') ? new_token(EQUAL_EQUAL, start, source->current) : new_token(EQUAL, start, end);
    case '>': return next_if(source, '=') ? new_token(GREATER_EQUAL, start, source->current) : new_token(GREATER, start, end);
    case '<': return next_if(source, '=') ? new_token(LESS_EQUAL, start, source->current) : new_token(LESS, start, end);
    case '*': return next_if(source, '=') ? new_token(STAR_EQUAL, start, source->current) : new_token(STAR, start, end);
    
    case '+': return next_if(source, '+') ? new_token(PLUS_PLUS, start, source->current) : 
                     next_if(source, '=') ? new_token(PLUS_EQUAL, start, source->current) : new_token(PLUS, start, end);
    
    case '-': return next_if(source, '-') ? new_token(MINUS_MINUS, start, source->current) : 
                     next_if(source, '=') ? new_token(MINUS_EQUAL, start, source->current) : new_token(MINUS, start, end);
    
    case '/': return slash_tokens(source, start); break;
    
    
    

    default: 
        return new_token(EOFI, start, end);
    }
    
}

bool append_token(TokensList* tokens, Token new_token)
{
    if (tokens->current == tokens->size)
    {
        //Code to reallocate list once it runs out
        //for now lets assume it never does
    }
    tokens->arr[tokens->current] = new_token;
    tokens->current++;

    return true;
}

TokensList* scan_tokens(Source* source){
    TokensList* tokens = malloc(sizeof(TokensList));
    tokens->size = MIN_LIST_VALUE;
    tokens->arr = (Token*) calloc(tokens->size, sizeof(Token));
    tokens->current =0;
    if (tokens->arr == NULL) printf("Mem Not Allocated");

    while (source->current < source->size) {
        // Getting next Token
        Token new_token = scan_next_token(source);
        append_token(tokens, new_token);

    }
    append_token(tokens, new_token(EOFI, source->current, source->current));

    return tokens;
}