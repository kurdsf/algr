#include<string.h>
#include<stdio.h>
#include<limits.h>

#include"vec.h"
#include"presedence.h"
#include"parser.h"
#include"optimizer.h"

#define  MAX_NUMBER_OF_CHARS_PER_LINE 40

int main(){
    
    printf("Welcome to the algr repl.\n");
    printf("Remember max chars per line is %d.\n",MAX_NUMBER_OF_CHARS_PER_LINE);
    printf("Also remember that the max int value is %d.\n",INT_MAX);
    AST* ast;
    while(1){
        /* we need the string to be stored on the heap so it is mutable
            and we can use realloc(...) on it */
        char* str = malloc(MAX_NUMBER_OF_CHARS_PER_LINE);
        if(str == NULL){
            perror("algr: repl");
            abort();
        }
        printf("\nalgr>");
        fgets(str,MAX_NUMBER_OF_CHARS_PER_LINE,stdin);
        
        if(str[0] == 'q' &&  str[1] == '\n') {
            free(str);
            break;
        }
        if(str[0] == '#'){
            free(str);
            continue;
        }
        ast = parse(str);
        ast = optimize(ast);
        putsAST(ast);
        freeAST(ast);
        free(str);
    }
    return 0;
}
 
