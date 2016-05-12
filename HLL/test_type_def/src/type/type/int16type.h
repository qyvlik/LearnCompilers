#ifndef INT16TYPE_H
#define INT16TYPE_H

#include "type.h"

namespace qyvlik {
namespace typer {

class Int16Type : public Type
{
public:
    Int16Type():
        instantiable(TypeInstantiablePtr(new Int16Instantiable))
    {}

    QString typeName() const override
    { return "int16"; }

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
#endif // INT16TYPE_H
