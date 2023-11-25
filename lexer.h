#ifndef LEXER_H
#define LEXER_H



enum TokenType {
	Add,
	Sub,
	Mul,
	Div,
    Pow,
	Lparen,
	Rparen,
	Var,
    Num
};



typedef struct {
	enum TokenType type;
	union {
		int num;
		char var;
        };
} Token;


typedef Token* vec_Token;



int is_left_assoc(Token);

/* Returns -1 if there is not next token */
int get_next_token(char**, Token*);

void print_token_to_stderr(Token);

#endif /* LEXER_H */
