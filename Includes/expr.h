#include "token.h"

typedef struct _expr Expr;
enum expr_kind{
    BinaryExpr, UnaryExpr, VALUE, GROUP
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
        }Value;

        struct{
            Expr* expr;
        }Group;
    };
};