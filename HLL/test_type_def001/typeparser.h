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

#include "lexer.h"
#include "throwable.h"

// 类型的元数据

namespace qyvlik {
namespace typer {

class ISymbol {
public:
    static QStringList baseTemplateTypeName;
    static QStringList baseTypeName ;
    static QSet<QString> customType;
};


class TypeTable
{
public:
    TypeTable()
    {
        unCompleteType.insert("stack", 1);
        unCompleteType.insert("vector", 1);
        unCompleteType.insert("map", 2);

        unCompleteType.insert("function", 3);

        completeType.insert("bool", 0);
        completeType.insert("int", 0);
        completeType.insert("double", 0);
        completeType.insert("string", 0);
    }

    // isTemplate
    // 第二参数是有几个参数的意思
    QMap<QString, int> unCompleteType;
    QMap<QString, int> completeType;

    void registerType(const QString& name, const int argsCount) {
        completeType.insert(name, argsCount);
    }

    bool isTemplate(const QString& name) {
        return unCompleteType.find(name) != unCompleteType.end();
    }

    bool typeNotExist(const QString& name) {
        return (unCompleteType.find(name) == unCompleteType.end())
                &&
                (completeType.find(name) == completeType.end());
    }

    int templateArgsCount(const QString& templateName) {
        auto find = unCompleteType.find(templateName);
        if(find != unCompleteType.end()) {
            return find.value();
        } else {
            return -2;
        }
    }

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
    TypeTable typeTable;
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

    void TypeDefinesStatement(TokenStream* lexerStream) throw(ParserError)
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

    //  LET TypeSpecifier AS TypeName
    void TypeDefineStatement(TokenStream* lexerStream ) throw(ParserError)
    {
        // current is `LET
        // read next
        lexerStream->next();

        // 这里指向下一个token
        TypeSpecifier(lexerStream);

        lexerStream->next();

        if(lexerStream->current().value() == "as") {
            lexerStream->next();
            if( typeTable.typeNotExist(lexerStream->current().value()) )
            {
                typeTable.registerType(lexerStream->current().value(), 0);
            } else {

                // qDebug() << "Type " << lexerStream->current().value() << " Already exist";
                throw ParserError(-1, "Type "+lexerStream->current().value()+" Already exist.");
            }
            TypeName(lexerStream);
            lexerStream->next();
        }
    }

    // TypeSpecifier : TypeName | TemplateTypeSpecifier
    void TypeSpecifier(TokenStream* lexerStream) throw(ParserError)
    {
        if( typeTable.isTemplate(lexerStream->current().value())) {
            TemplateTypeSpecifier(lexerStream);
        } else if( !typeTable.typeNotExist(lexerStream->current().value())) {
            TypeName(lexerStream);
        } else {
            throw ParserError(-1, "Type "+lexerStream->current().value()+" not exist.");
        }
    }


    // template < name (,name)* >
    void TemplateTypeSpecifier(TokenStream* lexerStream) throw(ParserError)
    {
        QString templateTypeName = lexerStream->current().value();
        const int templateArgsCount = typeTable.templateArgsCount(templateTypeName);

        //
        Q_ASSERT(templateArgsCount > 0);

        lexerStream->next();
        if(lexerStream->current().value() == "<") {
            lexerStream->next();
            TypeSpecifier(lexerStream);
            lexerStream->next();            // `, or `>
        } else {
            throw ParserError(-1, "Template Type " + templateTypeName+
                              " lost `< is ");
        }

        switch(templateArgsCount)
        {
        case 1: {
            if(lexerStream->current().value() != ">") {
                throw ParserError(-1, "Template Type "+
                                  templateTypeName+
                                  " templateArgsCount is "+templateArgsCount);
            } else {
                return;
            }
        }       // one template type argument

        default : {
            int commaCount = 0;
            // templateArgsCount-1 个 `,
            // (,TypeSpecifier)*
            if(lexerStream->current().value() == ",") {
                commaCount = 1;
                do {
                    lexerStream->next();
                    TypeSpecifier(lexerStream);
                    lexerStream->next();                // `, or `>

                    commaCount++;
                    if(commaCount > templateArgsCount) {
                        throw ParserError(-1, "Template Type "+
                                          templateTypeName+
                                          " templateArgsCount is "+templateArgsCount);
                    }

                } while(lexerStream->current().value() == ",");

                if(lexerStream->current().value() == ">") {
                    return ;
                }
            }
            throw ParserError(-2, "Template Type "+
                              templateTypeName+
                              " templateArgsCount is "+templateArgsCount);

        }       // two or more templte type arguemnts
        }
    }

    void TypeName(TokenStream* lexerStream) throw(ParserError)
    {
        if(typeTable.typeNotExist(lexerStream->current().value())) {
            // qDebug() << "Error type";
            throw ParserError(-1, "Type "+lexerStream->current().value()+" Not Exist.");
        } else if(typeTable.isTemplate(lexerStream->current().value())) {
            qDebug() << "base template type :" << lexerStream->current().value();
        } else {
            qDebug() << "base type :" << lexerStream->current().value();
        }
    }
};

}
} // namespace qyvlik::lexer

#endif // TYPEPARSER_H
