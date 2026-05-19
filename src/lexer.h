#pragma once
#include <string>
#include <vector>
#include <unordered_map>

enum class TokenType {
    LET, IF, ELSE, WHILE, PRINT, INPUT,
    IDENTIFIER, NUMBER,
    PLUS, MINUS, STAR, SLASH,
    ASSIGN, EQUAL, LESSER, GREATER,
    LEFT_PAREN, RIGHT_PAREN,
    LEFT_BRACE, RIGHT_BRACE,
    SEMICOLON,
    EOF_TOKEN, STRING
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line;
};

class Lexer {
public:
    Lexer(const std::string& source);
    std::vector<Token> scanTokens();

private:
    std::string source;
    std::vector<Token> tokens;
    int start = 0;
    int current = 0;
    int line = 1;
    
    std::unordered_map<std::string, TokenType> keywords;

    bool isAtEnd();
    char advance();
    char peek();
    bool match(char expected);
    
    bool isDigit(char c);
    bool isAlpha(char c);
    bool isAlphaNumeric(char c);
    
    void scanToken();
    void addToken(TokenType type);
    void number();
    void identifier();
    void stringLiteral();
};