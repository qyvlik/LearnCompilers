#include <QCoreApplication>

#include "src/registerbasevirtualmachine.hpp"

#include "src/stackbasevirtualmachine.hpp"

using namespace qyvlik;
using namespace virtual_machine;

void test_stack_base();
void test_stack_base_0();
void test_stack_base_1();

int main()
{
    //    test_register_base();
//    test_stack_base_0();
    test_stack_base_1();
    return 0;
}

void test_stack_base_0()
{
    std::vector< Code> codes;

    //    if(true) {
    //        1+1;
    //    }
    // push true
    // JumpIfZero 1
    // lable 0
    // push 1
    // push 1
    // add
    // print
    // lable 1

    StackBaseVirtualMachine vm;

    codes.push_back(Code(StackBaseVirtualMachine::Push, 0));
    codes.push_back(Code(StackBaseVirtualMachine::JumpIfZero, 1));
    codes.push_back(Code(StackBaseVirtualMachine::Label, 0));
    codes.push_back(Code(StackBaseVirtualMachine::Push, 1));
    codes.push_back(Code(StackBaseVirtualMachine::Push, 1));
    codes.push_back(Code(StackBaseVirtualMachine::Add,  0));
    codes.push_back(Code(StackBaseVirtualMachine::Print,  0));
    codes.push_back(Code(StackBaseVirtualMachine::Label, 1));

    codes.push_back(Code(StackBaseVirtualMachine::Push, 999));
    codes.push_back(Code(StackBaseVirtualMachine::Print,  0));

    vm.execute(codes);
}

void test_stack_base_1()
{
    std::vector< Code> codes;

//    int a = 1000000;
//    while(a != 0) {
//        a--;
//    }

    // push 1000000
    // label 0
    // pushtop
    // push 0
    // not_equal
    // jump_if__zero 1
    // push 1
    // sub
    // jump 0
    // lable 1

    StackBaseVirtualMachine vm;

    codes.push_back(Code(StackBaseVirtualMachine::Push, 1000000));
    codes.push_back(Code(StackBaseVirtualMachine::Label, 0));
    codes.push_back(Code(StackBaseVirtualMachine::PushTop, 1));
    codes.push_back(Code(StackBaseVirtualMachine::Push, 0));
    codes.push_back(Code(StackBaseVirtualMachine::NotEqual, 0));
    codes.push_back(Code(StackBaseVirtualMachine::JumpIfZero, 1));
    codes.push_back(Code(StackBaseVirtualMachine::Push,  1));
    codes.push_back(Code(StackBaseVirtualMachine::Sub,  0));
    // codes.push_back(Code(StackBaseVirtualMachine::Print,  0));
    codes.push_back(Code(StackBaseVirtualMachine::Jump, 0));
    codes.push_back(Code(StackBaseVirtualMachine::Label, 1));

    vm.execute(codes);

    std::cout << "size: " << vm.evaluationStackSize() << std::endl;
}

void test_register_base()
{
    enum {
        Load = 0,
        Add,
        Sub,
        Print
    };

    enum {
        R0 = 0,
        R1,
        R2
    };

    std::vector<FunPtr> fun;
    fun.push_back(LoadNumberToRegister);                        // 0
    fun.push_back(AddRegister);                                 // 1
    fun.push_back(SubRegister);                                 // 2
    fun.push_back(PrintRegister);                               // 3

    std::vector<Register> reg(16);

    RegisterBaseVirtualMachine vm(fun, reg);

    std::vector<Instruction> instructions;
    // load r1 #1
    // load r2 #1
    // add r1, r2, r1
    // print r1
    // [0, r1, #1], [0, r2, #1], [1, r1, r1, r2], [3, r1]
    Instruction i;

    i.operation = Load;
    i.dst = R1;
    i.arg0 = 1;
    i.arg1 = 0;
    instructions.push_back(i);

    i.operation = 0;
    i.dst = R2;
    i.arg0 = 1;
    i.arg1 = 0;
    instructions.push_back(i);

    i.operation = 1;
    i.dst = R1;
    i.arg0 = R1;
    i.arg1 = R2;
    instructions.push_back(i);

    i.operation = 1;
    i.dst = R1;
    i.arg0 = R1;
    i.arg1 = R2;
    instructions.push_back(i);

    i.operation = 1;
    i.dst = R1;
    i.arg0 = R1;
    i.arg1 = R2;
    instructions.push_back(i);

    i.operation = Print;
    i.dst = R1;
    i.arg0 = 0;
    i.arg1 = 0;
    instructions.push_back(i);

    int size = instructions.size();
    int iter = 0;
    while(iter < size) {
        vm.stepVMContext(instructions.at(iter));
        iter++;
    }

}
