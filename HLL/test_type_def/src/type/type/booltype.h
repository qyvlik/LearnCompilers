#ifndef BOOLTYPE_H
#define BOOLTYPE_H

#include "type.h"

namespace qyvlik {
namespace typer {


class BoolType : public Type
{
public:
public:
    BoolType():
        instantiable(TypeInstantiablePtr(new BoolInstantiable))
    {}

    QString typeName() const override
    { return "bool"; }

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

#endif // BOOLTYPE_H
