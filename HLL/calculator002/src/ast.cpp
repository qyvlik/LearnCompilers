#include "ast.h"

#include "calculator.h"

#include <assert.h>

void Visitor::visit(std::shared_ptr<Node> node)
{
    assert(node != nullptr);
    node->accept(this);
}

// expr -> expr + term
//       | expr - term
//       | term
void Visitor::visit(std::shared_ptr<ExprNode> node)
{
    assert(node != nullptr);
    std::shared_ptr<ExprNode> first = node->first.lock();

    first->accept(this);

    std::string op = node->op;

    std::shared_ptr<TermNode> second = node->second.lock();

    second->accept(this);

    std::string arg0 = this->valueStack.front();
    this->valueStack.erase(this->valueStack.begin());

    std::string arg1 = this->valueStack.front();
    this->valueStack.erase(this->valueStack.begin());

    valueStack.push_back(calcula(op, arg0, arg1));

}

// term -> term * factor
//       | term / factor
//       | factor
void Visitor::visit(std::shared_ptr<TermNode> node)
{
    assert(node != nullptr);
    std::shared_ptr<ExprNode> first = node->first.lock();

    first->accept(this);

    std::string op = node->op;

    std::shared_ptr<TermNode> second = node->second.lock();

    second->accept(this);

    std::string arg0 = this->valueStack.front();
    this->valueStack.erase(this->valueStack.begin());

    std::string arg1 = this->valueStack.front();
    this->valueStack.erase(this->valueStack.begin());

    valueStack.push_back(calcula(op, arg0, arg1));
}

// factor -> ( expr )
//         | number
void Visitor::visit(std::shared_ptr<FactorNode> node)
{
    assert(node != nullptr);

    std::shared_ptr<ExprNode> expr = node->expr.lock();

    if(expr) {
        expr->accept(this);
    } else {
        this->valueStack.push_back(node->token);
    }
}

//----------------------- Node ----------------------------

// expr -> expr + term
//       | expr - term
//       | term
void ExprNode::accept(Visitor *visitor)
{
    visitor->visit(
                std::dynamic_pointer_cast<ExprNode>(
                    this->shared_from_this()
                    )
                );
}

// term -> term * factor
//       | term / factor
//       | factor
void TermNode::accept(Visitor *visitor)
{
    visitor->visit(
                std::dynamic_pointer_cast<TermNode>(
                    this->shared_from_this()
                    )
                );
}

// factor -> ( expr )
//         | number
void FactorNode::accept(Visitor *visitor)
{
    visitor->visit(
                std::dynamic_pointer_cast<FactorNode>(
                    this->shared_from_this()
                    )
                );
}

