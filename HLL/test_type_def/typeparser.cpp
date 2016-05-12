#include "typeparser.h"

namespace qyvlik {
namespace typer {


QStringList ISymbol::baseTemplateTypeName = std::initializer_list<QString>{"vector", "map", "stack", "function"};

QStringList ISymbol::baseTypeName = std::initializer_list<QString>{"int", "real", "bool", "string"};

QSet<QString> ISymbol::customType;

void ITypeParser::TypeDefinesStatement(TokenStream *lexerStream) throw(ParserError)
{
    while(lexerStream->current().value() == "let") {
        TypeDefineStatement(lexerStream);
        if(lexerStream->current().value() != ";") {
            throw ParserError(-1, "Lost `;");
        } else {
            lexerStream->next();
        }
    }
    qDebug() << "OK";
}

// start of `let
void ITypeParser::TypeDefineStatement(TokenStream *lexerStream)  throw(ParserError)
{
    lexerStream->next();

    TypeSpecifier(lexerStream);

    if(lexerStream->current().value() == "as") {
        lexerStream->next();
        if(
                !ISymbol::customType.contains(lexerStream->current().value())
                && !ISymbol::baseTypeName.contains(lexerStream->current().value())
                && !ISymbol::baseTemplateTypeName.contains(lexerStream->current().value())
                )
        {
            // TODO
            ISymbol::customType.insert(lexerStream->current().value());

        } else {

            // qDebug() << "Type " << lexerStream->current().value() << " Already exist";
            throw ParserError(-1, "Type "+lexerStream->current().value()+" Already exist.");
        }
        TypeName(lexerStream);
        lexerStream->next();
    }
}


// start of `TypeName
void ITypeParser::TypeSpecifier(TokenStream *lexerStream) throw(ParserError)
{
    TypeName(lexerStream);


    // TODO 这里获取类型名，查询是否重复插入
    QString _current = lexerStream->current().value();
    bool _isTemplatetype = ISymbol::baseTemplateTypeName.contains(_current);


    lexerStream->next();

    if(lexerStream->current().value() == "<") {

        if(!_isTemplatetype) {
            throw ParserError(-1, "Not A Template Type");
        }

        lexerStream->next();
        TypeSpecifier(lexerStream);

        if(lexerStream->current().value() == ",") {

            TypeTemplateParams(lexerStream);
        } else if(lexerStream->current().value() == ">") {

            lexerStream->next();
        }
    } else if(lexerStream->current().value() == ",") {

        TypeTemplateParams(lexerStream);

    } else if(lexerStream->current().value() == ">") {

        lexerStream->next();
    }
}

// start of `,
void ITypeParser::TypeTemplateParams(TokenStream *lexerStream) throw(ParserError)
{
    lexerStream->next();                    // current is `,
    do {
        TypeSpecifier(lexerStream);
    } while(lexerStream->current().value() == ",");
}

void ITypeParser::TypeName(TokenStream *lexerStream) throw(ParserError)
{
    if(ISymbol::baseTypeName.contains(lexerStream->current().value())) {
        qDebug() << "base type :" << lexerStream->current().value();

    } else if(ISymbol::baseTemplateTypeName.contains(lexerStream->current().value())) {
        qDebug() << "base template type :" << lexerStream->current().value();

    } else if(ISymbol::customType.contains(lexerStream->current().value())) {
        qDebug() << "custom type :" << lexerStream->current().value();

    } else {
        // qDebug() << "Error type";
        throw ParserError(-1, "Type "+lexerStream->current().value()+" Not Exist.");
    }
}









}
} // namespace qyvlik::lexer

