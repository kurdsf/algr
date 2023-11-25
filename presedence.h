#ifndef PRESEDENCE_H
#define PRESEDENCE_H

#include"lexer.h"
#include"vec.h"

typedef struct  {
	vec_Token tokens;
	vec_int presedences; // -1 if not an operator.

} Presedence_Tokens;


Presedence_Tokens get_presedence_tokens(char*);

void free_presedence_tokens(Presedence_Tokens);
#endif /* PRESEDENCE_H */


