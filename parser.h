#pragma once
#include<iostream>
#include<vector>
#include<unordered_map>
#include<queue>
#include"lexer.h"

struct Ast {
    Token op;
    std::vector<Ast> nodes; 
};

Ast parse(std::queue<Token>& tokens);

std::ostream& operator<<(std::ostream& os, const Ast& ast);
bool operator==(const Ast& a, const Ast& b);



