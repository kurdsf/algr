#include<iostream>
#include<map>
#include<string>
#include<stdexcept>
#include<queue>
#include"lexer.h"

static bool is_whitespace(char c) {
    return c == '\n' || c == ' ' || c == '\t';
}

std::queue<Token> tokenize(const std::string& input) {
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
        if(is_whitespace(input[curr])) continue;
        if(is_single_char_token[input[curr]]) {
            Token t = char2token[input[curr]];
            t.pos = curr;
            res.push(t);
            continue;
        }

        // This can only be a number.
        size_t pos;
        try {
            long val = std::stol(input.substr(curr, input.size() - curr), &pos);
            res.push({.type = TokenType::Num, .num = val, .pos = curr});
            curr += pos - 1;
        } catch(std::invalid_argument const& ex) {
            for(size_t i = 0; i < curr; ++i) std::clog << ' ';
            std::clog << "^\n";
            std::cerr << "algr: invalid character\n";
            exit(1);
        } catch(std::out_of_range const& ex) {
            for(size_t i = 0; i < curr; ++i) std::clog << ' ';
            std::clog << "^\n";
            std::clog << "algr: integer literal out of range\n";
            exit(1);
        }
    }
    return res;
}

std::ostream& operator<<(std::ostream& os, Token t){
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

bool operator==(Token a, Token b) {
    if(a.type != b.type) return false;
    if(a.type == TokenType::Var && a.var != b.var) return false;
    if(a.type == TokenType::Num && a.num != b.num) return false;
    return true;
}

bool operator!=(Token a, Token b) {
    return !(a == b);
}

