/**
 * Chris Virtual Machine
 */

#ifndef __ChrisVM_h
#define __ChrisVM_h

#include <string>
#include <vector>

#include "../bytecode/OpCode.h"
#include <cstdint>

/**
 * Reads the current byte in the bytecode
 * and advances the ip pointer.
 */
#define READ_BYTE() *ip++

/**
 * Chris Virtual Machine
 */
class ChrisVM {
    public:
        ChrisVM() {}
    
        /**
        * Executes a program.
        */
        void exec(const std::string &program) {
            // 1. Parse the program
            // auto ast = parser->parse(program)

            // 2. Compile program to Chris bytecode
            // code = compiler->compile(ast);
            code = {OP_HALT};

            // Set instruction pointer to the beginning:
            ip = &code[0];

            return eval();
        }

        /** 
         * Main eval loop.
         */
        void eval() {
            for (;;) {
                switch (READ_BYTE()) {
                    case OP_HALT:
                        return;
                }
            }
        }

        /**
         * Instruction pointer (aka Program counter).
         */
        uint8_t* ip;

        /**
         * Bytecode.
         */
        std::vector<uint8_t> code;
};

#endif