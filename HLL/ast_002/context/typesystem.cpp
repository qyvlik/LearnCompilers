#include "typesystem.h"
#include "typesystemhelper_p.h"

namespace qyvlik {
namespace typer {

TypeSystem::TypeSystem():
    helper(new TypeSystemHelper)
{
    unCompleteType.insert("stack", new StackTypeFactory());
    typeNameMap.insert("stack", "stack");

    unCompleteType.insert("map", new MapTypeFactory());
    typeNameMap.insert("map", "map");

    unCompleteType.insert("vector", new VectorTypeFactory());
    typeNameMap.insert("vector", "vector");


    completeType.insert("int8", new Int8TypeMetaData());
    typeNameMap.insert("int8", "int8");

    completeType.insert("int16", new Int16TypeMetaData());
    typeNameMap.insert("int16", "int16");

    Int32TypeMetaData* int32TypeMetaData = new Int32TypeMetaData();
    completeType.insert("int32", int32TypeMetaData);
    typeNameMap.insert("int32", "int32");

    completeType.insert("int", int32TypeMetaData);
    typeNameMap.insert("int", "int");

    completeType.insert("double", new DoubleTypeMetaData());
    typeNameMap.insert("double", "double");

    completeType.insert("string", new StringTypeMetaData());
    typeNameMap.insert("string", "string");
}

TypeSystem::~TypeSystem() {
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

void TypeSystem::registerType(const QString &name, TypeMetaData *metaData) {
    completeType.insert(name, metaData);
}

TemplateTypeMetaDataFactory *TypeSystem::getTemplateTypeMetaData(const QString &templateName) {
    auto find = unCompleteType.find(templateName);

    // Q_ASSERT(find != unCompleteType.end());
    if(find != unCompleteType.end()) {
        return find.value();
    } else {
        qDebug() << Q_FUNC_INFO << ": templateName " << templateName << " not found!";
        return nullptr;
    }
}

void TypeSystem::mapTypeName(const QString &name, const QString &fullName) {
    typeNameMap.insert(name, fullName);
}

QString TypeSystem::getFullTypeName(const QString &name) {
    auto find = typeNameMap.find(name);
    if(find != typeNameMap.end()) {
        return find.value();
    } else {
        return name;
    }
}

bool TypeSystem::isTemplate(const QString &name) {
    return unCompleteType.find(name) != unCompleteType.end();
}

bool TypeSystem::isRegisterType(const QString &name) {
    return (unCompleteType.find(name) == unCompleteType.end())
            &&
            (completeType.find(name) == completeType.end());
}

TypeMetaData *TypeSystem::getTypeMetaData(const QString &name) {
    auto find = completeType.find(name);
    if(find != completeType.end()) {
        return find.value();
    } else {
        return nullptr;
    }
}

int TypeSystem::templateArgsCount(const QString &templateName) {
    auto find = unCompleteType.find(templateName);
    if(find != unCompleteType.end()) {
        return find.value()->templateArgumentsCount();
    } else {
        return -2;
    }
}

TypeSystemHelper *TypeSystem::getHelper() const
{
    return helper;
}

}
}
