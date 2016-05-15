#ifndef TYPESYSTEM_H
#define TYPESYSTEM_H

#include <QtDebug>
#include <QStack>
#include <QMap>

#include "types/buildintype.h"

namespace qyvlik {
namespace typer {

class TypeMetaData;
class TypeSystemHelper;

class TypeSystem {
public:
    TypeSystem();

    virtual ~TypeSystem();


    void registerType(const QString &name, TypeMetaData* metaData);

    TemplateTypeMetaDataFactory* getTemplateTypeMetaData(const QString& templateName);

    void mapTypeName(const QString& name, const QString& fullName);

    QString getFullTypeName(const QString& name);

    bool isTemplate(const QString& name);

    bool isRegisterType(const QString& name);

    TypeMetaData* getTypeMetaData(const QString& name);

    int templateArgsCount(const QString& templateName);

    TypeSystemHelper *getHelper() const;

protected:
    QMap<QString, TemplateTypeMetaDataFactory*> unCompleteType;
    QMap<QString, TypeMetaData*> completeType;
    QMap<QString, QString> typeNameMap;
    TypeSystemHelper* helper;
};


}
}

#endif // TYPESYSTEM_H
