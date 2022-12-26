#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "source.h"

#ifndef __SCANNER_H_INCLUDED__
#define __SCANNER_H_INCLUDED__

#define MIN_LIST_VALUE 500

char forward(Source* source);

char current(Source* source);

int forward_if(Source* source, char expected);

TokenType comment(Source* source);

TokenType number(Source* source);

TokenType string(Source* source, int start, char tok);


TokenType word(Source* source, int start);

TokenType scan_next_token(Source* source);

bool push_token(TokensList* tokens, Token new_token);

TokensList* scan_tokens(Source* source, TokensList* tokens);

#endif