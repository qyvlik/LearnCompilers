#ifndef STACKINSTANTIABLE_H
#define STACKINSTANTIABLE_H

#include "typeinstantiable.h"

namespace qyvlik {
namespace typer {



class StackInstantiable : public TypeInstantiable
{
public:
    StackInstantiable()
    {}
    StackInstantiable(TypeInstantiablePtr arg):
        templateArg0(arg)
    {}

    ValueProxy* newInstance() const {
        qDebug() << Q_FUNC_INFO;
        // check exprise
        templateArg0.lock()->newInstance();
        return nullptr;
    }
private:
    TypeInstantiableWeakPtr templateArg0;
};

}
}

#endif // STACKINSTANTIABLE_H
