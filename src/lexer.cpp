#include "lexer.h"
#include <iostream>

Lexer::Lexer(const std::string& source) : source(source) {
    keywords = {
        {"let", TokenType::LET}, {"if", TokenType::IF}, {"else", TokenType::ELSE}, // <-- Added ELSE
        {"while", TokenType::WHILE}, {"print", TokenType::PRINT}, {"input", TokenType::INPUT}
    };
}

std::vector<Token> Lexer::scanTokens() {
    while (!isAtEnd()) { start = current; scanToken(); }
    tokens.push_back({TokenType::EOF_TOKEN, "", line});
    return tokens;
}

bool Lexer::isAtEnd() { return current >= source.length(); }
char Lexer::advance() { return source[current++]; }
char Lexer::peek() { return isAtEnd() ? '\0' : source[current]; }
bool Lexer::match(char expected) {
    if (isAtEnd() || source[current] != expected) return false;
    current++; return true;
}

void Lexer::addToken(TokenType type) {
    std::string text = source.substr(start, current - start);
    tokens.push_back({type, text, line});
}

bool Lexer::isDigit(char c) { return c >= '0' && c <= '9'; }
bool Lexer::isAlpha(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }
bool Lexer::isAlphaNumeric(char c) { return isAlpha(c) || isDigit(c); }

void Lexer::number() {
    while (isDigit(peek())) advance();
    addToken(TokenType::NUMBER);
}

void Lexer::identifier() {
    while (isAlphaNumeric(peek())) advance();
    std::string text = source.substr(start, current - start);
    TokenType type = TokenType::IDENTIFIER;
    if (keywords.count(text)) type = keywords[text];
    addToken(type);
}

void Lexer::stringLiteral() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') line++;
        advance();
    }
    if (isAtEnd()) { std::cout << "Lexical Error: Unterminated string.\n"; return; }
    advance();
    std::string value = source.substr(start + 1, current - start - 2);
    tokens.push_back({TokenType::STRING, value, line});
}

void Lexer::scanToken() {
    char c = advance();
    switch (c) {
        case '(': addToken(TokenType::LEFT_PAREN); break;
        case ')': addToken(TokenType::RIGHT_PAREN); break;
        case '{': addToken(TokenType::LEFT_BRACE); break;
        case '}': addToken(TokenType::RIGHT_BRACE); break;
        case '+': addToken(TokenType::PLUS); break;
        case '-': addToken(TokenType::MINUS); break;
        case '*': addToken(TokenType::STAR); break;
        case '/': addToken(TokenType::SLASH); break;
        case ';': addToken(TokenType::SEMICOLON); break;
        case '<': addToken(TokenType::LESSER); break;
        case '>': addToken(TokenType::GREATER); break;
        case '=': addToken(match('=') ? TokenType::EQUAL : TokenType::ASSIGN); break;
        case '"': stringLiteral(); break;
        case ' ': case '\r': case '\t': break;
        case '\n': line++; break;
        default:
            if (isDigit(c)) number();
            else if (isAlpha(c)) identifier();
            else std::cout << "Lexical Error line " << line << ": Unexpected '" << c << "'\n";
            break;
    }
}