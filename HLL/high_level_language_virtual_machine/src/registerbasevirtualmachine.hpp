#ifndef REGISTERBASEVIRTUALMACHINE_HPP
#define REGISTERBASEVIRTUALMACHINE_HPP

//! code from [mini-vm](https://github.com/philipaconrad/mini-vm)

#include <iostream>
#include <stdint.h>
#include <assert.h>

namespace qyvlik {

namespace virtual_machine {

struct Instruction {
    int operation;
    int dst;
    int arg0;
    int arg1;
};

struct Register {
    int type;
    int value;
};

class RegisterBaseVirtualMachine;

typedef void (*FunPtr)(RegisterBaseVirtualMachine* ctx, const Instruction& instruction);

class RegisterBaseVirtualMachine
{
public:
    RegisterBaseVirtualMachine(const std::vector<FunPtr>& f, const std::vector<Register>& r):
        function_table(f),
        register_vector(r)
    {

    }

    void dispatch(const Instruction& instr) {
        const int i = instr.operation;
        (this->function_table.at(i))(this, instr);
    }

    void stepVMContext(const Instruction& instr) {
        // Dispatch to an opcode-handler.
        this->dispatch(instr);

        // Increment to next instruction.
    }

    std::vector<FunPtr> function_table;
    std::vector<Register> register_vector;
};


void LoadNumberToRegister(RegisterBaseVirtualMachine* ctx, const Instruction& instr) {
    const int a = instr.dst;                    // register_id
    const int b = instr.arg0;                   // number not register_id
    ctx->register_vector.at(a).value = b;
}

// add R0, R1, R2 -> R0 = R1 + R2
void AddRegister(RegisterBaseVirtualMachine* ctx, const Instruction& instr) {
    const int a = instr.dst;                    // register_id
    const int b = instr.arg0;                   // register_id
    const int c = instr.arg1;                   // register_id
    ctx->register_vector.at(a).value = ctx->register_vector.at(b).value + ctx->register_vector.at(c).value;
}

void SubRegister(RegisterBaseVirtualMachine* ctx, const Instruction& instr) {
    const int a = instr.dst;                    // register_id
    const int b = instr.arg0;                   // register_id
    const int c = instr.arg1;                   // register_id
    ctx->register_vector.at(a).value = ctx->register_vector.at(b).value - ctx->register_vector.at(c).value;
}

void PrintRegister(RegisterBaseVirtualMachine* ctx, const Instruction& instr) {
    const int a = instr.dst;                    // register_id
    printf("type: %d, value: %d \n", ctx->register_vector.at(a).type, ctx->register_vector.at(a).value);
}


}

}
#endif // REGISTERBASEVIRTUALMACHINE_HPP

