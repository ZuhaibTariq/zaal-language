#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "source.h"

#ifndef __SCANNER_H_INCLUDED__
#define __SCANNER_H_INCLUDED__

#define MIN_LIST_VALUE 500

char forward(Source* source, char _if);

char current(Source* source);

int forward_if(Source* source, char expected);

Token comment(Source* source, int start);

Token number(Source* source, int start);

Token string(Source* source, int start, char tok);


Token word(Source* source, int start);

TokenType scan_next_token(Source* source);

bool push_token(TokensList* tokens, Token new_token);

TokensList* scan_tokens(Source* source, TokensList* tokens);

#endif