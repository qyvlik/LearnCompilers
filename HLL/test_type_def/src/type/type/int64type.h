#ifndef INT64TYPE_H
#define INT64TYPE_H
#include "type.h"

namespace qyvlik {
namespace typer {


class Int64Type : public Type
{
    // Type interface
public:
    Int64Type():
        instantiable(TypeInstantiablePtr(new Int64Instantiable))
    {}

    QString typeName() const override
    { return "int64"; }

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



#endif // INT64TYPE_H
