#include<iostream>
#include<string>

#include"lexer.h"
#include"parser.h"


int main(){
    std::string line;
    std::getline(std::cin, line);
    std::queue<Token> tokens = tokenize(line);
    Ast ast = parse(tokens);
    std::cout << ast << '\n';
}

 
