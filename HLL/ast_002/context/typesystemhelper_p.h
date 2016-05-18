#ifndef TYPESYSTEMHELPER_H
#define TYPESYSTEMHELPER_H

#include <QString>
#include <QStack>

namespace qyvlik {
namespace typer {

class TypeMetaData;
class TypeSystemHelper {

public:
    void newCurrentTemplateTypeName();

    void pushTypeToken(const QString& typeToken);

    QString takeCurrentTemplateFullTypeName();

    void fillTemplateArgument(const QString& thizTemplateFullTypeName);

    void pushArgumentTypeMetaData(TypeMetaData* argType);

    TypeMetaData* takeCurrentArgumentTypeMetaData();

    QString takeFullTypeName();

private:
    QStack<TypeMetaData*> typeBuffer;
    QString fullTypeName;
    QStack<QString> thizTemplateTypeNameStack;
};



}
}

#endif // TYPESYSTEMHELPER_H
