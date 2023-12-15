#include<assert.h>
#include"algr.h"
#include"rule.h"
#include"optimizer.h"

static AST* perform_single_pass(AST* ast);

// Passes which cannot be written as rules.
static AST* eval_constexpr(AST* ast);
static AST* reduce_fraction(AST* ast);

static char is_constexpr(AST* ast);
static int _eval_constexpr(AST* ast);

RULE(mul_lhs_by_zero,
     &Mul(Any('x'),Num(0)),
     &Num(0))

RULE(mul_rhs_by_zero,
     &Mul(Num(0),Any('x')),
     &Num(0))

RULE(mul_lhs_by_one,
     &Mul(Any('x'),Num(1)),
     &Any('x'))

RULE(mul_rhs_by_one,
     &Mul(Num(1),Any('x')),
     &Any('x'))

RULE(div_lhs_by_one,
     &Div(Any('x'), Num(1)),
     &Any('x'))

RULE(add_rhs_by_zero,
     &Add(Num(0),Any('x')),
     &Any('x'))
      
RULE(add_lhs_by_zero,
     &Add(Any('x'),Num(0)),
     &Any('x'))
RULE(sub_rhs_by_zero,
     &Sub(Any('x'),Num(0)),
     &Any('x'))
RULE(div_rhs_by_zero,
     &Div(Num(0),Any('x')),
     &Num(0))

RULE(sub_x_by_x,
     &Sub(Any('x'), Any('x')),
     &Num(0))

RULE(div_x_by_x,
     &Div(Any('x'),Any('x')),
     &Num(1))

RULE(turn_around_frac, 
     &Div(Num(1), Div(Any('p'), Any('q'))),
     &Div(Any('q'), Any('p')))

RULE(sort_var_mul_const,
     &Mul(Any('x'),AnyNum('a')),
     &Mul(AnyNum('a'),Any('x')))

RULE(sort_var_add_const,
     &Add(Any('x'),AnyNum('a')),
     &Add(AnyNum('a'),Any('x')))

RULE(const_outmul,
     &Add(Mul(AnyNum('a'),AnyVar('x')),Mul(AnyNum('b'),AnyVar('x'))),
     &Mul(Add(AnyNum('a'),AnyNum('b')),AnyVar('x')))

RULE(const_outmul_2,
     &Add(Mul(AnyNum('a'),AnyVar('x')),AnyVar('x')),
     &Mul(Add(AnyNum('a'),Num(1)),AnyVar('x')))

RULE(const_outmul_3,
     &Add(AnyVar('x'),Mul(AnyNum('b'),AnyVar('x'))),
     &Mul(Add(Num(1),AnyNum('b')),AnyVar('x')))
      
RULE(assoc_const_add,
     &Add(AnyNum('a'), Add(AnyNum('b'),AnyVar('x'))),
     &Add(Add(AnyNum('a'),AnyNum('b')),AnyVar('x')))

RULE(assoc_const_mul,
     &Mul(AnyNum('a'), Mul(AnyNum('b'),AnyVar('x'))),
     &Mul(Mul(AnyNum('a'),AnyNum('b')),AnyVar('x')))

RULE(fraction_add,
    &Add(Div(Any('a'),Any('b')),Div(Any('c'),Any('d'))),
    &Div(Add(Mul(Any('a'),Any('d')),Mul(Any('b'),Any('c'))),Mul(Any('b'),Any('d'))))

RULE(fraction_sub,
    &Sub(Div(Any('a'),Any('b')),Div(Any('c'),Any('d'))),
    &Div(Sub(Mul(Any('a'),Any('d')),Mul(Any('b'),Any('c'))),Mul(Any('b'),Any('d'))))

RULE(fraction_mul,
    &Mul(Div(Any('a'),Any('b')),Div(Any('c'),Any('d'))),
    &Div(Mul(Any('a'),Any('c')),Mul(Any('b'),Any('d'))))

RULE(fraction_div,
    &Div(Div(Any('a'),Any('b')),Div(Any('c'),Any('d'))),
    &Div(Mul(Any('a'),Any('d')),Mul(Any('b'),Any('c'))))

RULE(pow_mul,
    &Pow(Pow(Any('x'),Any('y')),Any('z')),
    &Pow(Any('x'),Mul(Any('y'),Any('z'))))

RULE(pow_add,
    &Mul(Pow(Any('x'),Any('a')),Pow(Any('x'),Any('b'))),
    &Pow(Any('x'),Add(Any('a'),Any('b'))))

RULE(pow_sub,
    &Div(Pow(Any('x'),Any('a')),Pow(Any('x'),Any('b'))),
    &Pow(Any('x'),Sub(Any('a'),Any('b'))))


RULE(pow_mul_same_exp,
    &Mul(Pow(Any('x'),Any('a')),Pow(Any('y'),Any('a'))),
    &Pow(Mul(Any('x'),Any('y')),Any('a')))

RULE(pow_div_same_exp,
    &Div(Pow(Any('x'),Any('a')),Pow(Any('y'),Any('a'))),
    &Pow(Div(Any('x'),Any('y')),Any('a')))


AST* optimize(AST* ast){
    for(int i = 0; i < NUM_PASSES; i++){
        ast = perform_single_pass(ast);
    }
    return ast;

}

static AST* perform_single_pass(AST* ast){
    if(ast->info == INFO_NODE){
        ast = eval_constexpr(ast);
        ast = sort_var_mul_const(ast);
        ast = sort_var_add_const(ast);
        ast = const_outmul(ast);
        ast = const_outmul_2(ast);
        ast = const_outmul_3(ast);
        ast = mul_lhs_by_zero(ast);
        ast = mul_rhs_by_zero(ast);
        ast = mul_lhs_by_one(ast);
        ast = mul_rhs_by_one(ast);
        ast = div_lhs_by_one(ast);
        ast = add_rhs_by_zero(ast);
        ast = add_lhs_by_zero(ast);
        ast = sub_rhs_by_zero(ast);
        ast = div_rhs_by_zero(ast);
        ast = turn_around_frac(ast);
        ast = div_x_by_x(ast);
        ast = sub_x_by_x(ast);
        ast = assoc_const_add(ast);
        ast = assoc_const_mul(ast);
        ast = reduce_fraction(ast);
        ast = fraction_add(ast);
        ast = fraction_sub(ast);
        ast = fraction_mul(ast);
        ast = fraction_div(ast);
        ast = pow_mul(ast);
        ast = pow_add(ast);
        ast = pow_sub(ast);
        ast = pow_mul_same_exp(ast);
        ast = pow_div_same_exp(ast);

        if(ast->info == INFO_NODE){
            ast->lhs = perform_single_pass(ast->lhs);
            ast->rhs = perform_single_pass(ast->rhs);
        }
    }
    return ast;
}


static AST* eval_constexpr(AST* ast){
    if(is_constexpr(ast)){
        int res = _eval_constexpr(ast);
        freeAST(ast);
        return cloneAST(&Num(res));
    }
    
    return ast;
}


static char is_constexpr(AST* ast){
    if(ast->info == INFO_NODE){
        if(ast->op == Div) return 0;
        return is_constexpr(ast->lhs) && is_constexpr(ast->rhs);
    }
    return ast->info == INFO_NUM; 
}


static int powi(int base, int exp){
    int res = 1;
    for(int i = 0; i < exp; i++){
        res *= base;
    }
    return res;
}

static int  _eval_constexpr(AST* ast){
    if(ast->info == INFO_NODE){
        int lhs = _eval_constexpr(ast->lhs);
        int rhs = _eval_constexpr(ast->rhs);
        switch(ast->op){
            case Add:
                return lhs+rhs;
            case Sub:
                return lhs-rhs;
            case Mul:
                return lhs*rhs;
            case Pow:
                return powi(lhs,rhs);
            default:
                unreachable();
        }
    }
    
    assert(ast->info == INFO_NUM);
    return ast->num;
    
}


static int gcdi(int a, int b){
    while(a != b){
        if(a > b)
            a -= b;
        else 
            b -= a;
    }
    return a;
}



static AST* reduce_fraction(AST* ast){
    if(ast->info == INFO_NODE
        && ast->lhs->info == INFO_NUM
        && ast->rhs->info == INFO_NUM){
        int a = ast->lhs->num;
        int b = ast->rhs->num;
        if(b == 0) {
            fprintf(stderr, "Error: division by 0.\n");
            abort();
        }
        int d = gcdi(a,b);
        a /= d;
        b /= d;
        ast->lhs->num = a;
        ast->rhs->num = b;
    }
    return ast;
    
}

