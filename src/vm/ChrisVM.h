/**
 * Chris Virtual Machine
 */

#ifndef ChrisVM_h
#define ChrisVM_h

#include <array>
#include <string>
#include <vector>

#include "../Logger.h"
#include "../bytecode/OpCode.h"
#include "../parser/ChrisParser.h"
#include "../compiler/ChrisCompiler.h"
#include "ChrisValue.h"

using syntax::ChrisParser;

/**
 * Reads the current byte in the bytecode
 * and advances the ip pointer.
 */
#define READ_BYTE() *ip++

/**
 * Gets a constant from the pool.
 */
#define GET_CONST() co->constants[READ_BYTE()]

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
 * Generic value comparison.
 */
#define COMPARE_VALUES(op, v1, v2)  \
    do {                            \
        bool res;                   \
        switch (op) {               \
            case 0:                 \
                res = v1 < v2;      \
                break;              \
            case 1:                 \
                res = v1 > v2;      \
                break;              \
            case 2:                 \
                res = v1 == v2;     \
                break;              \
            case 3:                 \
                res = v1 >= v2;     \
                break;              \
            case 4:                 \
                res = v1 <= v2;     \
                break;              \
            case 5:                 \
                res = v1 != v2;     \
                break;              \
        };                          \
        push(BOOLEAN(res));         \
    } while (false)

/**
 * Chris Virtual Machine
 */
class ChrisVM {
    public:
        ChrisVM() : parser(std::make_unique<ChrisParser>()),
                    compiler(std::make_unique<ChrisCompiler>()) {}

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
        ChrisValue exec(const std::string& program) {
            // 1. Parse the program
            auto ast = parser->parse(program);

            // 2. Compile program to Chris bytecode
            co = compiler->compile(ast);

            // Set instruction pointer to the beginning:
            ip = &co->code[0];

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
                        auto op2 = pop();
                        auto op1 = pop();

                        // Numeric addition:
                        if (IS_NUMBER(op1) && IS_NUMBER(op2)) {
                            auto v1 = AS_NUMBER(op1);
                            auto v2 = AS_NUMBER(op2);
                            push(NUMBER(v1 + v2));
                        }
                        
                        // String concatenation
                        else if (IS_STRING(op1) && IS_STRING(op2)) {
                            auto s1 = AS_CPPSTRING(op1);
                            auto s2 = AS_CPPSTRING(op2);
                            push(ALLOC_STRING(s1 + s2));
                        }
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

                    // Comparison
                    case OP_COMPARE:
                    {
                        auto op = READ_BYTE();

                        auto op2 = pop();
                        auto op1 = pop();

                        if (IS_NUMBER(op1) && IS_NUMBER(op2)) {
                            auto v1 = AS_NUMBER(op1);
                            auto v2 = AS_NUMBER(op2);
                            COMPARE_VALUES(op, v1, v2);
                        } else if (IS_STRING(op1) && IS_STRING(op2)) {
                            auto s1 = AS_STRING(op1);
                            auto s2 = AS_STRING(op2);
                            COMPARE_VALUES(op, s1, s2);
                        }
                        break;
                    }
                    
                    default:
                        DIE << "Unknown opcode: " << std::hex << opcode;
                }
            }
        }

        /**
         * Parser.
         */
        std::unique_ptr<ChrisParser> parser;

        /**
         * Compiler.
         */
        std::unique_ptr<ChrisCompiler> compiler;

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
         * Code object.
         */
        CodeObject* co;
};

#endif