#include "Includes/parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "setjmp.h"
#include "Includes/expr.h"

jmp_buf hadFailed;

// static int forward_if(TokensList* tokens, TokenType expected)
// {
//     return tokens->current >= tokens->size ? false : tokens->arr[tokens->current].type == expected ? tokens->current++ : false;
// }

static int current(TokensList* tokens, TokenType expected)
{
    return tokens->current >= tokens->size ? false : tokens->arr[tokens->current].type == expected ? true : false;
}

void parseTokens(TokensList* tokens)
{
    tokens->current = 0;
    switch(setjmp(hadFailed))
    {
        case 0:
            parseExpression(tokens);
            break;
        case 67: //Closing Parenthesis Not Found While Parsing
            printf("Error! Closing Braces Not Found");
            break;
         case 66: //Closing Parenthesis Not Found While Parsing
            printf("Error! End of Primary Function");
            break;
    }
}

Expr* parseExpression(TokensList* tokens)
{
    return parseEquality(tokens);
}

Expr* parseEquality(TokensList* tokens)
{
    Expr* expr = parseComparison(tokens);
    while (current(tokens, NOT_EQUAL) || current(tokens, EQUAL_EQUAL)) {
        expr = createBinaryExpr(expr, tokens->arr[tokens->current++], parseComparison(tokens));
    }

    return expr;
}

Expr* parseComparison(TokensList* tokens)
{
    Expr* expr = parseTerm(tokens);
    while (current(tokens, GREATER) || current(tokens, GREATER_EQUAL) ||
           current(tokens, LESS) || current(tokens, LESS_EQUAL)) {
        expr = createBinaryExpr(expr, tokens->arr[tokens->current++], parseTerm(tokens));
    }

    return expr;
}

Expr* parseTerm(TokensList* tokens)
{
    Expr* expr = parseFactor(tokens);
    while (current(tokens, PLUS) || current(tokens, MINUS)) {
        expr = createBinaryExpr(expr, tokens->arr[tokens->current++], parseFactor(tokens));
    }

    return expr;
}

Expr* parseFactor(TokensList* tokens)
{
    Expr* expr = parseUnary(tokens);
    while (current(tokens, STAR) || current(tokens, SLASH)) {
        expr = createBinaryExpr(expr, tokens->arr[tokens->current++], parseUnary(tokens));
    }

    return expr;
}

Expr* parseUnary(TokensList* tokens)
{
    if(current(tokens, NOT) || current(tokens, MINUS))
    {
        return createUnaryExpr(tokens->arr[tokens->current++], parseUnary(tokens));
    }
    return parsePrimary(tokens);
}

Expr* parsePrimary(TokensList* tokens)
{
    if(current(tokens, TRUE) || current(tokens, FALSE) || 
       current(tokens, NUMBER) || current(tokens, STRING)) {
        Expr* expr = createLiteralExpr(tokens->arr[tokens->current++].value);
        return expr;
    }
    
    if (current(tokens, LEFT_PAREN)) {
        Expr* expr = parseExpression(tokens);
        if(! current(tokens, RIGHT_PAREN))
        {
            longjmp(hadFailed, 67);
        }
        return createGroupExpr(expr);
    }
    longjmp(hadFailed, 66);
}
