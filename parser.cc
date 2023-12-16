#include"lexer.h"
#include"parser.h"

static void expect(std::queue<Token>& tokens, Token front) {
    if(tokens.size() > 0 && tokens.front() == front) {
        tokens.pop();
        return;
    }

    std::cerr << "algr: expected: " << front << " but got: " << tokens.front() << " instead\n";
    exit(0);
}


Ast parse_product(std::queue<Token>& tokens);
Ast parse_primary(std::queue<Token>& tokens);

Ast parse(std::queue<Token>& tokens) {
    Ast lhs = parse_product(tokens);
    expect(tokens, {TokenType::Add});
    Ast rhs = parse(tokens);
    return {{TokenType::Add}, {lhs, rhs}};
}

Ast parse_product(std::queue<Token>& tokens) {
    Ast lhs = parse_primary(tokens);
    expect(tokens, {TokenType::Mul});
    Ast rhs = parse_product(tokens);
    return {{TokenType::Mul}, {lhs, rhs}};
}

Ast parse_primary(std::queue<Token>& tokens) {
    if(tokens.size() > 0 && tokens.front().type == TokenType::Num) {
        Token res = tokens.front();
        tokens.pop();
        return {res, {}};
    }

    expect(tokens, {TokenType::Lparen});
    Ast expr = parse(tokens);
    expect(tokens, {TokenType::Rparen});
    return expr;
}


std::ostream& operator<<(std::ostream& os, const Ast& ast) {
    os << ast.op << '{';
    for(auto node: ast.nodes) {
        os << node;
    }
    os << '}';
    return os;
}

bool operator==(const Ast& a, const Ast& b) {
    if(a.op !=  b.op) return false;
    if(a.nodes.size() != b.nodes.size()) return false;
    for(size_t i = 0; i < a.nodes.size(); ++i) {
        if(!(a.nodes[i] == b.nodes[i])) return false;
    }
    return true;

}



