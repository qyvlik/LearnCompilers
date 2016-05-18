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
    virtual void visit(TypeDefineNode*);
    virtual void visit(TypeSpecifierNode*);
    virtual void visit(TemplateTypeSpecifierNode*);
    virtual void visit(TypeNameNode*);
};

}
}

#endif // VISITOR_H
