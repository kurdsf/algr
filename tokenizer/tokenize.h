#pragma once

#include<stddef.h> // for size_t
#include<string.h> // for NULL,strlen(...)


#include"errors." // for algr_errno 
#include"token_utils.h" // for remove_whitespace(...)

// Public:
struct algr_Tokens algr_tokenize(char*);
void algr_free_tokens(struct algr_Tokens);

// Internal:
size_t algr_numtokens(char* input);
void algr_set_next_token_pos(char* input, int* i);
struct algr_Token algr_tokenize_next_word(char* input);



// Tokens are made up a type and a (optional) value


// Indicate the type of a token 
enum algr_TokenType {
        Plus, // "+"
        Minus, // "-" 
        Mul,  // etc
        Div,
        Pow,  
        Num, // 34,3,23 (only natural number (and 0) so  not 3.3 or -3) 
        Var, // one lowercase char like 'x', 'y' not 'X' or '?'.
};




// Indicate the value of the token
union algr_TokenValue {
        size_t Num,
        char Var,
        void* None // is equal to NULL
};

struct algr_Token {
        enum algr_TokenType type;
        union Value value;

};


struct algr_Tokens {
        struct algr_Token *tokenlist;
        size_t len;
};



struct algr_Tokens algr_tokenize(char* input){
        struct algr_Tokens res;
        // Note that the old input is not freed.
        input = remove_whitespace(input);

        size_t len = strlen(input);

        size_t numtokens = algr_numtokens(input);
        res.len = numtokens;

        struct algr_Token *tokenlist = (struct algr_Token*) malloc(res.len*sizeof(struct algr_Token));
        if(tokenlist == NULL){
                algr_errno = algr_MallocFail;
                algr_errno_linenum = -1;
                algr_errno_linepos = -1;
                return res;

        }
        res.tokenlist = 

        size_t i = 0;
        while (i != len-1){
                algr_set_next_token_pos(input+i,&i);
                struct algr_Token curr_token = algr_tokenize_next_word(input+i);
        }
        



}
