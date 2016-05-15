#ifndef TYPESYSTEM_H
#define TYPESYSTEM_H

#include <QtDebug>
#include <QStack>
#include <QMap>

#include "types/typeinstanciable.h"
#include "types/typemetadata.h"
#include "types/templatetypemetadatafactory.h"

namespace qyvlik {
namespace typer {

class TypeMetaData;

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

        // Q_ASSERT(find != unCompleteType.end());
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


}
}

#endif // TYPESYSTEM_H
