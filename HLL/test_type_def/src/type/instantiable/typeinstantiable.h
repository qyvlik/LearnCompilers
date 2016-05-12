#ifndef TYPEINSTANTIABLE_H
#define TYPEINSTANTIABLE_H

#include <QDebug>
#include <memory>

#include "../value/valueproxy.h"

namespace qyvlik {
namespace typer {

class TypeInstantiable {
public:
    virtual ~TypeInstantiable() = 0;
    virtual ValueProxy* newInstance() const = 0;
//protected:
//    virtual ValueProxy* defaultValue() const {return nullptr;}
//    virtual ValueProxy* newInstance(const QList<ValueProxy*>& args) const { return nullptr; }
};

typedef std::shared_ptr<TypeInstantiable> TypeInstantiablePtr;

typedef std::weak_ptr<TypeInstantiable>   TypeInstantiableWeakPtr;


}
}

#endif // TYPEINSTANTIABLE_H
