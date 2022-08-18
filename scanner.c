#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "source.h"
#include "token.h"
#include "utils.h"


#define MIN_LIST_VALUE 500
#define REALLOC_MULTIPLIER 2


char next(Source* source)
{
    return (source->current >= source->size) ? '\0' : source->data[source->current++];
}

char look_next(Source* source)
{
    return (source->current >= source->size) ? '\0' : source->data[source->current];
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

TokenType slash_token(Source* source, int start)
{
    if (next_if(source, '=')) return SLASH_EQUAL;

    else if (next_if(source, '/')){
        
        //since it's a comment, eat charaters till '\n' or '\0'
        for(char ch = next(source); ch != '\0'; ch=next(source))
            if (ch == '\n') {
                source->line++;
                break;
            }

        return NO_TOK;
    }
    else return SLASH;
}

TokenType number_token(Source* source, int start)
{
    //since it's a number, eat charaters till a character except 0-9
    for(char ch = look_next(source); ch >= '0' && ch <= '9'; ch=look_next(source)) source->current++;
    return NUMBER;
}

TokenType string_token(Source* source, int start, char tok)
{
    //since it's a string, eat charaters till a " or ' whatever is in tok
    for(char ch = next(source); ch != tok; ch=next(source)){
        if (ch == '\0' || ch == '\n'){
            //Print an Error
            log_line(source->data, start, source->current);
            printf("[ERROR] Unterminated String at line %lu\n", source->line);
            return NO_TOK;
        }
    }
    return STRING;
}


TokenType alphabet_token(Source* source, int start)
{
    // get all adjacent a-z A-Z _ 0-9 characters
    for(char ch = look_next(source); 
    ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z' || ch >= '0' && ch <= '9' || ch == '_'; 
    ch=look_next(source)) source->current++;
    char* sub_str = slice(source->data, start, source->current); // alloc-4 sub_str
    
    //decide whether its a keyword or a string
    TokenType type = match_kwrd(sub_str);
    free(sub_str); // free-4 sub_str
    return type == NO_TOK ? STRING : type;
}

TokenType scan_next_token(Source* source)
{
    int start = source->current;
    char* value;
    char ch = next(source);
    int end = source->current;
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
    //Double Characters Tokens
    case '!': return next_if(source, '=') ? NOT_EQUAL : NOT;
    case '=': return next_if(source, '=') ? EQUAL_EQUAL : EQUAL;
    case '>': return next_if(source, '=') ? GREATER_EQUAL : GREATER;
    case '<': return next_if(source, '=') ? LESS_EQUAL : LESS;
    case '*': return next_if(source, '=') ? STAR_EQUAL : STAR;
    //Triple Characters Tokens
    case '+': return next_if(source, '+') ? PLUS_PLUS : next_if(source, '=') ? PLUS_EQUAL : PLUS;
    
    case '-': return next_if(source, '-') ? MINUS_MINUS : next_if(source, '=') ? MINUS_EQUAL : MINUS;
    case '/': return slash_token(source, start);
    //Useless Characters
    case ' ':
    case '\r':
    case '\t':
            return NO_TOK;
    case '\n':
            source->line++;
            return NO_TOK;
    //For 0-9 Character
    case 48 ... 57: return number_token(source, start);
    // For Alphabet and _ Character
    case 'a' ... 'z':
    case 'A' ... 'Z':
    case '_':
            return alphabet_token(source,start);
    // For Quoted Strings
    case '"':
    case '\'':
            return string_token(source, start, ch);
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
    tokens->size = MIN_LIST_VALUE;
    
    tokens->arr = (Token*) calloc(tokens->size, sizeof(Token)); // alloc-2 tokens->arr
    if (tokens->arr == NULL) {  printf("[ERROR] Mem Not Allocated for Tokens Array"); exit(2); }

    tokens->current = 0;
    for (int start = 0; source->current < source->size; start = source->current) {
        // Getting next Token Type
        TokenType t_type = scan_next_token(source);
        if (t_type != NO_TOK && t_type != BAD_TOK)
        {
            push_token(tokens, new_token(t_type, source->data, start, source->current)); // alloc Token.value
        } 

    }
    push_token(tokens, new_token(EOFI, source->data, source->current, source->current)); // alloc Token.value

    return tokens;
}