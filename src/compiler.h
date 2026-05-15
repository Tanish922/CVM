#pragma once
#include "parser.h"
#include <vector>
#include <cstdint>
#include <string>
#include <iostream>

enum class ValType { NUMBER, STRING };
struct Value {
    ValType type; int num; std::string str;
    Value(int n) : type(ValType::NUMBER), num(n), str("") {}
    Value(std::string s) : type(ValType::STRING), num(0), str(s) {}
    Value() : type(ValType::NUMBER), num(0), str("") {} 
};

enum OpCode : uint8_t {
    OP_CONSTANT = 0, OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_EQUAL, OP_LESS,
    OP_PRINT, OP_DEFINE_VAR, OP_GET_VAR, 
    OP_SET_VAR, // <-- NEW
    OP_POP, OP_JUMP_IF_FALSE, OP_JUMP, OP_LOOP,
    OP_HALT
};

struct Chunk {
    std::vector<uint8_t> code;
    std::vector<Value> constants;
    std::vector<std::string> variables;
    
    void write(uint8_t byte) { code.push_back(byte); }
    uint8_t addConstant(Value value) { constants.push_back(value); return static_cast<uint8_t>(constants.size() - 1); }
    uint8_t addVariable(const std::string& name) { variables.push_back(name); return static_cast<uint8_t>(variables.size() - 1); }

    void disassemble(const std::string& name) {
        std::cout << "== " << name << " ==\n";
        for (size_t offset = 0; offset < code.size();) {
            std::cout << offset << "  ";
            uint8_t instruction = code[offset];
            switch (instruction) {
                case OP_CONSTANT: {
                    Value val = constants[code[offset + 1]];
                    std::cout << "OP_CONSTANT " << (int)code[offset + 1] << " (val: ";
                    if (val.type == ValType::NUMBER) std::cout << val.num; else std::cout << "\"" << val.str << "\"";
                    std::cout << ")\n";
                    offset += 2; break;
                }
                case OP_ADD: std::cout << "OP_ADD\n"; offset++; break;
                case OP_SUB: std::cout << "OP_SUB\n"; offset++; break;
                case OP_MUL: std::cout << "OP_MUL\n"; offset++; break;
                case OP_DIV: std::cout << "OP_DIV\n"; offset++; break;
                case OP_PRINT: std::cout << "OP_PRINT\n"; offset++; break;
                case OP_POP: std::cout << "OP_POP\n"; offset++; break;
                case OP_DEFINE_VAR: std::cout << "OP_DEFINE_VAR " << (int)code[offset + 1] << " (name: " << variables[code[offset + 1]] << ")\n"; offset += 2; break;
                case OP_GET_VAR: std::cout << "OP_GET_VAR " << (int)code[offset + 1] << " (name: " << variables[code[offset + 1]] << ")\n"; offset += 2; break;
                case OP_SET_VAR: std::cout << "OP_SET_VAR " << (int)code[offset + 1] << " (name: " << variables[code[offset + 1]] << ")\n"; offset += 2; break;
                case OP_JUMP:
                case OP_JUMP_IF_FALSE: {
                    uint16_t jump = (code[offset + 1] << 8) | code[offset + 2];
                    std::cout << (instruction == OP_JUMP ? "OP_JUMP " : "OP_JUMP_IF_FALSE ") << jump << " \n";
                    offset += 3; break;
                }
                case OP_LOOP: {
                    uint16_t jump = (code[offset + 1] << 8) | code[offset + 2];
                    std::cout << "OP_LOOP -" << jump << "\n";
                    offset += 3; break;
                }
                case OP_HALT: std::cout << "OP_HALT\n"; offset++; break;
                default: std::cout << "Unknown opcode " << (int)instruction << "\n"; offset++; break;
            }
        }
        std::cout << "\n";
    }
};

class Compiler {
public:
    Compiler();
    Chunk compile(const std::vector<std::unique_ptr<Stmt>>& ast);

private:
    Chunk chunk;
    void compileStatement(const Stmt* stmt);
    void compileExpression(const Expr* expr);
    void emitByte(uint8_t byte);
    
    int emitJump(uint8_t instruction);
    void patchJump(int offset);
    void emitLoop(int loopStart);
};