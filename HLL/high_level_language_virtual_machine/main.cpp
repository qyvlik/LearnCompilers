#include <QCoreApplication>

#include "src/registerbasevirtualmachine.hpp"

#include "src/stackbasevirtualmachine.hpp"

using namespace qyvlik;
using namespace virtual_machine;

void test_stack_base();
void test_stack_base_0();
void test_stack_base_1();
void test_stack_base_2();

int main()
{
    //    test_register_base();
    //    test_stack_base_0();
//    test_stack_base_1();
    test_stack_base_2();
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

// typedef void(*CallMethod)(StackBaseVirtualMachine::Arguments*, int* result);


// log(1,2,3) -> 3
void sb_vm_test_native_method_log(StackBaseVirtualMachine::Arguments* args, int* result)
{
    std::cout << "args ptr:" << "[" << args << "]" << std::endl;
    std::cout << "result ptr:" << "[" << result << "]" << std::endl;

    int size = 0;

    if(args && args->size() != 0) {
        auto iter = args->begin();
        auto end = args->end();
        while(iter != end){

            std::cout << (*iter);
            size++;
            iter++;
        }
        std::cout << std::endl;
    }
    if(result) {
        *result = size;
    }
}

void test_stack_base_2()
{
    std::vector< Code> codes;

    StackBaseVirtualMachine vm;

    vm.registerNativeMethod(0, sb_vm_test_native_method_log);

    // beforecall 0
    // param 1
    // param 2
    // param 3
    // call 0
    // print    // 3

    codes.push_back(Code(StackBaseVirtualMachine::BeforeCall, 0));
    codes.push_back(Code(StackBaseVirtualMachine::Param,  1));
    codes.push_back(Code(StackBaseVirtualMachine::Param,  2));
    codes.push_back(Code(StackBaseVirtualMachine::Param,  3));
    codes.push_back(Code(StackBaseVirtualMachine::Call, 0));
    codes.push_back(Code(StackBaseVirtualMachine::Print, 0));

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
