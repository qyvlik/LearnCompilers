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
#include "types/typeinstanciable.h"
#include "types/typemetadata.h"
#include "types/templatetypemetadatafactory.h"

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

class TypeSystemHelper {

public:
    void newCurrentTemplateTypeName() {
        thizTemplateTypeNameStack.push("");
    }

    void pushTypeToken(const QString& typeToken) {
        fullTypeName.push_back(typeToken);
        thizTemplateTypeNameStack.top().push_back(typeToken);
    }

    QString takeCurrentTemplateFullTypeName() {
        return thizTemplateTypeNameStack.pop();
    }

    void fillTemplateArgument(const QString& thizTemplateFullTypeName) {
        if(!thizTemplateTypeNameStack.isEmpty()) {
            thizTemplateTypeNameStack.top().push_back(thizTemplateFullTypeName);
        }
    }

    void pushArgumentTypeMetaData(TypeMetaData* argType) {
        typeBuffer.push(argType);
    }

    TypeMetaData* takeCurrentArgumentTypeMetaData() {
        // TODO
        qDebug() << Q_FUNC_INFO;
        return typeBuffer.pop();
    }

    QString takeFullTypeName() {
        QString t = fullTypeName;
        fullTypeName.clear();
        return t;
    }

private:
    QStack<TypeMetaData*> typeBuffer;
    QString fullTypeName;
    QStack<QString> thizTemplateTypeNameStack;
};

class TypeSystem {
public:
    TypeSystem():
        helper(new TypeSystemHelper)
    {
        unCompleteType.insert("stack", new StackTypeFactory());
        typeNameMap.insert("stack", "stack");

        unCompleteType.insert("map", new MapTypeFactory());
        typeNameMap.insert("map", "map");

        completeType.insert("int", new BaseTypeMetaData<int>());
        typeNameMap.insert("int", "int");

        completeType.insert("double", new BaseTypeMetaData<double>());
        typeNameMap.insert("double", "double");
    }

    virtual ~TypeSystem() {
        qDebug() << Q_FUNC_INFO;

        delete helper;

        qDebug() << "unCompleteType:";
        auto i = unCompleteType.constBegin();
        while (i != unCompleteType.constEnd()) {
            qDebug() << i.key() << ": " << i.value();
            ++i;
        }

        qDebug() << "completeType:";
        auto ii = completeType.constBegin();
        while (ii != completeType.constEnd()) {
            qDebug() << ii.key() << ": " << ii.value();
            ++ii;
        }

        qDebug() << "typeNameMap:";
        auto iii = typeNameMap.constBegin();
        while (iii != typeNameMap.constEnd()) {
            qDebug() << iii.key() << ": " << iii.value();
            ++iii;
        }
    }


    void registerType(const QString &name, TypeMetaData* metaData) {
        completeType.insert(name, metaData);
    }

    TemplateTypeMetaDataFactory* getTemplateTypeMetaData(const QString& templateName) {
        auto find = unCompleteType.find(templateName);

        Q_ASSERT(find != unCompleteType.end());
        if(find != unCompleteType.end()) {
            return find.value();
        } else {
            qDebug() << Q_FUNC_INFO << ": templateName " << templateName << " not found!";
            return nullptr;
        }
    }

    void mapTypeName(const QString& name, const QString& fullName) {
        typeNameMap.insert(name, fullName);
    }

    QString getFullTypeName(const QString& name) {
        auto find = typeNameMap.find(name);
        if(find != typeNameMap.end()) {
            return find.value();
        } else {
            return name;
        }
    }

    bool isTemplate(const QString& name) {
        return unCompleteType.find(name) != unCompleteType.end();
    }

    bool isRegisterType(const QString& name) {
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
            return find.value()->templateArgumentsCount();
        } else {
            return -2;
        }
    }

    TypeSystemHelper *getHelper() const
    {
        return helper;
    }

protected:
    QMap<QString, TemplateTypeMetaDataFactory*> unCompleteType;
    QMap<QString, TypeMetaData*> completeType;
    QMap<QString, QString> typeNameMap;
    TypeSystemHelper* helper;
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
