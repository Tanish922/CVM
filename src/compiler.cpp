#include "compiler.h"
#include <iostream>

Compiler::Compiler() {}
void Compiler::emitByte(uint8_t byte) { chunk.write(byte); }

int Compiler::emitJump(uint8_t instruction) {
    emitByte(instruction); emitByte(0xff); emitByte(0xff);
    return chunk.code.size() - 2;
}

void Compiler::patchJump(int offset) {
    int jump = chunk.code.size() - offset - 2;
    chunk.code[offset] = (jump >> 8) & 0xff;
    chunk.code[offset + 1] = jump & 0xff;
}

void Compiler::emitLoop(int loopStart) {
    emitByte(OP_LOOP);
    int jump = chunk.code.size() - loopStart + 2;
    emitByte((jump >> 8) & 0xff); emitByte(jump & 0xff);
}

Chunk Compiler::compile(const std::vector<std::unique_ptr<Stmt>>& ast) {
    for (const auto& stmt : ast) { compileStatement(stmt.get()); }
    emitByte(OP_HALT);
    return chunk;
}

void Compiler::compileStatement(const Stmt* stmt) {
    if (auto printStmt = dynamic_cast<const PrintStmt*>(stmt)) {
        compileExpression(printStmt->expression.get());
        emitByte(OP_PRINT);
    }
    else if (auto varDecl = dynamic_cast<const VarDeclStmt*>(stmt)) {
        if (varDecl->initializer) compileExpression(varDecl->initializer.get());
        else { uint8_t idx = chunk.addConstant(Value(0)); emitByte(OP_CONSTANT); emitByte(idx); }
        uint8_t nameIdx = chunk.addVariable(varDecl->name.lexeme);
        emitByte(OP_DEFINE_VAR);
        emitByte(nameIdx);
    }
    else if (auto blockStmt = dynamic_cast<const BlockStmt*>(stmt)) {
        for (const auto& s : blockStmt->statements) compileStatement(s.get());
    }
    else if (auto ifStmt = dynamic_cast<const IfStmt*>(stmt)) {
        compileExpression(ifStmt->condition.get());
        int thenJump = emitJump(OP_JUMP_IF_FALSE); 
        compileStatement(ifStmt->thenBranch.get());
        int elseJump = emitJump(OP_JUMP);          
        patchJump(thenJump);                       
        if (ifStmt->elseBranch) compileStatement(ifStmt->elseBranch.get());
        patchJump(elseJump);                       
    }
    else if (auto whileStmt = dynamic_cast<const WhileStmt*>(stmt)) {
        int loopStart = chunk.code.size();
        compileExpression(whileStmt->condition.get());
        int exitJump = emitJump(OP_JUMP_IF_FALSE);
        compileStatement(whileStmt->body.get());
        emitLoop(loopStart);
        patchJump(exitJump);
    }
    else if (auto exprStmt = dynamic_cast<const ExpressionStmt*>(stmt)) {
        compileExpression(exprStmt->expression.get());
        emitByte(OP_POP);
    }
}

void Compiler::compileExpression(const Expr* expr) {
    if (auto lit = dynamic_cast<const LiteralExpr*>(expr)) {
        uint8_t idx = (lit->value.type == TokenType::STRING) 
            ? chunk.addConstant(Value(lit->value.lexeme)) 
            : chunk.addConstant(Value(std::stoi(lit->value.lexeme)));
        emitByte(OP_CONSTANT); emitByte(idx);
    }
    else if (auto var = dynamic_cast<const VariableExpr*>(expr)) {
        uint8_t nameIdx = chunk.addVariable(var->name.lexeme);
        emitByte(OP_GET_VAR); emitByte(nameIdx);
    }
    // --- NEW LOGIC: Compile Assignment ---
    else if (auto assign = dynamic_cast<const AssignExpr*>(expr)) {
        compileExpression(assign->value.get()); // Compute right side
        uint8_t nameIdx = chunk.addVariable(assign->name.lexeme);
        emitByte(OP_SET_VAR);
        emitByte(nameIdx);
    }
    else if (auto bin = dynamic_cast<const BinaryExpr*>(expr)) {
        compileExpression(bin->left.get());
        compileExpression(bin->right.get());
        switch (bin->op.type) {
            case TokenType::PLUS:  emitByte(OP_ADD); break;
            case TokenType::MINUS: emitByte(OP_SUB); break;
            case TokenType::STAR:  emitByte(OP_MUL); break;
            case TokenType::SLASH: emitByte(OP_DIV); break;
            case TokenType::EQUAL: emitByte(OP_EQUAL); break;
            case TokenType::LESS:  emitByte(OP_LESS); break;
            default: break;
        }
    }
}