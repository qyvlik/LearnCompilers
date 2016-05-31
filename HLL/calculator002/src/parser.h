#ifndef PARSER_H
#define PARSER_H

#include "ast.h"

#include <vector>
#include <string>
#include <stack>
#include <cassert>

// expr -> expr + term
//       | expr - term
//       | term

// term -> term * factor
//       | term / factor
//       | factor

// factor -> ( expr )
//         | number
//         | id


// expr     -> term trest

// trest    -> + term
//           | - term
//           | ε

// term     -> factor frest

// frest    -> * factor
//           | / factor
//           | ε

// factor   -> NUMBER
//           | ( expr )

class Node;
class ExprNode;
class TermNode;
class FactorNode;

class ASTTree {
public:
    ASTTree()
    {}

    std::shared_ptr<ExprNode> createExprNode( std::shared_ptr<ExprNode> first,
                                              const Calc& calc,
                                              std::shared_ptr<TermNode> second)
    {
        assert(first != nullptr && second != nullptr);

        std::shared_ptr<ExprNode> expr (new ExprNode);

        nodes.push_back(expr);

        expr->first = first;
        expr->calc = calc;
        expr->second = second;
        return expr;
    }

    std::shared_ptr<TermNode> createTermNode( std::shared_ptr<TermNode> first,
                                              const Calc& calc,
                                              std::shared_ptr<FactorNode> second)
    {
        assert(first != nullptr && second != nullptr);

        std::shared_ptr<TermNode> term (new TermNode);

        nodes.push_back(term);

        term->first = first;
        term->calc = calc;
        term->second = second;
        return term;
    }

    std::shared_ptr<FactorNode> createFactorNode(std::shared_ptr<ExprNode> expr) {

        assert(expr != nullptr);

        std::shared_ptr<FactorNode> factor(new FactorNode);

        nodes.push_back(factor);

        factor->expr = expr;

        return factor;
    }

    std::shared_ptr<FactorNode> createFactorNode(int value) {
        std::shared_ptr<FactorNode> factor(new FactorNode);

        nodes.push_back(factor);

        factor->value = value;

        return factor;
    }

    std::shared_ptr<Node> astNode;
    std::vector< std::shared_ptr<Node> > nodes;             // save the nodes
    std::stack< std::shared_ptr<Node> > nodeVar;
};

class LexerStream;
class Parser {
public:
    void expr(LexerStream* lexers);

    void term(LexerStream* lexers);

    void factor(LexerStream* lexers);

    std::vector<std::string> code;

    ASTTree asttree;
};



#endif // PARSER_H
