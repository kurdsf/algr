#include<ctype.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include"vec.h"
#include"algr.h"
#include"lexer.h"

/* TODO: Find out what functions need to be changed if 
         you would like to add another token */

/* Internal functions */

int step_till_next_symbol(char**);

char is_single_char_token(char);

char is_whitespace(char);

_Noreturn void panic_invalid_token(char);

void step_till_end_of_num(char**);

int get_trailing_num(char*);



/* This means that given an
   expression like "3 op  3 op 4 op 3" we
   evaluate it as "3 op (3 op (4 op 3))".
   If this is not the case, we evaluate it 
   as "(((3 op 3) op 3) 4) op 3"
   where op is '+' or '-' etc.
*/
int is_left_assoc(Token op){
    switch(op.type){
        case Add:
        case Mul:
            return 0;
        case Sub:
        case Div:
        case Pow:
            return 1;
        default:
            /* The caller should prevent 
               that op is not an operator */
            unreachable();
    }
}




/* Returns -1 is there is no next symbol */
int step_till_next_symbol(char** strp){
    char* str = *strp;
    while(is_whitespace(*str))
        str++;
    if(*str == '\0') return -1;
    *strp = str;
    return 0;
}


char is_single_char_token(char c){
	return c == '('
            || c == ')'
            || c == '+'
 	        || c == '-'
            || c == '*'
            || c == '^'
	        || c == '/';
}

char is_whitespace(char c){
	return c == ' '
	    || c == '\t'
	    || c == '\n';
}


_Noreturn void panic_invalid_token(char c){
	fprintf(stderr,"Error: Invalid token `%c` found.\n",c);
    abort();
}



void step_till_end_of_num(char** strp){
    char* str = *strp;
    while(isdigit(*str))
        str++;
    *strp = str;
}

int get_trailing_num(char* str){
    size_t end_of_num_pos = 0;
    while(isdigit(str[end_of_num_pos])){
        end_of_num_pos++;
    }
    char tmp = str[end_of_num_pos];
    str[end_of_num_pos] = '\0';
    if(end_of_num_pos == 0){
        /* the caller should check that the beginng of the string is a number */
        unreachable();
    }
    int res = atoi(str);
    str[end_of_num_pos] = tmp;
    return res;
}

/* We don't try to lex the string all at once,
   rather we to always get the next token
   and then cut of the beginning of the string 
   For example:
     x + 3*3 => Var('x') " + 3*3" => Var('x') Add "3*3" etc.
*/

int get_next_token(char** strp, Token* token){

   if(step_till_next_symbol(strp) == -1)
        return -1;

   char* str = *strp;

   if(is_single_char_token(*str)){
        switch(*str){
            case '+':
                token->type = Add;
                break;
            case '-':
                token->type = Sub;
                break;
            case '*':
                token->type = Mul;
                break;
            case '/':
                token->type = Div;
                break;
            case '^':
                token->type = Pow;
                break;
            case '(':
                token->type = Lparen;
                break;
            case ')':
                token->type = Rparen;
                break;
            default:
                /* should be only the above chars by the above if statement */
                unreachable();
        }
        str++;
   }
   else if(isalpha(*str)){
        token->type = Var;
        token->var = *str;
        str++;
   }
   else if(isdigit(*str)){
        token->type = Num;
        token->num = get_trailing_num(str);
        step_till_end_of_num(&str);
        
   }
   else {
    
        panic_invalid_token(*str);
   }
   *strp = str;
   return 0;
}

void print_token_to_stderr(Token t){
    switch(t.type){
        case Add:
            fprintf(stderr,"+");
            break;
        case Sub:
            fprintf(stderr,"-");
            break;
        case Mul:
            fprintf(stderr,"*");
            break;
        case Div:
            fprintf(stderr,"/");
            break;
        case Pow:
            fprintf(stderr,"^");
            break;
        case Lparen:
            fprintf(stderr,"(");
            break;
        case Rparen:
            fprintf(stderr,")");
            break;
        case Var:
            fprintf(stderr,"%c",t.var);
            break;
        case Num:
            fprintf(stderr,"%d",t.num);
            break;
        default:
            /* Unknown type */
            unreachable();
    }
    return;
}
