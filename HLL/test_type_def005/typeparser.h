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

#include "context/typesystem.h"

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

class ITypeParser
{
public:
    bool start(TokenStream* lexerStream, TypeSystem* __typeSystem) {
        try {
            TypeDefinesStatement(lexerStream, __typeSystem);
            return true;
        } catch(const ParserError& error) {
            error.pritf();
            return false;
        }
    }

protected:
    void TypeDefinesStatement(TokenStream* lexerStream, TypeSystem* __typeSystem) throw(ParserError)
    {
        while(lexerStream->current().value() == "let") {
            TypeDefineStatement(lexerStream, __typeSystem);
            if(lexerStream->current().value() != ";") {
                throw ParserError(-1, "Lost `;");
            } else {
                lexerStream->next();
            }
        }
        qDebug() << "OK";
    }

    //  LET TypeSpecifier AS TypeName
    void TypeDefineStatement(TokenStream* lexerStream , TypeSystem* __typeSystem) throw(ParserError)
    {
        // current is `LET
        // read next
        lexerStream->next();

        // 这里指向下一个token
        TypeSpecifier(lexerStream, __typeSystem);

        lexerStream->next();

        if(lexerStream->current().value() == "as") {
            lexerStream->next();

            if(__typeSystem->isRegisterType(lexerStream->current().value())) {
                __typeSystem->registerType(lexerStream->current().value(),
                                           __typeSystem->getHelper()->takeCurrentArgumentTypeMetaData());
                __typeSystem->mapTypeName(lexerStream->current().value(),
                                          __typeSystem->getHelper()->takeFullTypeName());

            } else {
                throw ParserError(-1, "Type "+lexerStream->current().value()+" Already exist.");
            }
            TypeName(lexerStream, __typeSystem);
            lexerStream->next();
        }
    }

    // TypeSpecifier : TypeName | TemplateTypeSpecifier
    void TypeSpecifier(TokenStream* lexerStream, TypeSystem* __typeSystem) throw(ParserError)
    {
        if( __typeSystem->isTemplate(lexerStream->current().value())) {
            TemplateTypeSpecifier(lexerStream, __typeSystem);
        } else if( !__typeSystem->isRegisterType(lexerStream->current().value())) {
            TypeName(lexerStream, __typeSystem);

            __typeSystem->getHelper()->pushArgumentTypeMetaData(__typeSystem->getTypeMetaData(lexerStream->current().value()));
            __typeSystem->getHelper()->pushTypeToken(__typeSystem->getFullTypeName(lexerStream->current().value()));

        } else {
            throw ParserError(-1, "Type "+lexerStream->current().value()+" not exist.");
        }
    }


    // template < name (,name)* >
    void TemplateTypeSpecifier(TokenStream* lexerStream, TypeSystem* __typeSystem) throw(ParserError)
    {
        QString templateTypeName = lexerStream->current().value();

        const int templateArgsCount = __typeSystem->templateArgsCount(templateTypeName);

        Q_ASSERT(templateArgsCount > 0);

        QVector<TypeMetaData *> thizTemplateArguments;

        __typeSystem->getHelper()->newCurrentTemplateTypeName();


        __typeSystem->getHelper()->pushTypeToken(templateTypeName);

        lexerStream->next();
        if(lexerStream->current().value() == "<") {

            __typeSystem->getHelper()->pushTypeToken(lexerStream->current().value());

            lexerStream->next();
            TypeSpecifier(lexerStream, __typeSystem);     // `, or `>,

            lexerStream->next();            // `, or `>, read next
        } else {
            throw ParserError(-1, "Template Type " + templateTypeName+ " lost `< is ");
        }

        thizTemplateArguments.push_back(__typeSystem->getHelper()->takeCurrentArgumentTypeMetaData());

        switch(templateArgsCount)
        {
        case 1: {
            if(lexerStream->current().value() != ">") {
                throw ParserError(-1, "Template Type "+
                                  templateTypeName+
                                  " templateArgsCount is "+templateArgsCount);
            } else {

                __typeSystem->getHelper()->pushTypeToken(lexerStream->current().value());

                break;
            }
        }       // one template type argument

        default : {
            int commaCount = 0;
            // templateArgsCount-1 个 `,
            // (,TypeSpecifier)*
            if(lexerStream->current().value() == ",") {

                __typeSystem->getHelper()->pushTypeToken(lexerStream->current().value());

                commaCount = 1;
                do {
                    lexerStream->next();
                    TypeSpecifier(lexerStream, __typeSystem);

                    // __typeSystem->getHelper()->pushTypeToken(__typeSystem->getFullTypeName(lexerStream->current().value()));

                    lexerStream->next();                // `, or `>

                    commaCount++;
                    if(commaCount > templateArgsCount) {
                        throw ParserError(-1, "Template Type "+
                                          templateTypeName+
                                          " templateArgsCount is "+templateArgsCount);
                    }

                    thizTemplateArguments.push_back(__typeSystem->getHelper()->takeCurrentArgumentTypeMetaData());

                } while(lexerStream->current().value() == ",");

                if(lexerStream->current().value() == ">") {

                    __typeSystem->getHelper()->pushTypeToken(lexerStream->current().value());

                    break;
                }
            }
            throw ParserError(-2, "Template Type "+
                              templateTypeName+
                              " templateArgsCount is "+templateArgsCount);

        }       // two or more templte type arguemnts
        }   // switch

        TypeMetaData* templateTypeMeta = nullptr;
        TemplateTypeMetaDataFactory* templateTypeMetaData = nullptr;
        QString thizTemplateFullTypeName = __typeSystem->getHelper()->takeCurrentTemplateFullTypeName();
        if(__typeSystem->isRegisterType(thizTemplateFullTypeName)) {
            templateTypeMetaData = __typeSystem->getTemplateTypeMetaData(templateTypeName);
            templateTypeMeta =
                    templateTypeMetaData->templateTypeMetaData(thizTemplateArguments);
            __typeSystem->registerType(thizTemplateFullTypeName, templateTypeMeta);
        } else {
            templateTypeMeta = __typeSystem->getTypeMetaData(thizTemplateFullTypeName);
        }
        __typeSystem->getHelper()->pushArgumentTypeMetaData(templateTypeMeta);
        __typeSystem->getHelper()->fillTemplateArgument(thizTemplateFullTypeName);

    }

    void TypeName(TokenStream* lexerStream, TypeSystem* __typeSystem) throw(ParserError)
    {

        if(__typeSystem->isRegisterType(lexerStream->current().value())) {
            qDebug() << "Error type";
            throw ParserError(-1, "Type "+lexerStream->current().value()+" Not Exist.");
        } else if(__typeSystem->isTemplate(lexerStream->current().value())) {
            qDebug() << "base template type :" << lexerStream->current().value();
        } else {
            qDebug() << "base type :" << lexerStream->current().value();
        }
    }
};

}
} // namespace qyvlik::typer

#endif // TYPEPARSER_H
