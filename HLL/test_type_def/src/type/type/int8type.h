#ifndef INT8TYPE_H
#define INT8TYPE_H

#include "type.h"

namespace qyvlik {
namespace typer {

class Int8Type : public Type
{

public:
    Int8Type():
        instantiable(TypeInstantiablePtr(new Int8Instantiable))
    {}

    QString typeName() const override
    { return "int8"; }

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
#endif // INT8TYPE_H
