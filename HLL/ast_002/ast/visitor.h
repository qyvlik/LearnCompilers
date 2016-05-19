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
    virtual void visit(Node*) throw(TypeError, ParserError) ;
    virtual void visit(TypeDefinesNode*) throw(TypeError, ParserError) ;
    virtual void visit(TypeDefineNode*) throw(TypeError, ParserError) ;
    virtual void visit(TypeSpecifierNode*) throw(TypeError, ParserError) ;
    virtual void visit(TemplateTypeSpecifierNode*) throw(TypeError, ParserError) ;
    virtual void visit(TypeNameNode*) throw(TypeError, ParserError) ;

};

class TypeSystemVisitor : public Visitor
{
public:
    TypeSystemVisitor();
    ~TypeSystemVisitor();
    void visit(Node *) throw(TypeError, ParserError)  override;
    void visit(TypeDefinesNode *) throw(TypeError, ParserError)  override;
    void visit(TypeDefineNode *) throw(TypeError, ParserError)  override;
    void visit(TypeSpecifierNode *) throw(TypeError, ParserError)  override;
    void visit(TemplateTypeSpecifierNode *) throw(TypeError, ParserError)  override;
    void visit(TypeNameNode *) throw(TypeError, ParserError)  override;

    TypeSystem* typeSystem;

};

}
}

#endif // VISITOR_H
