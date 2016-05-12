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


class TypeInstanciable {
public:
    virtual ~TypeInstanciable(){}
    virtual int newInstance() = 0;
};

class TypeMetaData
{
public:
    virtual ~TypeMetaData() {}
    virtual int templateArgsCount() const = 0;
    virtual bool isTemplate() const = 0;
    virtual TypeInstanciable* instanciable() const = 0;
};


class IntInstanciable : public TypeInstanciable
{
public:
    int newInstance() override
    {
        qDebug() << Q_FUNC_INFO ;
        return 0;
    }
};

class IntType : public TypeMetaData
{
public:
    int templateArgsCount() const override
    {
        return 0;
    }
    bool isTemplate() const override
    {
        return false;
    }

    TypeInstanciable* instanciable() const {
        return new IntInstanciable();
    }
};

class StackInstanciable : public TypeInstanciable
{

public:
    StackInstanciable(TypeInstanciable* typeArg0):
        templateTypeArg0(typeArg0)
    {}

    int newInstance() override
    {
        templateTypeArg0->newInstance();
        qDebug() << Q_FUNC_INFO ;
        return 0;
    }

    TypeInstanciable* templateTypeArg0;
};

class StackType : public TypeMetaData
{
public:
    StackType(TypeInstanciable* arg0):
        arg0Instanciable(arg0)
    {}

    int templateArgsCount() const override
    {
        return 1;
    }
    bool isTemplate() const override
    {
        return true;
    }

    TypeInstanciable* instanciable() const {
        return new StackInstanciable(arg0Instanciable);
    }
private:
    TypeInstanciable* arg0Instanciable;
};

/////////////////////////

class ParserError : public Error
{
public:
    ParserError(int code, const QString& message):
        Error(code, message)
    { }
};


class TypeTable
{
public:

    TypeTable()
    {
        unCompleteType.insert("stack", 1);
        completeType.insert("int", new IntType);
    }

    void registerType(const QString &name, TypeMetaData* metaData) {
        completeType.insert(name, metaData);
    }

    void registerType(const QString& name, const int argsCount) {

        Q_UNUSED(argsCount)
        // TODO
        completeType.insert(name,  new IntType);
    }

    bool isTemplate(const QString& name) {
        return unCompleteType.find(name) != unCompleteType.end();
    }

    bool typeNotExist(const QString& name) {
        return (unCompleteType.find(name) == unCompleteType.end())
                &&
                (completeType.find(name) == completeType.end());
    }

    TypeMetaData* getTypeMetaData(const QString& name) {
        auto find = completeType.find(name);
        if(find != completeType.end()) {
            return find.value();
        } else {
            return nullptr;
        }
    }

    int templateArgsCount(const QString& templateName) {
        auto find = unCompleteType.find(templateName);
        if(find != unCompleteType.end()) {
            return find.value();
        } else {
            return -2;
        }
    }

    // isTemplate
    // 第二参数是有几个参数的意思
    QMap<QString, int> unCompleteType;
    QMap<QString, TypeMetaData*> completeType;
};


class ITypeParser
{
public:
    TypeTable typeTable;

    QStack<TypeMetaData*> typeBuffer;

    bool start(TokenStream* lexerStream) {
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
            // get type entity
        } else if( !typeTable.typeNotExist(lexerStream->current().value())) {
            TypeName(lexerStream);
            // get type entity
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

        QVector<TypeMetaData*> templateArguments;

        lexerStream->next();
        if(lexerStream->current().value() == "<") {
            lexerStream->next();
            TypeSpecifier(lexerStream);
            // get type entity
            // TODO
            templateArguments.push_back(typeBuffer.pop());
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
                    // TODO
                    templateArguments.push_back(typeBuffer.pop());
                    // get type entity
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
        // push

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
        // push
        qDebug() << "getTypeMetaData" << typeTable.getTypeMetaData(lexerStream->current().value());
        typeBuffer.push(typeTable.getTypeMetaData(lexerStream->current().value()));
    }
};

}
} // namespace qyvlik::lexer

#endif // TYPEPARSER_H
