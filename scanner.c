#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Includes/source.h"
#include "Includes/scanner.h"
#include "Includes/token.h"
#include "Includes/utils.h"


#define MIN_LIST_LENGTH 500
#define REALLOC_MULTIPLIER 2


char forward(Source* source)
{
    return (source->current >= source->size) ? '\0' : source->data[source->current++];
}

char current(Source* source)
{
    return (source->current >= source->size) ? '\0' : source->data[source->current];
}

int forward_if(Source* source, char expected)
{
    return source->current >= source->size ? false : source->data[source->current] == expected ? source->current++ : false;
}

TokenType comment(Source* source)
{
    //since it's a comment, eat charaters till '\n' or '\0'
    for(char ch = forward(source); ch != '\0'; ch=forward(source))
        if (ch == '\n') {
            source->line++;
            break;
        }

    return NO_TOK;
}

TokenType number(Source* source)
{
    //Point source->current to last of the adjacent 0-9 characters
    for(char ch = current(source); ch >= '0' && ch <= '9'; ch=current(source)) source->current++;

    return NUMBER;
}

TokenType string(Source* source, int start, char tok)
{
    //Point source->current to the ending tok (" or ')
    for(char ch = forward(source); ch != tok; ch=forward(source)){
        if (ch == '\0' || ch == '\n'){
            //Print an Error
            log_line(source->data, start, source->current);
            printf("[ERROR] Unterminated String at line %lu\n", source->line);
            return NO_TOK;
        }
    }

    return STRING;
}

TokenType word(Source* source, int start)
{
    // get all adjacent a-z A-Z _ 0-9 characters
    for(char ch = current(source); 
    ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z' || ch >= '0' && ch <= '9' || ch == '_'; 
    ch=current(source)) source->current++;
    char* sub_str = slice(source->data, start, source->current); // alloc-4 sub_str
    
    //decide whether its a keyword or a string
    TokenType type = match_kwrd(sub_str);
    free(sub_str); // free-4 sub_str
    return type == NO_TOK ? STRING : type;
}

TokenType scan_next_token(Source* source)
{
    int start = source->current;
    char ch = forward(source);
    switch (ch) {
    //Single Character Tokens
    case '\0': return EOFI;
    case '(': return LEFT_PAREN;
    case ')': return RIGHT_PAREN;
    case '{': return LEFT_BRACE;
    case '}': return RIGHT_BRACE;
    case ',': return COMMA;
    case '.': return DOT;
    case ';': return SEMICOLON;
    case '%': return MODULO;
    case '#': return comment(source);
    //Double Characters Tokens
    case '!': return forward_if(source, '=') ? NOT_EQUAL : NOT;
    case '=': return forward_if(source, '=') ? EQUAL_EQUAL : EQUAL;
    case '>': return forward_if(source, '=') ? GREATER_EQUAL : GREATER;
    case '<': return forward_if(source, '=') ? LESS_EQUAL : LESS;
    case '*': return forward_if(source, '=') ? STAR_EQUAL : STAR;
    case '/': return forward_if(source, '=') ? SLASH_EQUAL : SLASH;
    //Triple Characters Tokens
    case '+': return forward_if(source, '+') ? PLUS_PLUS : forward_if(source, '=') ? PLUS_EQUAL : PLUS;
    case '-': return forward_if(source, '-') ? MINUS_MINUS : forward_if(source, '=') ? MINUS_EQUAL : MINUS;
    //Useless Characters
    case ' ':
    case '\r':
    case '\t':
            return NO_TOK;
    case '\n':
            source->line++;
            return NO_TOK;
    //For 0-9 Character
    case '0' ... '9': return number(source);
    // For Alphabet and _ Character
    case 'a' ... 'z':
    case 'A' ... 'Z':
    case '_':
            return word(source,start);
    // For Quoted Strings
    case '"':
    case '\'':
            return string(source, start, ch);
    default:
        printf("[ERROR] Invalid Character: \"%c\"\n", ch);
        return BAD_TOK;
    }
    
}

bool push_token(TokensList* tokens, Token new_token)
{
    if (tokens->current >= tokens->size)
    {
        //Code to reallocate list once it runs out
        tokens->arr = (Token*) realloc(tokens->arr, sizeof(tokens->arr[0]) * tokens->size * REALLOC_MULTIPLIER);
        tokens->size *= REALLOC_MULTIPLIER;
        printf("[DEBUG] Reallocating Tokens List Memory: %d\n", tokens->size);

    }
    tokens->arr[tokens->current] = new_token;
    tokens->current++;

    return true;
}

TokensList* scan_tokens(Source* source, TokensList* tokens){
    
    tokens->arr = (Token*) calloc(MIN_LIST_LENGTH, sizeof(Token)); // alloc-2 tokens->arr
    if (tokens->arr == NULL) {  printf("[ERROR] Mem Not Allocated for Tokens Array"); exit(2); }
    tokens->size = MIN_LIST_LENGTH;
    tokens->current = 0;
    for (unsigned long tok_start = 0; tok_start < source->size; tok_start = source->current) {
        // Getting next Token Type
        TokenType t_type = scan_next_token(source);
        if (t_type != NO_TOK && t_type != BAD_TOK)
        {
            push_token(tokens, new_token(t_type, source->data, tok_start, source->current)); // alloc Token.value
        } 

    }
    push_token(tokens, new_token(EOFI, source->data, source->current, source->current)); // alloc Token.value

    return tokens;
}