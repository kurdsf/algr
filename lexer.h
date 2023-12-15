#ifndef LEXER_H
#define LEXER_H

#include<queue>
#include<string>


enum class TokenType {
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


struct Token {
	TokenType type;
	long num; // only used if type == Num.
	char var; // only used if type == Var.
};



std::queue<Token> tokenize(const std::string& input);

std::ostream& operator>>(std::ostream& os, Token t);

#endif /* LEXER_H */
