#pragma once
#include "lexer.h"
#include <memory>
#include <vector>
#include <stdexcept>

struct Expr { virtual ~Expr() = default; };
struct LiteralExpr : public Expr { Token value; LiteralExpr(Token value) : value(value) {} };
struct VariableExpr : public Expr { Token name; VariableExpr(Token name) : name(name) {} };

// --- NEW: Assignment Expression Node ---
struct AssignExpr : public Expr {
    Token name; std::unique_ptr<Expr> value;
    AssignExpr(Token name, std::unique_ptr<Expr> value) : name(name), value(std::move(value)) {}
};

struct BinaryExpr : public Expr {
    std::unique_ptr<Expr> left; Token op; std::unique_ptr<Expr> right;
    BinaryExpr(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right) : left(std::move(left)), op(op), right(std::move(right)) {}
};

struct Stmt { virtual ~Stmt() = default; };
struct PrintStmt : public Stmt {
    std::unique_ptr<Expr> expression;
    PrintStmt(std::unique_ptr<Expr> expression) : expression(std::move(expression)) {}
};
struct VarDeclStmt : public Stmt {
    Token name; std::unique_ptr<Expr> initializer;
    VarDeclStmt(Token name, std::unique_ptr<Expr> initializer) : name(name), initializer(std::move(initializer)) {}
};
struct ExpressionStmt : public Stmt {
    std::unique_ptr<Expr> expression;
    ExpressionStmt(std::unique_ptr<Expr> expression) : expression(std::move(expression)) {}
};

struct BlockStmt : public Stmt {
    std::vector<std::unique_ptr<Stmt>> statements;
    BlockStmt(std::vector<std::unique_ptr<Stmt>> statements) : statements(std::move(statements)) {}
};
struct IfStmt : public Stmt {
    std::unique_ptr<Expr> condition; std::unique_ptr<Stmt> thenBranch; std::unique_ptr<Stmt> elseBranch;
    IfStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> thenBranch, std::unique_ptr<Stmt> elseBranch)
        : condition(std::move(condition)), thenBranch(std::move(thenBranch)), elseBranch(std::move(elseBranch)) {}
};
struct WhileStmt : public Stmt {
    std::unique_ptr<Expr> condition; std::unique_ptr<Stmt> body;
    WhileStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body) : condition(std::move(condition)), body(std::move(body)) {}
};

class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    std::vector<std::unique_ptr<Stmt>> parse();

private:
    std::vector<Token> tokens; int current = 0;

    std::unique_ptr<Stmt> declaration();
    std::unique_ptr<Stmt> varDeclaration();
    std::unique_ptr<Stmt> statement();
    std::unique_ptr<Stmt> printStatement();
    std::unique_ptr<Stmt> expressionStatement();
    
    std::vector<std::unique_ptr<Stmt>> block();
    std::unique_ptr<Stmt> ifStatement();
    std::unique_ptr<Stmt> whileStatement();

    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> assignment(); // <-- NEW
    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> primary();

    bool match(std::initializer_list<TokenType> types);
    bool check(TokenType type);
    bool isAtEnd();
    Token advance(); Token peek(); Token previous();
    Token consume(TokenType type, const std::string& message);
};