#ifndef NODE_H
#define NODE_H

#include <QtDebug>

namespace qyvlik {
namespace typer {

class SourceLocal {
public:
    qint64 start;
    qint64 offset;
    qint64 line;
    qint64 column;
};

#define DECLARE_AST_NODE(name) \
    enum { K = Kind_##name };

class Visitor;
class Node{
public:
    enum Type {
        Kind_TypeDefine,
        Kind_TypeSpecifier,
        Kind_TemplateTypeSpecifier,
        Kind_TypeName
    };

    virtual ~Node();
    virtual void accept(Visitor* visitor) = 0 ;

    int kind;
};

class TypeSpecifierNode : public Node
{
public:
    DECLARE_AST_NODE(TypeSpecifier)
    TypeSpecifierNode();

    ~TypeSpecifierNode();
    void accept(Visitor *visitor)  override;
};

// TemplateTypeSpecifier : templateName < TypeSpecifierNode (, TypeSpecifierNode)* >
class TemplateTypeSpecifierNode : public TypeSpecifierNode
{
public:
    DECLARE_AST_NODE(TemplateTypeSpecifier)
    TemplateTypeSpecifierNode();
    SourceLocal templateName;
    QList<TypeSpecifierNode*> nodes;

    void accept(Visitor *visitor)  override;
};

// 叶子
class TypeNameNode final : public TypeSpecifierNode
{
public:
    DECLARE_AST_NODE(TypeName)
    TypeNameNode();
    ~TypeNameNode();

    void accept(Visitor *visitor)  override;
};

class TypeDefineNode : public Node
{
public:
    DECLARE_AST_NODE(TypeDefine)
    TypeDefineNode();
    ~TypeDefineNode();

    void accept(Visitor *visitor) override;

    SourceLocal let;
    TypeSpecifierNode* typeSpecifierNode;
    SourceLocal as;
    TypeNameNode* typeNameNode;
};

}
}
#endif // NODE_H
