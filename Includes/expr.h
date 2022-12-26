#ifndef __EXPR_H_INCLUDED__
#define __EXPR_H_INCLUDED__

#include "token.h"

typedef struct _expr Expr;
enum expr_kind{
    BinaryExpr, UnaryExpr, LITERAL, GROUP
};

struct _expr{
    enum expr_kind kind;
    union{
        struct{
            Expr* lhs;
            Token op;
            Expr* rhs;
        }Binary;

        struct{
            Token op;
            Expr* rhs;
        }Unary;

        struct{
            char* value;
        }Literal;

        struct{
            Expr* expr;
        }Group;
    };
};

Expr* createBinaryExpr(Expr* lhs, Token op, Expr* rhs);

Expr* createUnaryExpr(Token op, Expr* rhs);

Expr* createLiteralExpr(char* value);

Expr* createGroupExpr(Expr* expr);

#endif