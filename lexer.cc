#include<iostream>
#include<map>
#include<string>
#include<stdexcept>
#include<queue>
#include"lexer.h"

static bool is_whitespace(char c) {
    return c == '\n' || c == ' ' || c == '\t';
}

std::queue<Token> tokenize(const std::string input) {
    std::queue<Token> res;
    std::map<char, Token> char2token;
    std::map<char, bool> is_single_char_token;
    char2token['+'] = {TokenType::Add};
    is_single_char_token['+'] = true;
    char2token['-'] = {TokenType::Sub};
    is_single_char_token['-'] = true;
    char2token['*'] = {TokenType::Mul};
    is_single_char_token['*'] = true;
    char2token['/'] = {TokenType::Div};
    is_single_char_token['/'] = true;
    char2token['^'] = {TokenType::Pow};
    is_single_char_token['^'] = true;
    char2token['('] = {TokenType::Lparen};
    is_single_char_token['('] = true;
    char2token[')'] = {TokenType::Rparen};
    is_single_char_token[')'] = true;
    for(char i = 'a'; i <= 'z'; ++i) char2token[i] = {.type = TokenType::Var, .var = i};
    for(char i = 'a'; i <= 'z'; ++i) is_single_char_token[i] = true;

    for(size_t curr = 0; curr < input.size(); ++curr) {
        Token empty;
        if(is_whitespace(input[curr])) continue;
        if(is_single_char_token[input[curr]]) {
            res.push(char2token[input[curr]]);
            continue;
        }

        // This can only be a number.
        size_t pos;
        try {
            long val = std::stol(input.substr(curr, input.size() - curr), &pos);
            res.push({.type = TokenType::Num, .num = val});
            curr += pos;
        } catch(std::invalid_argument const& ex) {
            std::cerr << "algr:" << curr << ": not an integer literal\n";
            exit(1);
        } catch(std::out_of_range const& ex) {
            std::cerr << "algr:" << curr << ": integer literal out of range\n";
            exit(1);
        }
    }
    return res;
}

std::ostream& operator>>(std::ostream& os, Token t){
    switch(t.type) {
        case TokenType::Add:
            os << '+';
            break;
        case TokenType::Sub:
            os << '-';
            break;
        case TokenType::Mul:
            os << '*';
            break;
        case TokenType::Div:
            os << '/';
            break;
        case TokenType::Pow:
            os << '^';
            break;
        case TokenType::Lparen:
            os << '(';
            break;
        case TokenType::Rparen:
            os << ')';
            break;
        case TokenType::Var:
            os << "variable " << t.var;
            break;
        case TokenType::Num:
            os << "number " << t.num;
            break;
    }

    return os;
}

