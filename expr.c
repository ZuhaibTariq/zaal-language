#include "Includes/expr.h"
#include <stdlib.h>

Expr* createBinaryExpr(Expr* lhs, Token op, Expr* rhs)
{
    Expr* temp = (Expr*) malloc(sizeof(Expr)); //alloc-3 Expr
    temp->kind = BinaryExpr;
    temp->Binary.lhs = lhs;
    temp->Binary.op = op;
    temp->Binary.rhs = rhs;
    return temp;

}

Expr* createUnaryExpr(Token op, Expr* rhs)
{
    Expr* temp = (Expr*) malloc(sizeof(Expr)); //alloc-5 Expr
    temp->kind = UnaryExpr;
    temp->Unary.op = op;
    temp->Unary.rhs = rhs;
    return temp;
}

Expr* createLiteralExpr(char* value)
{
    Expr* temp = (Expr*) malloc(sizeof(Expr)); //alloc-6 Expr
    temp->kind = LITERAL;
    temp->Literal.value = value;
    return temp;
}

Expr* createGroupExpr(Expr* expr)
{
    Expr* temp = (Expr*) malloc(sizeof(Expr)); //alloc-7 Expr
    temp->kind = GROUP;
    temp->Group.expr = expr;
    return temp;
}