#include<assert.h>
#include<stdio.h>
#include<stdlib.h>

#include"algr.h"
#include"utest.h"
#include"vec.h"
#include"parser.h"
#include"rule.h"

int isAny(AST*);
int isAnyNum(AST*);
int isAnyVar(AST*);


// returns the pos of the first instance of b in a,
// if there is none, returns -1.
static int find_first_char(vec_char a, char b);


// If for example rule = Add(Sub(Any,Any),Any) then
// check that ast begins with Add(Sub(...,...),...) where
// ... can be any value.
static int ast_shape_fits_to_rule(AST* rule, AST* ast);

// If for example rule = Add(Any(x),Any(x)) then 
// check that lhs and rhs of ast are equal.
static int ast_same_anys_are_equal(AST* rule, AST* ast);


static void ast_get_all_anys(AST* rule, AST* ast, vec_AST** anys, vec_char* any_names);


static int find_first_char(vec_char a, char b){
    for(int i = 0; i < vector_size(a); i++){
        if(a[i] == b){
            return i;
        }
    }
    return -1;
}


static int ast_shape_fits_to_rule(AST* rule, AST* ast){
    if(isAny(rule)){
        return 1;
    }
    else if(isAnyNum(rule)){
        return ast->info == INFO_NUM;
    }
    else if(isAnyVar(rule)){
        return ast->info == INFO_VAR;
    }
    else if (rule->info != ast->info){
        return 0;
    }
    else if(rule->info == INFO_NODE){
        return ast_shape_fits_to_rule(rule->lhs,ast->lhs)
            && ast_shape_fits_to_rule(rule->rhs,ast->rhs)
            && rule->op == ast->op;
    }
    else if(rule->info == INFO_VAR){
        return rule->var == ast->var;
    }
    else if(rule->info == INFO_NUM){
        return rule->num == ast->num;
    }
    else {
        /* unknown rule->info */
        unreachable();
    }

}


// Note: `anys` and `any_names` must point to vec_empty();
// If some node of rule is Any('x'), 
// the first vector we save the name of the any,
// while in the second vector we save a pointer to the corresponding
// node of ast.
static void ast_get_all_anys(AST* rule, AST* ast, vec_AST** anys, vec_char* any_names){
    if(isAny(rule) || isAnyVar(rule) || isAnyNum(rule)){
        vector_add(anys, ast);
        vector_add(any_names,(char) rule->num);
    }
    else if(rule->info == INFO_NODE){
        ast_get_all_anys(rule->lhs,ast->lhs,anys,any_names);
        ast_get_all_anys(rule->rhs,ast->rhs,anys,any_names);
    }
}



static int ast_same_anys_are_equal(AST* rule, AST* ast){
    vec_AST* anys = vector_create();
    vec_char any_names = vector_create();
    ast_get_all_anys(rule,ast,&anys,&any_names);

    assert(vector_size(anys) == vector_size(any_names));
    
    for(size_t i = 0; i < vector_size(anys); i++){
        for(size_t j = i; j < vector_size(anys); j++){ 
            // If the names of the any's are the 
            // same, the any's themselves shall also 
            // be the same.
            if(any_names[j] == any_names[i]){
                if(!ASTeq(anys[j],anys[i])){
                    vector_free(anys);
                    vector_free(any_names);
                    return 0;
                }
            }
        }
    }
    vector_free(anys);
    vector_free(any_names);
    return 1;
}



int ast_fits_to_rule(AST* rule,AST* ast){
    return ast_shape_fits_to_rule(rule,ast)
        && ast_same_anys_are_equal(rule,ast);
}

/* Notice that this function is mutating the AST* to */
AST* rule_substitute(AST* from, AST* to, AST* ast){
    vec_AST* anys = vector_create();
    vec_char any_names = vector_create();
    ast_get_all_anys(from,ast,&anys,&any_names);
    
    to = cloneAST(to); 

    vec_AST* anys_to = vector_create();
    vec_char any_names_to = vector_create();
    ast_get_all_anys(to,to,&anys_to,&any_names_to);


    
    

    assert(vector_size(anys) == vector_size(any_names));
    assert(vector_size(anys_to) == vector_size(any_names_to));
    
    for(int i = 0; i < vector_size(anys_to); i++){
        int j = find_first_char(any_names,any_names_to[i]);
        if(j == -1){
            fprintf(stderr,"Any('%c') not found in `from` of the rule.\n",any_names_to[i]);
            unreachable();
        }
        AST* new = cloneAST(anys[j]);
        *(anys_to[i]) = *new;
        free(new); 
    }
    

    vector_free(anys);
    vector_free(any_names);
    vector_free(anys_to);
    vector_free(any_names_to);

    freeAST(ast);

    return to;
}



AST* cloneAST(AST* ast){
    AST* res;
    res = malloc(sizeof(AST));
    if(res == NULL){
        perror("algr: rule");
        abort();
    }
    memcpy(res,ast,sizeof(AST));
    if(ast->info == INFO_NODE){
        res->lhs = cloneAST(res->lhs);
        res->rhs = cloneAST(res->rhs);
    }
    return res;
}

int isAny(AST* ast){
    return ast->info == INFO_ANY;
}

int isAnyNum(AST* ast){
    return ast->info == INFO_ANY_NUM;
}


int isAnyVar(AST* ast){
    return ast->info == INFO_ANY_VAR;
}

