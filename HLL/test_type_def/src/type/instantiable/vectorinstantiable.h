#ifndef VECTORINSTANTIABLE_H
#define VECTORINSTANTIABLE_H


#include "typeinstantiable.h"

namespace qyvlik {
namespace typer {



class VectorInstantiable : public TypeInstantiable
{
public:
    VectorInstantiable(TypeInstantiablePtr arg):
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

#endif // VECTORINSTANTIABLE_H
