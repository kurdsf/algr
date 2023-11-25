#include<stdlib.h>
#include<stdio.h>

#include"algr.h"
#include"parser.h"
#include"vec.h"
#include"lexer.h"
#include"presedence.h"



AST* parse_section(Presedence_Tokens ptokens, int begin, int end);
AST* parse_ptokens(Presedence_Tokens ptokens);

AST* parse(char* input){
    Presedence_Tokens ptokens = get_presedence_tokens(input);
    AST* res = parse_ptokens(ptokens);
    free_presedence_tokens(ptokens);
    return res;
}




int pos_of_op_with_least_presedence(Presedence_Tokens ptokens, int begin, int end){
    int min_pos = begin;
    vec_int presedences = ptokens.presedences;
    for(int i = begin; i < end; i++){
        if(presedences[min_pos] == -1){
            min_pos = i;
        }
        if(presedences[min_pos] > presedences[i] && presedences[i] != -1){
            min_pos = i;
        }
    }
    if(is_left_assoc(ptokens.tokens[min_pos])){
        min_pos = begin;
        for(int i = begin; i < end; i++){
            if(presedences[min_pos] == -1){
                 min_pos = i;
            }
            if(presedences[min_pos] >= presedences[i] && presedences[i] != -1){
                min_pos = i;
            }
        }
    
    }
    return min_pos;
}


AST* parse_section(Presedence_Tokens ptokens, int begin, int end){
    if(end - begin == 1){
        AST* ast = malloc(sizeof(AST));
        if(ast == NULL){
            perror("algr: parser");
            abort();
        }
        Token currTok = ptokens.tokens[begin];
        if(currTok.type == Num){
            ast->info = INFO_NUM;
            ast->num = currTok.num;
        }
        else if(currTok.type == Var){
            ast->info = INFO_VAR;
            ast->var = currTok.var;
        }
        else {
            /* should be checked by the lexer */
            unreachable();
        }
        return ast;
    }
    
    int pos = pos_of_op_with_least_presedence(ptokens,begin,end);
    AST* ast = malloc(sizeof(AST));
    if(ast == NULL){
        perror("algr: parser");
        abort();
    }
    ast->info = INFO_NODE;
    ast->op = ptokens.tokens[pos].type;
    ast->lhs = parse_section(ptokens,begin,pos);
    ast->rhs = parse_section(ptokens,pos+1,end);
    
    return ast;
    
    
}




AST* parse_ptokens(Presedence_Tokens ptokens){
   AST* res =  parse_section(ptokens,0,vector_size(ptokens.tokens));
   return res;
}

void freeAST(AST* ast){
    if(ast->info == INFO_NODE ){
        freeAST(ast->lhs);
        freeAST(ast->rhs);
    }
    free(ast);
}

int ASTeq(AST* a, AST* b){
    if(a->info != b->info){
        return 0;
    }
    if(a->info == INFO_NODE){
        return ASTeq(a->lhs,b->lhs) && ASTeq(a->rhs,b->rhs);
    }
    if(a->info == INFO_NUM){
        return a->num == b->num;
    }
    if(a->info == INFO_VAR){
        return a->var == b->var;
    }
    /* unknown info of a */
    unreachable();
}


    
void putsAST(AST* ast){
    if(ast->info == INFO_NODE){
        if(ast->lhs->info == INFO_NODE)
            putchar('(');

        putsAST(ast->lhs);

        if(ast->lhs->info == INFO_NODE)
            putchar(')');

        switch(ast->op){
            case Add:
                putchar('+');
                break;
            case Sub:
                putchar('-');
                break;
            case Mul:
                putchar('*');
                break;
            case Div:
                putchar('/');
                break;
            case Pow:
                putchar('^');
                break;
            default:
                /* invalid type */
                unreachable();
        
        }

        if(ast->rhs->info == INFO_NODE)
            putchar('(');

        putsAST(ast->rhs);

        if(ast->rhs->info == INFO_NODE)
            putchar(')');
        
    }
    else if(ast->info == INFO_VAR){
        putchar(ast->var);
    }
    else if(ast->info == INFO_NUM) {
        printf("%d",ast->num);
    }
    else if(ast->info == INFO_ANY){
        fprintf(stderr,"\nWarning: Should not print `Any`,\n"
                       "for internal use only.\n");
                    
        printf("Any{%c}",ast->num);
    }
    else if(ast->info == INFO_ANY_NUM){
        fprintf(stderr,"\nWarning: Should not print `AnyNum`,\n"
                       "for internal use only.\n");
        printf("AnyNum{%c}",ast->num);
    }
    else if(ast->info == INFO_ANY_VAR){
        fprintf(stderr,"\nWarning: Should not print `AnyVar`,\n"
                       "for internal use only.\n");
        printf("AnyVar{%c}",ast->num);
    }
    /* ast->info not known */
    else {
        unreachable();
    }
}



