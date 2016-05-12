#ifndef TYPE_H
#define TYPE_H


#include <QList>
#include <QMap>
#include <QDebug>

#include "src/type/instantiable/typeinstantiable.h"
#include "src/type/type/templatetype.h"

namespace qyvlik {
namespace lexer {


class TypeInstantiableFactory {
public:
    TypeInstantiableFactory();
    ~TypeInstantiableFactory();

    void registerBaseType();

    TypeInstantiablePtr registerType(const QString& name, TypeInstantiablePtr instancer);

    TypeInstantiablePtr getTypeInstantiable(const QString& name);
private:
    // full type name
    QMap<QString, TypeInstantiablePtr> instancers;
};





}
}


#endif // TYPE_H
