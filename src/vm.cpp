#include "vm.h"
#include <iostream>

VM::VM() : ip(0) {}

void VM::push(Value value) { stack.push_back(value); }
Value VM::pop() { Value value = stack.back(); stack.pop_back(); return value; }

void VM::run(Chunk& chunk) {
    ip = 0; stack.clear();

    while (true) {
        uint8_t instruction = chunk.code[ip++];
        switch (instruction) {
            case OP_CONSTANT: { push(chunk.constants[chunk.code[ip++]]); break; }
            case OP_ADD: { 
                Value b = pop(); Value a = pop(); 
                if (a.type == ValType::NUMBER && b.type == ValType::NUMBER) push(Value(a.num + b.num)); 
                else if (a.type == ValType::STRING && b.type == ValType::STRING) push(Value(a.str + b.str)); 
                else { std::cout << "VM Error: Mismatched types.\n"; return; } break; 
            }
            case OP_SUB: { Value b = pop(); Value a = pop(); push(Value(a.num - b.num)); break; }
            case OP_MUL: { Value b = pop(); Value a = pop(); push(Value(a.num * b.num)); break; }
            case OP_DIV: { Value b = pop(); Value a = pop(); push(Value(a.num / b.num)); break; }
            case OP_EQUAL: { 
                Value b = pop(); Value a = pop(); 
                if (a.type == ValType::NUMBER && b.type == ValType::NUMBER) push(Value(a.num == b.num ? 1 : 0)); 
                else if (a.type == ValType::STRING && b.type == ValType::STRING) push(Value(a.str == b.str ? 1 : 0));
                else push(Value(0)); break; 
            }
            case OP_LESS: { Value b = pop(); Value a = pop(); push(Value(a.num < b.num ? 1 : 0)); break; }
            case OP_PRINT: {
                Value val = pop();
                if (val.type == ValType::NUMBER) std::cout << val.num << "\n"; else std::cout << val.str << "\n";
                break;
            }
            case OP_POP: pop(); break; 
            case OP_DEFINE_VAR: { globals[chunk.variables[chunk.code[ip++]]] = pop(); break; }
            case OP_GET_VAR: { push(globals[chunk.variables[chunk.code[ip++]]]); break; }
            
            // --- NEW LOGIC: Set Variable ---
            case OP_SET_VAR: {
                uint8_t nameIndex = chunk.code[ip++];
                std::string name = chunk.variables[nameIndex];
                globals[name] = stack.back(); // Overwrite the variable without popping
                break;
            }
            
            case OP_JUMP_IF_FALSE: {
                uint16_t offset = (chunk.code[ip] << 8) | chunk.code[ip + 1];
                ip += 2;
                Value condition = pop();
                bool isTruthy = (condition.type == ValType::NUMBER && condition.num != 0) || (condition.type == ValType::STRING && condition.str != "");
                if (!isTruthy) ip += offset; 
                break;
            }
            case OP_JUMP: {
                uint16_t offset = (chunk.code[ip] << 8) | chunk.code[ip + 1];
                ip += 2; ip += offset;
                break;
            }
            case OP_LOOP: {
                uint16_t offset = (chunk.code[ip] << 8) | chunk.code[ip + 1];
                ip += 2; ip -= offset; 
                break;
            }
            case OP_HALT: return;
            default: std::cout << "VM Error: Unknown opcode\n"; return;
        }
    }
}