/**
 * Chris Virtual Machine
 */

#ifndef ChrisVM_h
#define ChrisVM_h

#include <array>
#include <string>
#include <vector>
#include <cstdint>

#include "../Logger.h"
#include "../bytecode/OpCode.h"
#include "ChrisValue.h"

/**
 * Reads the current byte in the bytecode
 * and advances the ip pointer.
 */
#define READ_BYTE() *ip++

/**
 * Gets a constant from the pool.
 */
#define GET_CONST() constants[READ_BYTE()]

/**
 * Stack top (stack overflow after exceeding).
 */
#define STACK_LIMIT 512

/**
 * Binary operation.
 */
#define BINARY_OP(op)                \
    do {                             \
        auto op2 = AS_NUMBER(pop()); \
        auto op1 = AS_NUMBER(pop()); \
        push(NUMBER(op1 op op2));    \
    } while (false)

/**
 * Chris Virtual Machine
 */
class ChrisVM {
    public:
        ChrisVM() {}

        /**
         * Pushes a value onto the stack.
         */
        void push(const ChrisValue& value) {
            if ((size_t)(sp - stack.begin()) == STACK_LIMIT) {
                DIE << "push(): Stack overflow.\n";
            }
            *sp = value;
            sp++;
        }

        /**
         * Pops a value from the stack.
         */
        ChrisValue pop() {
            if (sp == stack.begin()) {
                DIE << "pop(): empty stack.\n";
            }
            --sp;
            return *sp;
        }
    
        /**
        * Executes a program.
        */
        ChrisValue exec(const std::string &program) {
            // 1. Parse the program
            // auto ast = parser->parse(program)

            // 2. Compile program to Chris bytecode
            // code = compiler->compile(ast);

            constants.push_back(NUMBER(10));
            constants.push_back(NUMBER(3));
            constants.push_back(NUMBER(10));

            // (- (* 10 3) 10)
            code = {
                OP_CONST,
                0,
                OP_CONST,
                1,
                OP_MUL,
                OP_CONST,
                2,
                OP_SUB,
                OP_HALT
            };

            // Set instruction pointer to the beginning:
            ip = &code[0];

            // Init the stack:
            sp = &stack[0];

            return eval();
        }

        /** 
         * Main eval loop.
         */
        ChrisValue eval() {
            for (;;) {
                auto opcode = READ_BYTE();
                //log(opcode);
                switch (opcode) {
                    case OP_HALT:
                        return pop();

                    // ---------------------
                    // Constants:
                    case OP_CONST:
                        push(GET_CONST());
                        break;

                    // ---------------------
                    // Math ops:
                    case OP_ADD:
                    {
                        BINARY_OP(+);
                        break;
                    }

                    case OP_SUB:
                    {
                        BINARY_OP(-);
                        break;
                    }

                    case OP_MUL:
                    {
                        BINARY_OP(*);
                        break;
                    }

                    case OP_DIV:
                    {
                        BINARY_OP(/);
                        break;
                    }
                    
                    default:
                        DIE << "Unknown opcode: " << std::hex << opcode;
                }
            }
        }

        /**
         * Instruction pointer (aka Program counter).
         */
        uint8_t* ip;

        /**
         * Stack pointer.
         */
        ChrisValue* sp;

        /**
         * Operands stack.
         */
        std::array<ChrisValue, STACK_LIMIT> stack;

        /**
         * Constant pool.
         */
        std::vector<ChrisValue> constants;

        /**
         * Bytecode.
         */
        std::vector<uint8_t> code;
};

#endif