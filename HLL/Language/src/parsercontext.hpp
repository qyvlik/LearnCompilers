#ifndef PARSERCONTEXT_HPP
#define PARSERCONTEXT_HPP

#include <stack>
#include <memory>
#include <iostream>
#include <string>

#include <assert.h>
#include <tuple>

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
        block_count(0),
        label_count(0),
        expression_statement_count(0)
    {}

    virtual ~ParserContext()
    { }

    virtual void clear()
    {
        loop_statement_count = 0;
        function_body_count = 0;
        block_count = 0;
        label_count = 0;
        expression_statement_count = 0;

        labels.clear();
        loop_start_labels.clear();
        loop_end_labels.clear();
        block_ids.clear();
        expression_statements.clear();
        temporary_ids.clear();
        tokens.clear();
    }

    Token popToken() {
        if(!tokens.empty()) {
            Token item = tokens.back();
            tokens.pop_back();
            return item;
        } else {
            return Token();
        }
    }

    Token topToken() const {
        if(!tokens.empty()) {
            return tokens.back();
        } else {
            return Token();
        }
    }

    void pushToken(const Token& token) {
        tokens.push_back(token);
    }

    int getTemporaryId() {
        int current = temporary_ids.size() - 1;
        if(current < 0) {
            temporary_ids.push_back(0);
            return 0;
        } else {
            int id = temporary_ids.at(current);
            temporary_ids[current] = ++id;
            return id;
        }
    }

    int currentExpressionStatementId() {
        if(!expression_statements.empty()) {
           return expression_statements.back();
        } else {
            return -1;
        }
    }

    void enterExpressionStatement() {
        expression_statement_count++;
        expression_statements.push_back(expression_statement_count);
        temporary_ids.push_back(0);
    }

    void exitExpressionStatement() {
        if(!expression_statements.empty()) {
            expression_statements.pop_back();
        }

        if(!temporary_ids.empty()) {
            temporary_ids.pop_back();
        }
    }

    bool inLoopStatement() const {
        return loop_statement_count > 0;
    }

    void enterLoopStatement() {
        loop_statement_count++;
    }

    void exitLoopStatement() {
        loop_statement_count--;
    }

    bool inFunctionBody() const {
        return function_body_count > 0;
    }

    void enterFunctionBody() {
        function_body_count++;
    }

    void exitFunctionBody() {
        function_body_count--;
    }

    bool InBlock() const {
        return block_ids.size() > 0;
    }

    void enterBlock() {
        block_count++;
        block_ids.push_back(block_count);
    }

    void exitBlock() {
        if(!block_ids.empty()) {
            block_ids.pop_back();
        }
    }

    int getBlockId() const {
        if(!block_ids.empty()) {
            return block_ids.back();
        } else {
            return -1;
        }
    }

    int getNewLabel() {
        return label_count++;
    }

    void pushLabel(int l) {
        labels.push_back(l);
    }

    void popLable() {
        if(!labels.empty()) {
            labels.pop_back();
        }
    }

    void pushLoopStartLabel(int l) {
        labels.push_back(l);
        loop_start_labels.push_back(l);
    }

    void popLoopStartLabel() {
        popLable();
        if(!loop_start_labels.empty()) {
            loop_start_labels.pop_back();
        }
    }

    void pushLoopEndLabel(int l) {
        labels.push_back(l);
        loop_end_labels.push_back(l);
    }

    void popLoopEndLabel() {
        popLable();
        if(!loop_end_labels.empty()) {
            loop_end_labels.pop_back();
        }
    }

    int getLoopStartLabel() const {
        if(!loop_start_labels.empty()) {
            return loop_start_labels.back();
        } else {
            return -1;
        }
    }

    int getLoopEndLabel() const {
        if(!loop_end_labels.empty()) {
            return loop_end_labels.back();
        } else {
            return -1;
        }
    }

private:
    int loop_statement_count;
    int function_body_count;
    int block_count;
    int label_count;
    int expression_statement_count;

    std::vector<int> labels;
    std::vector<int> loop_start_labels;
    std::vector<int> loop_end_labels;
    std::vector<int> block_ids;
    std::vector<int> expression_statements;
    std::vector<int> temporary_ids;
    std::vector<Token> tokens;
};

#endif // PARSERCONTEXT_HPP

