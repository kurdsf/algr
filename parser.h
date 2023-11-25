#ifndef PARSER_H
#define PARSER_H

#include"presedence.h"
#include"lexer.h"

typedef struct AST {
    #define INFO_NODE 0
    #define INFO_NUM  1
    #define INFO_VAR  2
    #define INFO_ANY  3 /* only used in rule.h */
    #define INFO_ANY_NUM 4 /* only used in rule.h */
    #define INFO_ANY_VAR 5 /* only used in rule.h */
    char info;
    union {
        int num;
        char var;
    };
    
    enum TokenType op;
    struct AST* lhs;
    struct AST* rhs;
    
} AST;



typedef AST* vec_AST;

/* Only declared here for testing reasons */
int pos_of_op_with_least_presedence(Presedence_Tokens ptokens, int begin, int end);

AST* parse(char*);


int ASTeq(AST*, AST*);

void putsAST(AST* ast);

void freeAST(AST *ast);

#endif /* PARSER_H */



