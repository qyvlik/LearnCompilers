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

class StackBaseVirtualMachine
{
public:

    typedef std::vector<int> Arguments;
    struct StackFrame {
        StackFrame(int id):
            methodId(id)
        { }
        int methodId;
        int result;
        Arguments arguments;
    };

    typedef std::stack<StackFrame> CallStack;
    // for  int (int...) function ;
    typedef void(*CallMethod)(Arguments*, int* result);
    typedef std::map<int, CallMethod> NativeMethods;

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

        BeforeCall,         // BeforeCall methodNameId
        Param,
        Call,               // Call methodNameId

        Print,

        Label,

        Stop,
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

                // BeforeCall methodId
            case BeforeCall    :
                call_stack.push(StackFrame(value));

                std::cout << "Into Call Scope" << std::endl;

                break;

            case Param         :
                call_stack.top().arguments.push_back(value);
                break;

            case Call          :
                native_methods[call_stack.top().methodId](&call_stack.top().arguments, &call_stack.top().result);
                push(call_stack.top().result);
                call_stack.pop();

                std::cout << "Go Out Call Scope" << std::endl;
                break;

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

    void registerNativeMethod(int methodNameId, CallMethod method) {
        native_methods.insert(std::pair<int, CallMethod>(methodNameId, method));
    }

private:
    std::stack<int> evaluation_stack;
    CallStack call_stack;
    NativeMethods native_methods;
};

typedef std::tuple<StackBaseVirtualMachine::Operation, int> Code;


}
}

#endif // STACKBASEVIRTUALMACHINE

