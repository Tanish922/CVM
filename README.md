# CVM++ Virtual Machine

A dynamically typed, stack-based virtual machine and scripting language built entirely from scratch in C++. 

## Features
* **Custom Lexer & Parser:** Generates an Abstract Syntax Tree (AST) from raw text.
* **Bytecode Compiler:** Flattens the AST into Reverse Polish Notation opcodes.
* **Stack-Based VM:** Executes opcodes with a simulated instruction pointer.
* **Dynamic Typing:** Supports both integers and strings using a custom Tagged Union `Value` struct.
* **Turing Complete:** Full support for `if/else` control flow and `while` loops via bytecode jump patching.

## How to Run
```bash
mkdir build
cd build
mingw32-make
./cvm.exe ../test.cvm