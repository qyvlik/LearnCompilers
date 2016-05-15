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

class TypeSystem;
class TypeParser
{
public:
    bool start(TokenStream* lexerStream, TypeSystem* __typeSystem);

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
