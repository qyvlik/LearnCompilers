#ifndef INT32TYPE_H
#define INT32TYPE_H

#include "type.h"

namespace qyvlik {
namespace typer {


class Int32Type : public Type
{
    // Type interface
public:
    Int32Type():
        instantiable(TypeInstantiablePtr(new Int32Instantiable))
    {}

    QString typeName() const override
    { return "int32"; }

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

#endif // INT32TYPE_H
