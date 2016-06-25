#ifndef PARSERCONTEXT_HPP
#define PARSERCONTEXT_HPP

#include <stack>
#include <memory>
#include <iostream>
#include <string>
#include <tuple>
#include <map>
#include <algorithm>
#include <assert.h>

#include "../throwable/throwable.hpp"
#include "token.hpp"

namespace qyvlik {

// Identity, methodId
class Symbol
{
public:
    Symbol():
        block_count(0)
    {}

    bool insertSymbol(int currentBlockId, const std::string& symbolName) {
        if(existsSymbol(currentBlockId, symbolName)) {
            return false;
        } else {
            symbol.insert(std::pair<int, std::string>(currentBlockId, symbolName));
            return true;
        }
    }

//    // 只在嵌套的块中寻找
//    bool existsSymbol(int currentBlockId, const std::string& symbolName) const {
//        (void)currentBlockId;
//        return std::find_if(
//                    symbol.begin(),
//                    symbol.end(),
//                    [&](const std::pair<int, std::string>& item) -> bool {
//            return std::find_if(block_ids.back(), block_ids.front(),
//                                [&](const int& stack_item){
//                return stack_item == item.first;
//            }) != block_ids.end()
//                    && item.second == symbolName;
//        }) != symbol.end();
//    }

    bool existsSymbol(int currentBlockId, const std::string& symbolName) const {
        return std::find_if(
                    symbol.begin(),
                    symbol.end(),
                    [&](const std::pair<int, std::string>& item) -> bool {
            return item.first == currentBlockId && item.second == symbolName;
        }) != symbol.end();
    }

    bool inBlock() const {
        return block_ids.size() > 0;
    }

    void enterBlock() {
        block_ids.push_back(block_count);
        block_count++;
    }

    void exitBlock() {
        if(!block_ids.empty()) {
            block_ids.pop_back();
        }
    }

    int currentBlockId() const {
        if(!block_ids.empty()) {
            return block_ids.back();
        } else {
            return -1;
        }
    }

private:
    // TODO 块状代码。嵌套才可达。
    std::multimap<int, std::string> symbol;
    std::vector<int> block_ids;
    int block_count;
};

class ParserContext
{
public:
    ParserContext():
        loop_statement_count(0),
        function_body_count(0),
        label_count(0),
        expression_statement_count(0),
        delcaration_statement_count(0)
    {}

    virtual ~ParserContext()
    { }

    virtual void clear()
    {
        loop_statement_count = 0;
        function_body_count = 0;
        label_count = 0;
        expression_statement_count = 0;

        labels.clear();
        loop_start_labels.clear();
        loop_end_labels.clear();
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
            std::cout << "\tPop Token Null" << std::endl;
            std::cout << "-------------------------------" << std::endl;
            CALLEE_PRINT_TRACK;
            std::cout << "-------------------------------" << std::endl;
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

    bool insertSymbol(int currentBlockId, const std::string& symbolName) {
        return symbol.insertSymbol(currentBlockId, symbolName);
    }

    bool existsSymbol(int currentBlockId, const std::string& symbolName) const {
        return symbol.existsSymbol(currentBlockId, symbolName);
    }

    bool inDeclarationStatement() {
        return delcaration_statements.size() != 0;
    }

    void enterDeclarationStatement() {
        delcaration_statements.push_back(delcaration_statement_count);
        delcaration_statement_count++;
    }

    void exitDeclarationStatement() {
        if(!delcaration_statements.empty()) {
            delcaration_statements.pop_back();
        }
    }

    int currentDeclarationStatementId() {
        if(!delcaration_statements.empty()) {
            return delcaration_statements.back();
        } else {
            return -1;
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
        expression_statements.push_back(expression_statement_count);
        expression_statement_count++;
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

    bool inBlock() const {
        return symbol.inBlock();
    }

    void enterBlock() {
        symbol.enterBlock();
    }

    void exitBlock() {
        symbol.exitBlock();
    }

    int currentBlockId() const {
        return symbol.currentBlockId();
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
    int label_count;
    int expression_statement_count;
    int delcaration_statement_count;

    std::vector<int> labels;
    std::vector<int> loop_start_labels;
    std::vector<int> loop_end_labels;
    std::vector<int> expression_statements;
    std::vector<int> temporary_ids;
    std::vector<Token> tokens;
    std::vector<int> delcaration_statements;
    Symbol symbol;


};

}

#endif // PARSERCONTEXT_HPP

