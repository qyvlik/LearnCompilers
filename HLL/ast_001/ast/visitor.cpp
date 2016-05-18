#include "visitor.h"

#include "node.h"

#include <QtDebug>

namespace qyvlik {
namespace typer {

Visitor::~Visitor()
{}

void Visitor::visit(Node * node)
{
    Q_ASSERT(node != nullptr);
    qDebug() << "Node(" << node << ")";
}

void Visitor::visit(TypeDefineNode * node)
{
    Q_ASSERT(node != nullptr);
    qDebug() << "TypeDefineNode(" << node << ")" ;
    this->visit(node->typeSpecifierNode);
    this->visit(node->typeNameNode);
}

void Visitor::visit(TypeSpecifierNode * node)
{
    Q_ASSERT(node != nullptr);
    //    std::cout << "TypeSpecifierNode(" << node << ")" << std::endl;
    if(node->kind == Node::Kind_TypeName) {
        this->visit(dynamic_cast<TypeNameNode*>(node));
    } else if(node->kind == Node::Kind_TemplateTypeSpecifier) {
        this->visit(dynamic_cast<TemplateTypeSpecifierNode*>(node));
    }

}

void Visitor::visit(TemplateTypeSpecifierNode * node)
{
    Q_ASSERT(node != nullptr);
    qDebug() << "TemplateTypeSpecifierNode(" << node << ")" ;
    auto iter = node->nodes.begin();
    auto end = node->nodes.end();
    while(iter != end) {
        this->visit(*iter);
        iter++;
    }
}

void Visitor::visit(TypeNameNode * node)
{
    Q_ASSERT(node != nullptr);
    qDebug() << "TypeNameNode(" << node << ")";
}

////////////////////////

void TypeSystemVisitor::visit(Node * node)
{
    Q_ASSERT(node != nullptr);
    qDebug() << "Node(" << node << ")";
}

void TypeSystemVisitor::visit(TypeDefineNode * node)
{
    Q_ASSERT(node != nullptr);
    qDebug() << "TypeDefineNode(" << node << ")" ;
    this->visit(node->typeSpecifierNode);
    this->visit(node->typeNameNode);
}

void TypeSystemVisitor::visit(TypeSpecifierNode * node)
{
    Q_ASSERT(node != nullptr);
    //    std::cout << "TypeSpecifierNode(" << node << ")" << std::endl;
    if(node->kind == Node::Kind_TypeName) {
        this->visit(dynamic_cast<TypeNameNode*>(node));
    } else if(node->kind == Node::Kind_TemplateTypeSpecifier) {
        this->visit(dynamic_cast<TemplateTypeSpecifierNode*>(node));
    }

}

void TypeSystemVisitor::visit(TemplateTypeSpecifierNode * node)
{
    Q_ASSERT(node != nullptr);
    qDebug() << "TemplateTypeSpecifierNode(" << node << "," << node->templateName << ")" ;
    auto iter = node->nodes.begin();
    auto end = node->nodes.end();
    while(iter != end) {
        this->visit(*iter);
        iter++;
    }
}

void TypeSystemVisitor::visit(TypeNameNode * node)
{
    Q_ASSERT(node != nullptr);
    qDebug() << "TypeNameNode(" << node << "," << node->typeName << ")";
}

}
}
