#pragma once
#include "compiler.h"
#include <vector>
#include <unordered_map>
#include <string>

class VM {
public:
    VM();
    void run(Chunk& chunk);

private:
    size_t ip;
    std::vector<Value> stack; // <-- Stack now holds Values
    std::unordered_map<std::string, Value> globals; // <-- Variables now hold Values

    void push(Value value);
    Value pop();
};