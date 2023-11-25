#include<assert.h>
#include<stdlib.h>
#include<stdio.h>


#include"vec.h"
#include"utest.h"
#include"lexer.h"
#include"presedence.h"
#include"parser.h"
#include"rule.h"
#include"optimizer.h"



UTEST_MAIN();

UTEST(lexer,get_next_token){
    Token tok;
    char* str;

    str = strdup("33333+");

    char *tmp = str;

    ASSERT_TRUE(get_next_token(&str,&tok) != -1);
    ASSERT_EQ(tok.type,Num);
    ASSERT_EQ(tok.num,33333);
    
    ASSERT_TRUE(get_next_token(&str,&tok) != -1);
    ASSERT_EQ(tok.type,Add);
    
    ASSERT_EQ(get_next_token(&str,&tok),-1);

    free(tmp);
}


UTEST(presedences,get_presedence_tokens){
    Presedence_Tokens toks;
    char* str;
    str = strdup("33 * (4 - (3 + 55555))");
    
    toks = get_presedence_tokens(str);
    vec_Token tokens = toks.tokens;
    vec_int pres = toks.presedences;
    ASSERT_EQ(vector_size(tokens),7);
    ASSERT_EQ(tokens[0].type,Num);
    ASSERT_EQ(tokens[0].num,33);
    ASSERT_EQ(tokens[1].type,Mul);
    ASSERT_EQ(tokens[2].type,Num);
    ASSERT_EQ(tokens[2].num,4);
    ASSERT_EQ(tokens[3].type,Sub);
    ASSERT_EQ(tokens[4].type,Num);
    ASSERT_EQ(tokens[4].num,3);
    ASSERT_EQ(tokens[5].type,Add);
    ASSERT_EQ(tokens[6].type,Num);
    ASSERT_EQ(tokens[6].num,55555);
    
    /* Test operator order */
    ASSERT_GT(pres[3],pres[1]);
    ASSERT_GT(pres[5],pres[3]);
    /* Where no operator is it is -1 */
    ASSERT_EQ(pres[0],-1);
    ASSERT_EQ(pres[2],-1);
    ASSERT_EQ(pres[4],-1);
    ASSERT_EQ(pres[6],-1);
    
    

    
    free_presedence_tokens(toks);
    free(str);

    str = strdup("33 ^ (x + y) ");
    
    toks = get_presedence_tokens(str);
    tokens = toks.tokens;
    pres = toks.presedences;
    ASSERT_EQ(vector_size(tokens),5);
    ASSERT_EQ(tokens[0].type,Num);
    ASSERT_EQ(tokens[0].num,33);
    ASSERT_EQ(tokens[1].type,Pow);
    ASSERT_EQ(tokens[2].type,Var);
    ASSERT_EQ(tokens[2].var,'x');
    ASSERT_EQ(tokens[3].type,Add);
    ASSERT_EQ(tokens[4].type,Var);
    ASSERT_EQ(tokens[4].var,'y');
    
    /* Test operator order */
    ASSERT_GT(pres[3],pres[1]);
    /* Where no operator is it is -1 */
    ASSERT_EQ(pres[0],-1);
    ASSERT_EQ(pres[2],-1);
    ASSERT_EQ(pres[4],-1);
    
    free_presedence_tokens(toks);
    free(str);
    
   
    
    
}


UTEST(presedences,pos_of_op_with_least_presedence){
    Presedence_Tokens toks;
    char *str;
    str = strdup("33 * ( 3 - 4 ) * 3");
    toks = get_presedence_tokens(str);
    int pos = pos_of_op_with_least_presedence(toks,0,7);
    ASSERT_EQ(pos,1);
    
    free_presedence_tokens(toks);
    free(str);
    
    str = strdup("33 - 4 + 12");
    toks = get_presedence_tokens(str);
    pos = pos_of_op_with_least_presedence(toks,0,5);
    ASSERT_EQ(pos,3);
    

    free_presedence_tokens(toks);
    free(str);

    str = strdup("33 - 4 - 12");
    toks = get_presedence_tokens(str);
    pos = pos_of_op_with_least_presedence(toks,0,5);
    ASSERT_EQ(pos,3);
    

    free_presedence_tokens(toks);
    free(str);
   
    
}

UTEST(optimizer,ASTeq){
    char* str1;
    char* str2;
    AST* ast1;
    AST* ast2;

    str1 = strdup("x + 1");
    str2 = strdup(str1);
    ast1 = parse(str1);
    ast2 = parse(str2);

    ASSERT_TRUE(ASTeq(ast1,ast2));

    freeAST(ast1);
    freeAST(ast2);
    free(str1);
    free(str2);

    str1 = strdup("(x / x) * (x - x / x * 2 + 3)");
    str2 = strdup(str1);
    ast1 = parse(str1);
    ast2 = parse(str2);

    ASSERT_TRUE(ASTeq(ast1,ast2));

    freeAST(ast1);
    freeAST(ast2);
    free(str1);
    free(str2);

    str1 = strdup("x / x * x / x - 3 * 3 / (x + x)");
    str2 = strdup(str1);
    ast1 = parse(str1);
    ast2 = parse(str2);

    ASSERT_TRUE(ASTeq(ast1,ast2));

    freeAST(ast1);
    freeAST(ast2);
    free(str1);
    free(str2);

    str1 = strdup("1 * 2 - y + x / x - y");
    str2 = strdup(str1);
    ast1 = parse(str1);
    ast2 = parse(str2);

    ASSERT_TRUE(ASTeq(ast1,ast2));

    freeAST(ast1);
    freeAST(ast2);
    free(str1);
    free(str2);
}

UTEST(rule,cloneAST){
    char* str;
    AST* ast;
    AST* ast1;

    str = strdup("3 + x - 3");
    ast = parse(str);
    ast1 = cloneAST(ast);
    ASSERT_TRUE(ASTeq(ast,ast1));

    freeAST(ast);
    freeAST(ast1);
    free(str);

    str = strdup("3 / 3 * 3 - 3");
    ast = parse(str);
    ast1 = cloneAST(ast);
    ASSERT_TRUE(ASTeq(ast,ast1));

    freeAST(ast);
    freeAST(ast1);
    free(str);

    str = strdup(" x * x * x * x * x * x * x");
    ast = parse(str);
    ast1 = cloneAST(ast);
    ASSERT_TRUE(ASTeq(ast,ast1));

    freeAST(ast);
    freeAST(ast1);
    free(str);


}

UTEST(rule,ast_fits_to_rule){
   ASSERT_TRUE(ast_fits_to_rule(
                &Any('x'),

                &Sub(
                    Var('x'),
                    Var('y')
                    )
                                ));
   ASSERT_TRUE(ast_fits_to_rule(
                &Sub(
                    Any('x'),
                    Any('x')
                    ),

                &Sub(
                    Sub(Var('x'),Var('y')),
                    Sub(Var('x'),Var('y'))
                    )
                                ));
   ASSERT_TRUE(!ast_fits_to_rule(
                &Sub(
                    Any('x'),
                    Any('x')
                    ),

                &Sub(
                    Var('y'),
                    Var('x')
                    )
                                ));
   ASSERT_TRUE(ast_fits_to_rule(
                &Sub(
                    Mul(
                        Any('x'),
                        Any('y')
                       ),
                    Any('z')
                    ),
                &Sub(
                    Mul(
                        Num(3),
                        Num(3)
                       ),
                    Num(3)
                    )
                                ));
    ASSERT_TRUE(!ast_fits_to_rule(
                &Mul(
                     Any('x'),
                     Num(0)
                     ),
                 &Mul(
                     Num(3),
                     Num(3)
                     )
                                  ));
    ASSERT_TRUE(ast_fits_to_rule(
                &Mul(
                    AnyVar('x'),
                    AnyNum('y')
                    ),
                &Mul(
                    Var('x'),
                    Num(3)
                    )
                                ));
    ASSERT_TRUE(!ast_fits_to_rule(
                &Mul(
                    AnyVar('x'),
                    AnyNum('y')
                    ),
                &Mul(
                    Num(3),
                    Var('x')
                    )
                                ));
    ASSERT_TRUE(ast_fits_to_rule(
                &Mul(
                    AnyVar('x'),
                    AnyNum('y')
                    ),
                &Mul(
                    Var(3),
                    Num(3)
                    )
                                ));
    ASSERT_TRUE(!ast_fits_to_rule(
                &Mul(
                    AnyVar('x'),
                    AnyNum('x')
                    ),
                &Mul(
                    Var(3),
                    Num(3)
                    )
                                ));
}


int rule_converts_to(AST* from, AST* to, char* inp, char* exp_out){
   /* note that from and to are muted */
   AST *ast1,*ast2;
   inp = strdup(inp);
   exp_out = strdup(exp_out);
   ast1 = parse(inp);

   assert(ast_fits_to_rule(from,ast1));
   
   ast1 = rule_substitute(from,to,ast1);

   ast2 = parse(exp_out);
   int res = ASTeq(ast1,ast2);
   free(inp);
   free(exp_out);
   freeAST(ast1);
   freeAST(ast2);
   return res;
   
    
}



UTEST(rule,rule_substitute){
    AST *from,*to;

    from = &Add(Any('x'),Any('x')); 
    to = &Mul(Num(2),Any('x'));
    ASSERT_TRUE(rule_converts_to(from,to,"3 + 3","2*3"));
    ASSERT_TRUE(rule_converts_to(from,to,"x + x","2*x"));
    
    from = &Sub(Any('x'),Any('x'));
    to = &Num(0);
    ASSERT_TRUE(!rule_converts_to(from,to,"x -  x","1"));
    ASSERT_TRUE(rule_converts_to(from,to,"x - x","0"));
    ASSERT_TRUE(rule_converts_to(from,to,"3 - 3","0"));
    ASSERT_TRUE(rule_converts_to(from,to,"(x + x) - (x + x)","0"));

    from = &Div(Any('x'),Any('x'));
    to = &Num(1);   
    ASSERT_TRUE(!rule_converts_to(from,to,"x / x","0"));
    ASSERT_TRUE(rule_converts_to(from,to,"x / x","1"));
    ASSERT_TRUE(rule_converts_to(from,to,"3 / 3","1"));
    ASSERT_TRUE(rule_converts_to(from,to,"(x + x) / (x + x)","1"));


    /* NOTE: This rule does not make sense */
    from = &Div(AnyNum('x'),AnyNum('x'));
    to = &Mul(AnyNum('x'),Num(1));
    ASSERT_TRUE(rule_converts_to(from,to,"3 / 3 ","3 * 1"));
    ASSERT_TRUE(rule_converts_to(from,to,"4 / 4", "4 * 1"));

    /* NOTE: This rule does not make sense */
    from = &Div(AnyNum('x'),AnyVar('y'));
    to = &Mul(AnyNum('x'),Num(1));
    ASSERT_TRUE(rule_converts_to(from,to,"3 / x ","3 * 1"));
    ASSERT_TRUE(rule_converts_to(from,to,"4 / k", "4 * 1"));

    from = &Div(Any('x'),Any('y'));
    to = &Div(Any('y'),Any('x'));
    ASSERT_TRUE(rule_converts_to(from,to,"x / y","y / x"));
    ASSERT_TRUE(rule_converts_to(from,to,"(x - x) / y","y / (x - x)"));
    
    
}


int is_optimized_to(char* inp, char* exp_out){
    inp = strdup(inp);
    exp_out = strdup(exp_out);

    AST* ast_out = parse(inp);
    ast_out = optimize(ast_out);

    AST* ast_exp_out = parse(exp_out);

    int res = ASTeq(ast_out,ast_exp_out);
    
    freeAST(ast_exp_out);
    freeAST(ast_out);
    free(inp);
    free(exp_out);
    return res;
}



UTEST(optimizer,constexpr){
    ASSERT_TRUE(is_optimized_to("0","0"));
    ASSERT_TRUE(is_optimized_to("3*4","12"));
    ASSERT_TRUE(is_optimized_to("1 + 1 + 1 * 2","4"));
    ASSERT_TRUE(is_optimized_to("1 * 1 * 1","1"));
    ASSERT_TRUE(is_optimized_to("2 ^ 2","4"));
}


UTEST(optimizer,lhs_eq_rhs){
    ASSERT_TRUE(is_optimized_to("(x - x) + (x - x) ","0"));
    ASSERT_TRUE(is_optimized_to("(x / x) * (x - x)","0"));
    ASSERT_TRUE(is_optimized_to("1 + 1 + x / x","3"));
    ASSERT_TRUE(is_optimized_to("(x + x - x)/(x + x - x)","1"));
}

UTEST(optimizer,some_side_is_one){
    ASSERT_TRUE(is_optimized_to("((x - x) + 1)/1","1"));
    ASSERT_TRUE(is_optimized_to("1 * y","y"));
    ASSERT_TRUE(is_optimized_to("(x / x) * x","x"));
    ASSERT_TRUE(is_optimized_to("z * 1","z"));
    
}

UTEST(optimizer,some_side_is_zero){
    ASSERT_TRUE(is_optimized_to("(x - x) * x","0"));
    ASSERT_TRUE(is_optimized_to("(x - x) * (x + x)","0"));
    ASSERT_TRUE(is_optimized_to("x * 0","0"));
    ASSERT_TRUE(is_optimized_to("0 * (x - x * x / x - 3 * 4)","0"));
    ASSERT_TRUE(is_optimized_to("(x - x) * y","0"));

}

UTEST(optimizer,const_outmul){
    ASSERT_TRUE(is_optimized_to("3*x + 4*x","7*x"));
    ASSERT_TRUE(is_optimized_to("x*3 + 4*x","7*x"));
    ASSERT_TRUE(is_optimized_to("(x/x)*x + 3*x","4*x"));
    ASSERT_TRUE(is_optimized_to("(0 - 1)*x + 1*x","0"));
}

UTEST(optimizer,assoc){
    ASSERT_TRUE(is_optimized_to("(3 + x) + 4","7 + x"));
    ASSERT_TRUE(is_optimized_to("4 + (3 + x)","7 + x"));
}


UTEST(optimizer,fraction){
    ASSERT_TRUE(is_optimized_to("30/40","3/4"));
    ASSERT_TRUE(is_optimized_to("3/4 + 2/4","5/4"));
    ASSERT_TRUE(is_optimized_to("3/4 - 2/4","1/4"));
    ASSERT_TRUE(is_optimized_to("(3/4) * (2/4)","3/8"));
    ASSERT_TRUE(is_optimized_to("(3/4) / (2/4)","3/2"));
}


UTEST(optimizer,pow_arith){
    ASSERT_TRUE(is_optimized_to("(x^y)^z","x^(y*z)"));
    ASSERT_TRUE(is_optimized_to("x ^ a * x ^ b","x ^ ( a + b )"));
    ASSERT_TRUE(is_optimized_to("x ^ a / x ^ b ","x ^ ( a - b)"));
    ASSERT_TRUE(is_optimized_to("x ^ a * y ^ a","(x * y) ^ a"));
    ASSERT_TRUE(is_optimized_to("x ^ a / y ^ a","(x / y) ^ a"));
    
}

