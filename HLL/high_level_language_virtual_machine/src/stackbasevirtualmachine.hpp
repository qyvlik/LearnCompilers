#ifndef STACKBASEVIRTUALMACHINE
#define STACKBASEVIRTUALMACHINE

#include <iostream>
#include <stdint.h>
#include <assert.h>
#include <stack>
#include <vector>
#include <tuple>
#include <map>

namespace qyvlik {
namespace virtual_machine {

/*
  - ADD: integer addition
  - DIV: integer division
  - MUL: integer multiplication
  - SUB: integer subtraction
  - INC: incrémentation
  - DEC: décrémentation
  - PUSH: push value on the stack
  - POP: pop value from stack
  - CMP: comparator
  - JE: jump if équal
  - JNE: jump if not équal
  - JMP: jump to instruction
  - PRINT: print
  - READ: read
  - POS: set cursor pos
  - DUP: duplicate the element at the top of the stack
  - SWAP: exchange elements at the top of the stack
  - END: exit the program
  - NOP: nop
*/

class StackBaseVirtualMachine
{
public:
    // for int32
    enum Operation {
        Add,
        Sub,
        Mul,
        Div,
        Mod,

        Equal,
        NotEqual,

        Push,
        Pop,
        PushTop,
        SwapTop,

        JumpIfNotZero,

        JumpIfZero,

        Jump,

        Print,

        Label,              // label 0 = Lable + 0; Lable n = lable + n;

        Stop,               //
    };

    void add() {
        int v0 = pop();
        int v1 = pop();
        push(v0 + v1);
    }

    void sub() {
        int v0 = pop();
        int v1 = pop();
        push(v1 - v0);
    }

    void mul() {
        int v0 = pop();
        int v1 = pop();
        push(v0 * v1);
    }

    void div() {
        int v0 = pop();
        int v1 = pop();
        assert(v1 != 0);
        push(v0 / v1);
    }

    void mod() {
        int v0 = pop();
        int v1 = pop();
        assert(v1 != 0);
        push(v0 % v1);
    }

    void equal() {
        int v0 = pop();
        int v1 = pop();
        push(v0 == v1);
    }

    void notEqual() {
        int v0 = pop();
        int v1 = pop();
        push(v0 != v1);
    }

    void push(int value) {
        evaluation_stack.push(value);
    }

    int pop() {
        if(!evaluation_stack.empty()) {
            int v = evaluation_stack.top();
            evaluation_stack.pop();
            return v;
        } else {
            assert(false);
            return -1;
        }
    }

    void pushTop() {
        if(!evaluation_stack.empty()) {
            evaluation_stack.push(evaluation_stack.top());
        } else {
            std::cout << "stack is empty" << std::endl;
        }
    }

    void swapTop(int value) {
        if(!evaluation_stack.empty()) {
            evaluation_stack.top() = value;
        } else {
            std::cout << "stack is empty" << std::endl;
        }
    }

    // -1
    int jumpIfNotZero(int label) {
        if(evaluation_stack.top() != 0) {

            pop();

            return label;
        } else {
            pop();

            return -1;
        }
    }

    // -1
    int jumpIfZero(int label) {
        if(evaluation_stack.top() == 0) {

            pop();

            return label;
        } else {
            pop();

            return -1;
        }
    }

    void printTop() {
        std::cout << "top: " << evaluation_stack.top() << std::endl;
    }

    void execute(const std::vector<std::tuple<Operation, int>>& codes)
    {
        size_t pc = 0;
        size_t size = codes.size();

        int op = -1;
        int value = -1;
        int jump_lable = -1;

        std::map<int, int> label_code_to_pc = LabelToProgramCount(codes);

        while(pc < size) {

            auto& inst = codes.at(pc);
            op = std::get<0>(inst);
            value = std::get<1>(inst);
            switch(op)
            {
            case Add           :
                add();
                break;

            case Sub           :
                sub();
                break;

            case Mul           :
                mul();
                break;

            case Div           :
                div();
                break;

            case Mod           :
                mod();
                break;

            case Equal         :
                equal();
                break;

            case NotEqual      :
                notEqual();
                break;

            case Push          :
                push(value);
                break;

            case PushTop       :
                pushTop();
                break;

            case Pop           :
                pop();
                break;

            case SwapTop       :
                swapTop(value);
                break;

            case JumpIfNotZero :
                jump_lable = jumpIfNotZero(label_code_to_pc[value]);
                if(jump_lable != -1) {
                    pc = jump_lable;
                    continue;
                }
                break;

            case JumpIfZero    :
                jump_lable = jumpIfZero(label_code_to_pc[value]);
                if(jump_lable != -1) {
                    pc = jump_lable;
                    continue;
                }
                break;

            case Jump          :
                pc = label_code_to_pc[value];
                continue;
                // break;

            case Print         :
                printTop();
                break;

            case Stop          :
                std::cout << "Stop" << std::endl;
                pc = size;
                continue;

            case Label         :
            default            :
                break;
            }

            pc++;
        }
    }

    static std::map<int, int> LabelToProgramCount(const std::vector<std::tuple<Operation, int>>& codes)
    {
        size_t iter = 0;
        size_t size = codes.size();
        std::map<int, int> lines;
        while(iter < size) {
            auto& item = codes.at(iter);

            if(std::get<0>(item) == Label) {
                // lines.push_back(iter);
                lines[std::get<1>(item)] = iter;
            }

            iter++;
        }
        return lines;
    }

    size_t evaluationStackSize() const{
        return evaluation_stack.size();
    }

private:
    std::stack<int> evaluation_stack;
};

typedef std::tuple<StackBaseVirtualMachine::Operation, int> Code;


}
}

#endif // STACKBASEVIRTUALMACHINE

