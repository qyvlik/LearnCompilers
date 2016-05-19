#ifndef VISITOR_H
#define VISITOR_H

/**

LET
    : 'let'
    ;

COMMA
    : ','
    ;

AS
    : 'as'
    ;

LAngleBrackets
    : '<'
    ;

RAngleBrackets
    : '>'
    ;

TypeSpecifier
    : TypeName
    | TemplateTypeSpecifier
    ;

TypeDefine
    : LET TypeSpecifier AS TypeName
    ;

TypeDefines:
    : (TypeDefine)*
    ;

TemplateTypeSpecifier:
    : TemplateTypeName ( LAngleBrackets TypeSpecifier (COMMA TypeSpecifier)* RAngleBrackets )*
    ;

TemplateTypeName
    :
    ;

TypeName
    :
    ;
*/

#include <QMap>
#include <QString>
#include <QStack>

#include "../types/templatetypemetadatafactory.h"
#include "../types/typemetadata.h"

#include "../context/typesystemhelper_p.h"
#include "../context/typesystem.h"

namespace qyvlik {
namespace typer {

// 叶子节点就没有 Node 尾缀

class Node;
class TypeDefineNode;
class TypeSpecifierNode;
class TemplateTypeSpecifierNode;
class TypeNameNode;

class Visitor {
public:
    virtual ~Visitor();
    virtual void visit(Node*);
    virtual void visit(TypeDefinesNode*);
    virtual void visit(TypeDefineNode*);
    virtual void visit(TypeSpecifierNode*);
    virtual void visit(TemplateTypeSpecifierNode*);
    virtual void visit(TypeNameNode*);

};

class TypeSystemVisitor : public Visitor
{
public:
    TypeSystemVisitor();
    ~TypeSystemVisitor();
    void visit(Node *) override;
    void visit(TypeDefinesNode *) override;
    void visit(TypeDefineNode *) override;
    void visit(TypeSpecifierNode *) override;
    void visit(TemplateTypeSpecifierNode *) override;
    void visit(TypeNameNode *) override;

    TypeSystem* typeSystem;

};

}
}

#endif // VISITOR_H
