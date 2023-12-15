#include<assert.h>
#include<stdlib.h>
#include<stdio.h>

#include"vec.h"
#include"lexer.h"
#include"algr.h"
#include"presedence.h"

// The maximum presedence an operator can have without brackeys.
#define MAX_PRESEDENCE_WITHOUT_BRACKEYS 2

int get_op_presedence(Token);
int is_operator(Token);
void assert_every_operand_has_two_operators(Presedence_Tokens);




int get_op_presedence(Token t){
	switch(t.type){
		case Add:
		case Sub:
			return 0;
		case Mul:
		case Div:
			return 1;
        case Pow:
            return 2;
		default:
		// the caller should make sure that
		// t is an operator.
		// for example with is_operator(...);
        unreachable();
		
	}

}

int is_operator(Token t){
	switch(t.type){
		case Add:
		case Sub:
		case Mul:
		case Div:
        case Pow:
			return 1;
		default:
			return 0;
	}
	
}



_Noreturn void invalid_token(Token t){
	fprintf(stderr,"Error: Invalid token `");
    print_token_to_stderr(t);
    fprintf(stderr,"` in this context.\n");
    
    abort();
	
}


/* Assert that tokens (without brackeys) 
   are in the form of <operand><operator><operand>...<operator><operand>.
   So for example        3         +         4          -         2     .
   Notice that invalid expressions like "3+" or "* 3 2" don't have this pattern.
   TODO: Support unary '-'.

*/

void assert_every_operand_has_two_operators(Presedence_Tokens ptokens ){
		vec_Token tokens = ptokens.tokens;
        
		size_t len = vector_size(tokens);
		for(size_t i = 0; i < len; i++){
			Token curr_token = tokens[i];
			if(i % 2 == 0){
				if(curr_token.type != Num && curr_token.type != Var){
					invalid_token(curr_token);
				}
			}else {
				if(!is_operator(curr_token)){
					invalid_token(curr_token);
				}
			}

			
		}
		if(len % 2 == 0){
			fprintf(stderr,"Error: Invalid number of operators.\n");
            abort();
        }
		return;
		
}



Presedence_Tokens get_presedence_tokens(char* input){

    Presedence_Tokens res;
	int brackeys_depth = 0;

    vec_Token tokens = vector_create();
    /* This vector is just as long 
       as the tokens vector, even though
       no each token is an operator.
       We just store -1 instead.
       This allows us to find the presedence
       of token[i] at presedences[i].
     */
	vec_int presedences = vector_create();
    
    Token t;
    while(get_next_token(&input,&t) != -1){
		if(is_operator(t)){
      		// we have to make sure
			// that even if t has the lowest
			// presedence possible,
			// if it is in brackeys,
			// the presedence is higher
			// than that of all operators
			// that are not in brackeys;
            int op_presedence = get_op_presedence(t);
            op_presedence += (MAX_PRESEDENCE_WITHOUT_BRACKEYS + 1)*brackeys_depth;

            vector_add(&presedences,op_presedence);
            vector_add(&tokens,t);
		}
		else if (t.type == Lparen){

			brackeys_depth++;
		}
		else if (t.type == Rparen){
			brackeys_depth--;
		}
		else {
            vector_add(&presedences,-1);
            vector_add(&tokens,t); 
		}
        
		  
	}
    res.tokens = tokens;
    res.presedences = presedences;
    if(vector_size(tokens) == 0){
        fprintf(stderr,"Error: Illegal empty expression.\n");
        abort();
    }
    
    assert_every_operand_has_two_operators(res);
    if(brackeys_depth != 0){
        fprintf(stderr,"Error: Brackeys do not match.\n");
        abort();
    }
		
    return res;
}

void free_presedence_tokens(Presedence_Tokens toks){
    vector_free(toks.tokens);
    vector_free(toks.presedences);
    return;
}



