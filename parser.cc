#include"lexer.h"
#include"parser.h"

static void expect(std::queue<Token>& tokens, Token front) {
    if(tokens.size() > 0 && tokens.front() == front) {
        tokens.pop();
        return;
    }

    if(tokens.size() > 0) {
        for(size_t i = 0; i < tokens.front().pos; ++i) std::clog << ' ';
        std::clog << "^\n";
        std::clog << "algr: expected " << front << " but got " << tokens.front() << " instead\n";
    } else {
        std::clog << "algr: expected " << front << " but got end of input instead\n";
    }
    exit(1);
}


static Ast parse_sum(std::queue<Token>& tokens);
static Ast parse_product(std::queue<Token>& tokens);
static Ast parse_primary(std::queue<Token>& tokens);


Ast parse(std::queue<Token>& tokens) {
    Ast res = parse_sum(tokens);
    if(tokens.size() != 0) {
        for(size_t i = 0; i < tokens.front().pos; ++i) std::clog << ' ';
        std::clog << "^\n";
        std::clog << "algr: expected end of input but got " << tokens.front() << " instead\n";
        exit(1);
    }
    return res;

}

static Ast parse_sum(std::queue<Token>& tokens) {
    Ast lhs = parse_product(tokens);
    if(tokens.size() > 0 
        && tokens.front().type == TokenType::Add || tokens.front().type == TokenType::Sub) {
        Token op = tokens.front();
        tokens.pop();
        Ast rhs = parse_sum(tokens);
        return {op, {lhs, rhs}};

    }
    return lhs;
}

static Ast parse_product(std::queue<Token>& tokens) {
    Ast lhs = parse_primary(tokens);
    if(tokens.size() > 0 
        && tokens.front().type == TokenType::Mul || tokens.front().type == TokenType::Div) {
        Token op = tokens.front();
        tokens.pop();
        Ast rhs = parse_product(tokens);
        return {op, {lhs, rhs}};

    }
    return lhs;
}



static Ast parse_primary(std::queue<Token>& tokens) {
    if(tokens.size() > 0 && tokens.front().type == TokenType::Num || tokens.front().type == TokenType::Var) {
        Token res = tokens.front();
        tokens.pop();
        return {res, {}};
    }

    expect(tokens, {TokenType::Lparen});
    Ast expr = parse_sum(tokens);
    expect(tokens, {TokenType::Rparen});
    return expr;
}


// Print the AST in lisp-style
std::ostream& operator<<(std::ostream& os, const Ast& ast) {
    // Don't print brackeys when printing an atom (num or var).
    if(ast.nodes.size() == 0) {
        os << ast.op;
        return os;
    }
    os << '(' << ast.op << ' ';
    for(const auto& node: ast.nodes) {
        os << node << ' ';
    }
    os << ')';
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



