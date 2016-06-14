#ifndef PARSERCONTEXT_HPP
#define PARSERCONTEXT_HPP

#include <stack>
#include <memory>
#include <iostream>
#include <string>

#include "abstractsyntaxtree.hpp"
#include "throwable.hpp"
#include "token.hpp"

class ParserContext
{
public:
    enum StatementType {
        Program,

        IfStatement,

        LoopStatement,

        DoWhileStatement,
        WhileStatement,
        ForStatement,

        FunctionBody,

    };

    ParserContext():
        loop_statement_count(0),
        function_body_count(0),
        block_count(0)
    {}

    virtual ~ParserContext()
    {}

    virtual void clear()
    {}

    bool inLoopStatement() const {
        return loop_statement_count != 0;
    }

    bool inFunctionBody() const {
        return function_body_count != 0;
    }

    bool InBlock() const {
        return block_count != 0;
    }

    void enterLoopStatement() {
        loop_statement_count++;
    }

    void exitLoopStatement() {
        loop_statement_count--;
    }

    void enterFunctionBody() {
        function_body_count++;
    }

    void exitFunctionBody() {
        function_body_count--;
    }

    void enterBlock() {
        block_count++;
    }

    void exitBlock() {
        block_count--;
    }

private:
    int loop_statement_count;
    int function_body_count;
    int block_count;

};

#endif // PARSERCONTEXT_HPP

