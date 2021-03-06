#ifndef AST_H
#define AST_H

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <functional>

class Node;
class ExprNode;
class TermNode;
class FactorNode;

class Visitor {
public:
    virtual void visit(std::shared_ptr<Node> node) ;
    virtual void visit(std::shared_ptr<ExprNode> node);
    virtual void visit(std::shared_ptr<TermNode> node);
    virtual void visit(std::shared_ptr<FactorNode> node);

    void print() const {

        size_t count = valueStack.size();

        while(count-- > 0) {
            std::cout << valueStack.at(count) << "  ";
        }

        std::cout << std::endl;

    }

    void clear() {
        valueStack.clear();
    }

private:
    std::vector<int> valueStack;
};


// expr -> expr + term
//       | expr - term
//       | term

// term -> term * factor
//       | term / factor
//       | factor

// factor -> ( expr )
//         | number

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

// typedef std::string(*Calc)(const std::string&, const std::string&);

typedef int(*Calc)(const int&, const int&);

class Node : public std::enable_shared_from_this<Node>
{
public:
    enum NodeType {
        Expr,
        Term,
        Factor,
        NumberLiteral,
    };
    virtual ~Node()
    {
        std::cout << "kind: " << kind << std::endl;
    }
    virtual void accept(Visitor* visitor) = 0;
    int kind;
};


#define TYPE(NODETYPE) enum { K = NODETYPE };


// expr -> expr + term
//       | expr - term
//       | term
class ExprNode : public Node
{
public:
    TYPE(Expr)

    ExprNode(){
        kind = K;
    }
    void accept(Visitor* visitor) override;

    std::weak_ptr<ExprNode> first;
    std::weak_ptr<TermNode> second;

    Calc calc;

};


// term -> term * factor
//       | term / factor
//       | factor
class TermNode : public ExprNode
{
public:
    TYPE(Term)

    TermNode(){
        kind = K;
    }
    void accept(Visitor* visitor) override;
    std::weak_ptr<TermNode> first;
    // std::string op;         // *, /
    std::weak_ptr<FactorNode> second;

    // std::function<std::string(const std::string& , const std::string)> calc;
};

// factor -> ( expr )
//         | number
class FactorNode : public TermNode
{
public:
    TYPE(Factor)

    FactorNode(){
        kind = K;
    }
    void accept(Visitor* visitor) override;

    int value;                                  // literal
    std::weak_ptr<ExprNode> expr;               // 要么是 value ，要么是 expr
};



#endif // AST_H
