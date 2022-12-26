#ifndef __PARSER_H_INCLUDED__
#define __PARSER_H_INCLUDED__

#include "expr.h"

void parseTokens(TokensList* tokens);

Expr* parseExpression(TokensList* tokens);

Expr* parseEquality(TokensList* tokens);

Expr* parseComparison(TokensList* tokens);

Expr* parseTerm(TokensList* tokens);

Expr* parseFactor(TokensList* tokens);

Expr* parseUnary(TokensList* tokens);

Expr* parsePrimary(TokensList* tokens);

int fwd_tok_if(TokensList* tokens, TokenType expected);

#endif