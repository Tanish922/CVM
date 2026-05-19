# CVM++ Virtual Machine & Language Compiler


## OVERVIEW:
CVM++ is a custom, dynamically-typed scripting language and stack-based virtual machine written in C++. It features a complete frontend pipeline (Lexer -> Parser -> Compiler) and a backend execution engine (VM).

---
## PROJECT STRUCTURE:

```
CVM/
├── CMakeLists.txt        # Build system configuration
├── demo.cvm              # Sample script: Factorial Calculator
├── test1.cvm              # Sample script: General loop and if/else tests
├── test2.cvm              # Sample script: Arithmetic and strings tests
├── readme.txt            # Project documentation and instructions
├── build/                # Generated folder for compiled object files and the .exe
└── src/                  # Core C++ Source Code
    ├── main.cpp          # Entry point; handles REPL and File Execution modes
    ├── lexer.h/.cpp      # Lexical Analysis (Text -> Tokens)
    ├── parser.h/.cpp     # Recursive Descent Parser (Tokens -> AST)
    ├── compiler.h/.cpp   # Bytecode Generation (AST -> Opcodes/Reverse Polish Notation)
    └── vm.h/.cpp         # Virtual Machine (Executes Opcodes, manages Stack & Memory)
```
---

## PREREQUISITES:
- C++17 Compatible Compiler (MinGW GCC recommended)
- Make (mingw32-make)

## HOW TO BUILD:

1. Open your terminal and navigate to the root directory of the project.
2. Create and enter the build directory (if not already created):
```
   > mkdir build
   > cd build
```
3. Compile the project using Make:
```
   > mingw32-make
```

## HOW TO RUN:
The engine supports two modes of execution:

1. Interactive REPL Mode:
   Run the executable without any arguments to type code line-by-line.
   ```
   > cvm.exe
   (Type 'exit' to quit)
   ```

2. File Execution Mode:
   Pass a .cvm script file as an argument to execute the whole program.
   ```
   > cvm.exe ../demo.cvm
   ```

## NOTE ON DEBUGGING:
The system is configured to print the disassembled bytecode (Reverse Polish Notation) before executing the logic, demonstrating the successful compilation of the AST into machine instructions.