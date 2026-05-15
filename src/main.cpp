#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "lexer.h" 
#include "parser.h" 
#include "compiler.h"
#include "vm.h"

void executeCode(const std::string& source, VM& vm, bool debugMode = false) {
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    
    try {
        Parser parser(tokens);
        std::vector<std::unique_ptr<Stmt>> ast = parser.parse();
        
        if (debugMode) {
            std::cout << "[DEBUG] AST Generated: " << ast.size() << " top-level statements.\n";
        }
        
        Compiler compiler;
        Chunk chunk = compiler.compile(ast);
        
        if (debugMode) {
            chunk.disassemble("Compiled Bytecode");
        }
        
        vm.run(chunk);
    } catch (const std::runtime_error& error) {
        std::cerr << "Execution Error: " << error.what() << "\n";
    }
}

void runFile(const std::string& path, VM& vm) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << path << "'\n";
        return;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    executeCode(buffer.str(), vm);
}

void runREPL(VM& vm) {
    std::string line;
    std::cout << "CVM++ REPL [Type 'exit' to quit]\n";
    while (true) {
        std::cout << "cvm++ > ";
        if (!std::getline(std::cin, line) || line == "exit") break;
        if (line.empty()) continue;
        executeCode(line, vm);
    }
}

int main(int argc, char* argv[]) {
    VM vm;
    
    if (argc == 2) {
        // Run from file if passed as an argument: ./cvm script.cvm
        runFile(argv[1], vm);
    } else if (argc == 1) {
        // Run interactive prompt
        runREPL(vm);
    } else {
        std::cerr << "Usage: cvm [path/to/script.cvm]\n";
        return 1;
    }
    
    return 0;
}