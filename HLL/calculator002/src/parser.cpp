#include "parser.h"
#include "lexerstream.h"

#include "calculator.h"

#include <iostream>


// expr -> expr + term
//       | expr - term
//       | term
void Parser::expr(LexerStream *lexers)
{
    term(lexers);

    while(lexers->current() == "+" || lexers->current() == "-") {

        std::string op = lexers->current();

        lexers->next();
        term(lexers);

        std::cout << op;     // PUSH

        code.push_back(op);

        std::shared_ptr<ExprNode> frist = std::dynamic_pointer_cast<ExprNode>(asttree.nodeVar.top()) ;
        asttree.nodeVar.pop();

        std::shared_ptr<TermNode> second = std::dynamic_pointer_cast<TermNode>(asttree.nodeVar.top()) ;
        asttree.nodeVar.pop();

        std::shared_ptr<ExprNode> exprNode ;

        if(op == "+") {
            exprNode = asttree.createExprNode(frist, &qyvlik::add, second);

        } else {
            exprNode = asttree.createExprNode(frist, &qyvlik::sub, second);
        }

        asttree.nodeVar.push(exprNode);
    }

    asttree.astNode = asttree.nodeVar.top();

}

// term -> term * factor
//       | term / factor
//       | factor
void Parser::term(LexerStream *lexers)
{
    factor(lexers);

    while(lexers->current() == "*" || lexers->current() == "/") {

        std::string op = lexers->current();

        lexers->next();
        factor(lexers);

        std::cout << op;     // PUSH
        code.push_back(op);

        std::shared_ptr<TermNode> frist = std::dynamic_pointer_cast<TermNode>(asttree.nodeVar.top()) ;
        asttree.nodeVar.pop();

        std::shared_ptr<FactorNode> second = std::dynamic_pointer_cast<FactorNode>(asttree.nodeVar.top()) ;
        asttree.nodeVar.pop();

        std::shared_ptr<TermNode> termNode;

        if(op == "*") {
            termNode = asttree.createTermNode(frist, &qyvlik::mul, second);
        } else {
            std::cout << std::endl;
            std::cout << "lexers->current()" << op << std::endl;
            termNode = asttree.createTermNode(frist, &qyvlik::div, second);
        }

        asttree.nodeVar.push(termNode);
    }
}


// factor -> ( expr )
//         | number
//         | id
void Parser::factor(LexerStream *lexers)
{
    std::string current = lexers->current();

    if(current == "(") {
        lexers->next();

        expr(lexers);

        lexers->next();

        std::shared_ptr<ExprNode> exprNode = std::dynamic_pointer_cast<ExprNode>(asttree.nodeVar.top()) ;
        asttree.nodeVar.pop();

        std::shared_ptr<FactorNode> factorNode =  asttree.createFactorNode(exprNode);
        asttree.nodeVar.push(factorNode);

//        std::string e = lexers->current();

//        if(e != ")") {
//            std::cout << "lost `(" << std::endl;
//        }
    } else {
        std::cout << current;      // PUSH

        code.push_back(current);

        std::shared_ptr<FactorNode> factorNode =  asttree.createFactorNode(current);
        asttree.nodeVar.push(factorNode);

        lexers->next();
    }
}
