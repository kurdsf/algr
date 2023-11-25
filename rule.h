#ifndef RULE_H
#define RULE_H

#include<stdlib.h>

#include"lexer.h"
#include"parser.h"


/* We want to define rules like this:
    rule: x - x => 0
    RULE(x_sub_x_is_zero,Sub(Any('x'),Any('x')),Num(0))
    rule: x / x => 1
    RULE(x_div_x_is_one,Div(Any('x'),Any('x')),Num(1))
    rule a*x + b*x = (a + b)*x.
    RULE(rule_3 // dont know the name 
                   Add(Mul(Any('a'),Any('x')),Mul(Any('b'),Any('x')))
                   Mul(Add(Any('a'),Any('b')),Any('x')))
*/
    

int ast_fits_to_rule(AST* rule,AST* ast);
AST* rule_substitute(AST* from, AST* to, AST* ast); 

#define Add(x,y) (AST) {.info = INFO_NODE, .op = Add, .lhs = &(x), .rhs = &(y)}
#define Sub(x,y) (AST) {.info = INFO_NODE, .op = Sub, .lhs = &(x), .rhs = &(y)}
#define Mul(x,y) (AST) {.info = INFO_NODE, .op = Mul, .lhs = &(x), .rhs = &(y)}
#define Div(x,y) (AST) {.info = INFO_NODE, .op = Div, .lhs = &(x), .rhs = &(y)}
#define Pow(x,y) (AST) {.info = INFO_NODE, .op = Pow, .lhs = &(x), .rhs = &(y)}
#define Num(x) (AST) {.info = INFO_NUM, .num = (x)}
#define Var(x) (AST) {.info = INFO_VAR, .var = (x)}
#define Any(x) (AST) {.info = INFO_ANY, .num = (x)}

#define AnyNum(x) (AST) {.info = INFO_ANY_NUM, .num = (x)}
#define AnyVar(x) (AST) {.info = INFO_ANY_VAR, .num = (x)}


AST* cloneAST(AST* ast);



#define RULE(name,from,to)                       \
AST* name(AST* ast) {                            \
    if(ast_fits_to_rule(from,ast)){              \
        ast = rule_substitute(from,to,ast);      \
    }                                            \
    return ast;                                  \
}                          
#endif /* RULE_H */

