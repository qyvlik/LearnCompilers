#ifndef STRINGTYPE_H
#define STRINGTYPE_H

#include "type.h"

namespace qyvlik {
namespace typer {


class StringType : public Type
{
    // Type interface
public:
    StringType():
        instantiable(TypeInstantiablePtr(new StringInstantiable))
    {}

    QString typeName() const override
    { return "string"; }

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

#endif // STRINGTYPE_H
