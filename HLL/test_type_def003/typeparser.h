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
    StackType(TypeMetaData* arg0):
        templateArg0(arg0)
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
        return new StackInstanciable(templateArg0->instanciable());
    }
private:
    TypeMetaData* templateArg0;
};

class TemplateTypeMetaDataFactory
{
public:
    virtual ~TemplateTypeMetaDataFactory()
    {}
    virtual QString templateTypeName()const = 0;
    virtual TypeMetaData* templateTypeMetaData(const QVector<TypeMetaData*>& templateArgs) =0;
    virtual int templateArgumentsCount() const = 0;
};

class StackTypeFactory: public TemplateTypeMetaDataFactory
{
public:
    ~StackTypeFactory(){}
    QString templateTypeName() const override
    {
        return "stack";
    }
    TypeMetaData *templateTypeMetaData(const QVector<TypeMetaData *> &templateArgs) override
    {
        Q_ASSERT(templateArgs.size() > 0 && templateArgs.size() == templateArgumentsCount());
        // qDebug() << Q_FUNC_INFO ;
        return new StackType(templateArgs.at(0));
    }

    int templateArgumentsCount()const override {
        return 1;
    }
};

/////////////////////////

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
        completeType.insert("int", new IntType);
        typeNameMap.insert("int", "int");
        typeNameMap.insert("stack", "stack");
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
        qDebug() << "name:" << name << ",fullTypeName:" << fullName;
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



class TypeTable
{
public:

    TypeTable()
    {
        unCompleteType.insert("stack", new StackTypeFactory());
        completeType.insert("int", new IntType);
        typeNameMap.insert("int", "int");
        typeNameMap.insert("stack", "stack");
    }

    ~TypeTable() {
//        qDebug() << "unCompleteType:";
//        auto i = unCompleteType.constBegin();
//        while (i != unCompleteType.constEnd()) {
//            qDebug() << i.key() << ": " << i.value();
//            ++i;
//        }

//        qDebug() << "completeType:";
//        auto ii = completeType.constBegin();
//        while (ii != completeType.constEnd()) {
//            qDebug() << ii.key() << ": " << ii.value();
//            ++ii;
//        }

//        qDebug() << "typeNameMap:";
//        auto iii = typeNameMap.constBegin();
//        while (iii != typeNameMap.constEnd()) {
//            qDebug() << iii.key() << ": " << iii.value();
//            ++iii;
//        }
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

    void nameMap(const QString& name, const QString& fullName) {
        qDebug() << "name:" << name << ",fullTypeName:" << fullName;
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

    // isTemplate
    // 第二参数是有几个参数的意思
    QMap<QString, TemplateTypeMetaDataFactory*> unCompleteType;
    QMap<QString, TypeMetaData*> completeType;
    QMap<QString, QString> typeNameMap;
};



class ITypeParser
{
public:
    TypeTable typeTable;

    bool start(TokenStream* lexerStream, TypeSystem* __typeSystem = nullptr) {
        try {
            TypeDefinesStatement(lexerStream, __typeSystem);
            return true;
        } catch(const ParserError& error) {
            error.pritf();
            return false;
        }
    }

    void TypeDefinesStatement(TokenStream* lexerStream, TypeSystem* __typeSystem = nullptr) throw(ParserError)
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
    void TypeDefineStatement(TokenStream* lexerStream , TypeSystem* __typeSystem = nullptr) throw(ParserError)
    {
        // current is `LET
        // read next
        lexerStream->next();

        // 这里指向下一个token
        TypeSpecifier(lexerStream, __typeSystem);

        lexerStream->next();

        if(lexerStream->current().value() == "as") {
            lexerStream->next();
            //! [0]
            if(__typeSystem->isRegisterType(lexerStream->current().value())) {
                __typeSystem->registerType(lexerStream->current().value(),
                                           __typeSystem->getHelper()->takeCurrentArgumentTypeMetaData());
                __typeSystem->mapTypeName(lexerStream->current().value(),
                                          __typeSystem->getHelper()->takeFullTypeName());
            }
            //! [0]

            //! [0]
//            if( typeTable.isRegisterType(lexerStream->current().value()) )
//            {
//                TypeMetaData* m = this->takeCurrentArgumentTypeMetaData();

//                typeTable.registerType(lexerStream->current().value(), m);
//                typeTable.nameMap(lexerStream->current().value(), this->takeFullTypeName());
//            }
            //! [0]

            else {
                // qDebug() << "Type " << lexerStream->current().value() << " Already exist";
                throw ParserError(-1, "Type "+lexerStream->current().value()+" Already exist.");
            }
            TypeName(lexerStream, __typeSystem);
            lexerStream->next();
        }
    }

    // TypeSpecifier : TypeName | TemplateTypeSpecifier
    void TypeSpecifier(TokenStream* lexerStream, TypeSystem* __typeSystem = nullptr) throw(ParserError)
    {
        if( typeTable.isTemplate(lexerStream->current().value())) {
            TemplateTypeSpecifier(lexerStream, __typeSystem);
            // get type entity
        } else if( !typeTable.isRegisterType(lexerStream->current().value())) {
            TypeName(lexerStream, __typeSystem);

            //! [1]
              __typeSystem->getHelper()->pushArgumentTypeMetaData(__typeSystem->getTypeMetaData(lexerStream->current().value()));
              __typeSystem->getHelper()->pushTypeToken(__typeSystem->getFullTypeName(lexerStream->current().value()));
            //! [1]

            //! [1]
//            // typeBuffer.push(typeTable.getTypeMetaData(lexerStream->current().value()));
//            this->pushArgumentTypeMetaData(typeTable.getTypeMetaData(lexerStream->current().value()));
//            // thizTemplateTypeNameStack.top().push_back(lexerStream->current().value());
//            // fullTypeName.push_back(lexerStream->current().value());
//            this->pushTypeToken(typeTable.getFullTypeName(lexerStream->current().value()));
            //! [1]

        } else {
            throw ParserError(-1, "Type "+lexerStream->current().value()+" not exist.");
        }
    }


    // template < name (,name)* >
    void TemplateTypeSpecifier(TokenStream* lexerStream, TypeSystem* __typeSystem = nullptr) throw(ParserError)
    {
        QString templateTypeName = lexerStream->current().value();
        const int templateArgsCount = typeTable.templateArgsCount(templateTypeName);

        Q_ASSERT(templateArgsCount > 0);

        //! [2]
         __typeSystem->getHelper()->newCurrentTemplateTypeName();
        //! [2]

        //! [2]
        // thizTemplateTypeNameStack.push("");
//        this->newCurrentTemplateTypeName();
        //! [2]



        QVector<TypeMetaData *> thizTemplateArguments;

        //! [3]
         __typeSystem->getHelper()->pushTypeToken(templateTypeName);
        //! [3]

        //! [3]
        // fullTypeName.push_back(templateTypeName);
        // thizTemplateTypeNameStack.top().push_back(templateTypeName);
//        this->pushTypeToken(templateTypeName);
        //! [3]

        lexerStream->next();
        if(lexerStream->current().value() == "<") {

            //! [4]
             __typeSystem->getHelper()->pushTypeToken(lexerStream->current().value());
            //! [4]

            //! [4]
            // fullTypeName.push_back(lexerStream->current().value());
            // thizTemplateTypeNameStack.top().push_back(lexerStream->current().value());
//            this->pushTypeToken(lexerStream->current().value());
            //! [4]

            lexerStream->next();
            TypeSpecifier(lexerStream, __typeSystem);     // `, or `>,

            lexerStream->next();            // `, or `>, read next
        } else {
            throw ParserError(-1, "Template Type " + templateTypeName+ " lost `< is ");
        }

        //! [5]
         thizTemplateArguments.push_back(__typeSystem->getHelper()->takeCurrentArgumentTypeMetaData());
        //! [5]

        //! [5]
        // thizTemplateArguments.push_back(typeBuffer.pop());
//        thizTemplateArguments.push_back(this->takeCurrentArgumentTypeMetaData());
        //! [5]

        switch(templateArgsCount)
        {
        case 1: {
            if(lexerStream->current().value() != ">") {
                throw ParserError(-1, "Template Type "+
                                  templateTypeName+
                                  " templateArgsCount is "+templateArgsCount);
            } else {

                //! [6]
                 __typeSystem->getHelper()->pushTypeToken(lexerStream->current().value());
                //! [6]

                //! [6]
                // fullTypeName.push_back(lexerStream->current().value());
                // thizTemplateTypeNameStack.top().push_back(lexerStream->current().value());
//                this->pushTypeToken(lexerStream->current().value());
                //! [6]

                break;
            }
        }       // one template type argument

        default : {
            int commaCount = 0;
            // templateArgsCount-1 个 `,
            // (,TypeSpecifier)*
            if(lexerStream->current().value() == ",") {

                //! [7]
                 __typeSystem->getHelper()->pushTypeToken(lexerStream->current().value());
                //! [7]

                //! [7]
                // fullTypeName.push_back(lexerStream->current().value());
                // thizTemplateTypeNameStack.top().push_back(lexerStream->current().value());
//                this->pushTypeToken(lexerStream->current().value());
                //! [7]

                commaCount = 1;
                do {
                    lexerStream->next();
                    TypeSpecifier(lexerStream, __typeSystem);

                    //! [8]
                     __typeSystem->getHelper()->pushTypeToken(__typeSystem->getFullTypeName(lexerStream->current().value()));
                    //! [8]

                    //! [8]
                    // fullTypeName.push_back(typeTable.getFullTypeName(lexerStream->current().value()));
                    // thizTemplateTypeNameStack.top().push_back(typeTable.getFullTypeName(lexerStream->current().value()));
//                    this->pushTypeToken(typeTable.getFullTypeName(lexerStream->current().value()));
                    //! [8]

                    lexerStream->next();                // `, or `>

                    commaCount++;
                    if(commaCount > templateArgsCount) {
                        throw ParserError(-1, "Template Type "+
                                          templateTypeName+
                                          " templateArgsCount is "+templateArgsCount);
                    }

                } while(lexerStream->current().value() == ",");

                if(lexerStream->current().value() == ">") {

                    //! [9]
                     __typeSystem->getHelper()->pushTypeToken(lexerStream->current().value());
                    //! [9]

                    //! [9]
                    // fullTypeName.push_back(lexerStream->current().value());
                    // thizTemplateTypeNameStack.top().push_back(lexerStream->current().value());
//                    this->pushTypeToken(lexerStream->current().value());
                    //! [9]

                    break;
                }
            }
            throw ParserError(-2, "Template Type "+
                              templateTypeName+
                              " templateArgsCount is "+templateArgsCount);

        }       // two or more templte type arguemnts
        }   // switch
        // push

        //        QString thizTemplateFullTypeName = thizTemplateTypeNameStack.pop();




        //! [10]
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
        //! [10]

        //! [10]
//        TypeMetaData* templateTypeMeta = nullptr;
//        TemplateTypeMetaDataFactory* templateTypeMetaData = nullptr;
//        QString thizTemplateFullTypeName = this->takeCurrentTemplateFullTypeName();

//        if(typeTable.isRegisterType(thizTemplateFullTypeName)) {
//            // 还没有注册过
//            templateTypeMetaData = typeTable.getTemplateTypeMetaData(templateTypeName);
//            templateTypeMeta =
//                    templateTypeMetaData->templateTypeMetaData(thizTemplateArguments);
//            typeTable.registerType(thizTemplateFullTypeName, templateTypeMeta);

//            qDebug() << "thizTemplateFullTypeName:" << thizTemplateFullTypeName << " not exist";
//        } else {
//            // 已经注册过了
//            templateTypeMeta = typeTable.getTypeMetaData(thizTemplateFullTypeName);

//            qDebug() << "thizTemplateFullTypeName:" << thizTemplateFullTypeName << " Already exist. "
//                     << "templateTypeMeta: " << templateTypeMeta;
//        }

//        // typeBuffer.push(templateTypeMeta);
//        this->pushArgumentTypeMetaData(templateTypeMeta);

//        // if(!thizTemplateTypeNameStack.isEmpty()) {
//        //    thizTemplateTypeNameStack.top().push_back(thizTemplateFullTypeName);
//        // }
//        this->fillTemplateArgument(thizTemplateFullTypeName);
        //! [10]

    }

    void TypeName(TokenStream* lexerStream, TypeSystem* __typeSystem = nullptr) throw(ParserError)
    {
        Q_UNUSED(__typeSystem);

        if(typeTable.isRegisterType(lexerStream->current().value())) {
            // qDebug() << "Error type";
            throw ParserError(-1, "Type "+lexerStream->current().value()+" Not Exist.");
        } else if(typeTable.isTemplate(lexerStream->current().value())) {
            //qDebug() << "base template type :" << lexerStream->current().value();
        } else {
            //qDebug() << "base type :" << lexerStream->current().value();
        }
        // push
        //        qDebug() <<lexerStream->current().value()
        //                << " getTypeMetaData"
        //                << typeTable.getTypeMetaData(lexerStream->current().value());
    }

private:
    QStack<TypeMetaData*> typeBuffer;
    QString fullTypeName;
    QStack<QString> thizTemplateTypeNameStack;

    QString takeFullTypeName() {
        QString t = fullTypeName;
        fullTypeName.clear();
        return t;
    }

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
};

}
} // namespace qyvlik::lexer

#endif // TYPEPARSER_H
