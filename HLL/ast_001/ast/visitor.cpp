#include "visitor.h"
#include "node.h"

#include <QtDebug>

#include "types/buildintype.h"

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

/////////////TypeSystemVisitor///////////

TypeSystemVisitor::TypeSystemVisitor()
{
    unCompleteType.insert("stack", new StackTypeFactory());
    typeNameMap.insert("stack", "stack");

    unCompleteType.insert("map", new MapTypeFactory());
    typeNameMap.insert("map", "map");

    completeType.insert("int", new BaseTypeMetaData<int>());
    typeNameMap.insert("int", "int");

    completeType.insert("double", new BaseTypeMetaData<double>());
    typeNameMap.insert("double", "double");
}

void TypeSystemVisitor::visit(Node * node)
{
    Q_ASSERT(node != nullptr);
    qDebug() << "Node(" << node << ")";
}

// 定义类型
void TypeSystemVisitor::visit(TypeDefineNode * node)
{
    Q_ASSERT(node != nullptr);
    qDebug() << "TypeDefineNode(" << node << ")" ;
    this->visit(node->typeSpecifierNode);
    this->visit(node->typeNameNode);
}

// 模板类型
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
