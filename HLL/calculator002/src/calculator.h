#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <string>
#include <stack>
#include <vector>
#include <iostream>
#include <assert.h>

namespace qyvlik {

static std::string add(const std::string& arg0, const std::string& arg1) {
    int a0 = std::stoi(arg0);
    int a1 = std::stoi(arg1);
    return std::to_string(a0 + a1);
}

static std::string sub(const std::string& arg0, const std::string& arg1) {
    int a0 = std::stoi(arg0);
    int a1 = std::stoi(arg1);
    return std::to_string(a0 - a1);
}

static std::string mul(const std::string& arg0, const std::string& arg1) {
    int a0 = std::stoi(arg0);
    int a1 = std::stoi(arg1);
    return std::to_string(a0 * a1);
}

static std::string div(const std::string& arg0, const std::string& arg1) {
    int a0 = std::stoi(arg0);
    int a1 = std::stoi(arg1);

    // std::cout << "arg1: " << arg1 << std::endl;

    assert(a1 != 0);

    return std::to_string(a0 / a1);
}

static std::string calcula(const std::string& operation, const std::string& arg0, const std::string& arg1) {

    if(operation == "+") {

        return add(arg0, arg1);
    } else if(operation == "-") {

        return sub(arg0, arg1);
    } else if(operation == "*") {

        return mul(arg0, arg1);
    } else if(operation == "/") {

        return div(arg0, arg1);
    } else {
        assert(0);
        return std::string("0");
    }
}

static inline int add(const int& arg0, const int& arg1)
{
    return arg0 + arg1;
}

static inline int sub(const int& arg0, const int& arg1)
{
    return arg0 - arg1;
}

static inline mul(const int& arg0, const int& arg1)
{
    return arg0 * arg1;
}

static inline div(const int& arg0, const int& arg1)
{
    assert( arg1 != 0);
    return arg0 / arg1;
}

}

class Calculator {
public:
    Calculator()
    {}

    Calculator(const std::vector<std::string>& code):
        code(code)
    { }

    // op 后必定有两个操作数

    bool isOperator(const std::string& s) {
        return s == "+" || s == "-" || s == "*" || s == "/";
    }

    std::string result() {

        std::string current;

        do {
            current = code.front();
            code.erase(code.begin());

            if(!isOperator(current)) {
                calc.push(current);
            } else {

                std::string arg0 = calc.top();
                calc.pop();
                std::string arg1 = calc.top();
                calc.pop();

                calc.push(qyvlik::calcula(current, arg0, arg1));
            }
        } while(!code.empty());

        return calc.top();
    }

    std::vector<std::string> code;
    std::stack<std::string> calc;
};


#endif // CALCULATOR_H
