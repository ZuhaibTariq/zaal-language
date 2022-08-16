#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "source.h"

#ifndef __SCANNER_H_INCLUDED__
#define __SCANNER_H_INCLUDED__

#define MIN_LIST_VALUE 500

char next(Source* source);

char look_next(Source* source);

bool next_if(Source* source, char expected);

Token slash_token(Source* source, int start);

Token number_token(Source* source, int start);

Token string_token(Source* source, int start, char tok);


Token alphabet_token(Source* source, int start);

TokenType scan_next_token(Source* source);

bool push_token(TokensList* tokens, Token new_token);

TokensList* scan_tokens(Source* source, TokensList* tokens);

#endif