#ifndef DOUBLETYPE_H
#define DOUBLETYPE_H

#include "type.h"

namespace qyvlik {
namespace typer {

class DoubleType : public Type
{
public:
    DoubleType():
        instantiable(TypeInstantiablePtr(new DoubleInstantiable))
    {}

    QString typeName() const override
    { return "double"; }

    TypeWeakPtr baseType() const override
    { return TypeWeakPtr(); }

    TypeInstantiablePtr typeInstantiable() const override
    { return instantiable; }

    bool isTemplate() const override
    { return false; }
private:
    TypeInstantiablePtr instantiable;
};

}
}
#endif // DOUBLETYPE_H
