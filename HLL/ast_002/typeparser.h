#ifndef TYPEPARSER_H
#define TYPEPARSER_H

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

#include <QtDebug>
#include <QStack>
#include <QMap>

#include "token.h"
#include "throwable.h"

// 类型的元数据

namespace qyvlik {
namespace typer {

class ParserError : public Error
{
public:
    ParserError(int code, const QString& message):
        Error(code, message)
    { }
};

class TypeDefinesNode;
class TypeDefineNode;
class TypeSpecifierNode;
class TemplateTypeSpecifierNode;
class TypeNameNode;
class Node;

class ASTTree {
public:
    ASTTree();
    TypeDefinesNode* createTypeDefinesNode(const QList<TypeDefineNode*>& typeDefines);

    TypeDefineNode* createTypeDefineNode( TypeSpecifierNode* typeSpecifierNode, TypeNameNode* typeNameNode);

    // TypeSpecifierNode -> TemplateTypeSpecifierNode | TypeName
    TypeSpecifierNode* createTypeSpecifierNode(TypeSpecifierNode* node);

    TemplateTypeSpecifierNode* createTemplateTypeSpecifierNode(const QList<TypeSpecifierNode*>& nodes);

    TypeNameNode* createTypeName();

    Node* astNode;
    QStack<Node*> nodeVar;
};

class TypeSystem;
class TypeParser
{
public:
    TypeParser();
    bool start(TokenStream* lexerStream, TypeSystem* __typeSystem = 0);
    ASTTree* astTree;

protected:
    void TypeDefinesStatement(TokenStream* lexerStream, TypeSystem* __typeSystem) throw(ParserError);

    //  LET TypeSpecifier AS TypeName
    void TypeDefineStatement(TokenStream* lexerStream , TypeSystem* __typeSystem) throw(ParserError);

    // TypeSpecifier : TypeName | TemplateTypeSpecifier
    void TypeSpecifier(TokenStream* lexerStream, TypeSystem* __typeSystem) throw(ParserError);

    // template < name (,name)* >
    void TemplateTypeSpecifier(TokenStream* lexerStream, TypeSystem* __typeSystem) throw(ParserError);

    void TypeName(TokenStream* lexerStream, TypeSystem* __typeSystem) throw(ParserError);


};

}
} // namespace qyvlik::typer

#endif // TYPEPARSER_H
