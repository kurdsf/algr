
/* 
 The main entrypoint (the only user exposed function algr_solutions) 
 of the solver system.
*/ 
#pragma once

#include<string.h> // for NULL
#include<stddef.h> // for size_t

#include"errors/errors.h" // for algr_errno 
#include"tokenizer/tokenize.h" // for algr_tokenize, algr_free_tokens
#include"tokenizer/check_tokens.h" // for algr_check
#include"solver/solver.h" // for __algr_solutions


// Returns NULL incase of failure an set algr_errno.
float* algr_solutions(char* lhs, char* rhs, size_t* numsolutions){

        // Replace string by list of tokens
        // For example 3 + 4 => 'Num(3) Plus Num(4)'.
        struct algr_Tokens tokens_lhs  = algr_tokenize(lhs);
        if (algr_errno != algr_NoError){
                algr_free_tokens(tokens_lhs);
                return NULL;
        }
        struct algr_Tokens tokens_rhs  = algr_tokenize(rhs);
        if (algr_errno != algr_NoError){
                algr_free_tokens(tokens_lhs);
                algr_free_tokens(tokens_rhs);
                return NULL;
        }

        // Check list of tokens for logical errors
        // For example 'Plus Plus' Or 'Plus Num(3)'
        algr_check_tokens(tokens_lhs);
        if (algr_errno != algr_NoError){
                algr_free_tokens(tokens_lhs);
                algr_free_tokens(tokens_rhs);
                return NULL;
        }
        algr_check_tokens(tokens_rhs);
        if(algr_errno !=  algr_NoError){
                algr_free_tokens(tokens_lhs);
                algr_free_tokens(tokens_rhs);
                return NULL;
        }

         
        // Find solutions 
        float* solutions = __algr_solutions(tokens_lhs,tokens_rhs,numsolutions);
        // These aren't used anymore in any case (so also incase of error).
        // So error checking is delayed ...
        algr_free_tokens(tokens_lhs);
        algr_free_tokens(tokens_rhs);
        // ... to here
        
        if(algr_errno != algr_NoError){
                // Note that __algr_solutions(...) should also return NULL
                // if algr_errno != algr_NoError so this is redundant and 
                // only used for readability.
                return NULL;
        }

        return solutions;
} 


