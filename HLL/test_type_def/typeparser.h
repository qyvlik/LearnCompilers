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
    : TypeName ( LAngleBrackets TypeSpecifier (COMMA TypeSpecifier)* RAngleBrackets )*
    ;

TypeDefine
    : LET TypeSpecifier AS TypeName
    ;

TypeDefines:
    : (TypeDefine)*
    ;
*/

#include <QtDebug>
#include <QStack>
#include "lexer.h"
#include "src/throwable/throwable.h"

#include "src/type/type/type.h"
#include "src/type/type/templatetype.h"
#include "src/type/type/int32type.h"


#include <map>

// 类型的元数据

namespace qyvlik {
namespace typer {

class ISymbol {
public:
    static QStringList baseTemplateTypeName;
    static QStringList baseTypeName ;
    static QSet<QString> customType;
};


class ParserError : public Error
{
public:
    ParserError(int code, const QString& message):
        Error(code, message)
    { }
};

class ITypeParser
{

public:
    bool start(TokenStream* lexerStream) {
        //Q_UNUSED(typeSystem)
        try {
            TypeDefinesStatement(lexerStream);
            return true;
        } catch(const ParserError& error) {
            error.pritf();
            return false;
        }
    }

    void TypeDefinesStatement(TokenStream* lexerStream) throw(ParserError);

    void TypeDefineStatement(TokenStream* lexerStream ) throw(ParserError);

    void TypeSpecifier(TokenStream* lexerStream) throw(ParserError);

    void TypeTemplateParams(TokenStream* lexerStream) throw(ParserError);

    void TypeName(TokenStream* lexerStream) throw(ParserError);

private:
    QString fullTypeName;
};

}
} // namespace qyvlik::lexer

#endif // TYPEPARSER_H
