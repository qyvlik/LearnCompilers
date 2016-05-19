#ifndef NODE_H
#define NODE_H

#include <QtDebug>

#include "error/typeerror.h"
#include "error/parsererror.h"

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
        Kind_TypeDefines,
        Kind_TypeDefine,
        Kind_TypeSpecifier,
        Kind_TemplateTypeSpecifier,
        Kind_TypeNameNode
    };

    virtual ~Node();
    virtual void accept(Visitor* visitor) throw(TypeError, ParserError)  = 0 ;

    int kind;
};



class TypeSpecifierNode : public Node
{
public:
    DECLARE_AST_NODE(TypeSpecifier)
    TypeSpecifierNode();

    ~TypeSpecifierNode();
    void accept(Visitor *visitor) throw(TypeError, ParserError)  override;
};

// TemplateTypeSpecifier : templateName < TypeSpecifierNode (, TypeSpecifierNode)* >
class TemplateTypeSpecifierNode : public TypeSpecifierNode
{
public:
    DECLARE_AST_NODE(TemplateTypeSpecifier)
    TemplateTypeSpecifierNode();
    QString templateName;
    QList<TypeSpecifierNode*> nodes;

    void accept(Visitor *visitor) throw(TypeError, ParserError)   override;
};

// 叶子
class TypeNameNode final : public TypeSpecifierNode
{
public:
    DECLARE_AST_NODE(TypeNameNode)
    TypeNameNode();
    ~TypeNameNode();

    void accept(Visitor *visitor) throw(TypeError, ParserError)  override;

    QString typeName;
};

class TypeDefineNode : public Node
{
public:
    DECLARE_AST_NODE(TypeDefine)
    TypeDefineNode();
    ~TypeDefineNode();

    void accept(Visitor *visitor) throw(TypeError, ParserError)  override;

    SourceLocal let;
    TypeSpecifierNode* typeSpecifierNode;
    SourceLocal as;
    TypeNameNode* typeNameNode;
};

class TypeDefinesNode : public Node
{
public:
    DECLARE_AST_NODE(TypeDefines)

    TypeDefinesNode();
    void accept(Visitor *visitor) throw(TypeError, ParserError)  override;

    QList<TypeDefineNode*> typeDefines;
};

}
}
#endif // NODE_H
