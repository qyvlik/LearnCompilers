#ifndef BOOLINSTANTIABLE_H
#define BOOLINSTANTIABLE_H

#include "typeinstantiable.h"

namespace qyvlik {
namespace typer {


class BoolInstantiable : public TypeInstantiable
{
public:
    BoolInstantiable()
    {}
    ValueProxy* newInstance() const
    {
        qDebug() << Q_FUNC_INFO;
        return nullptr;
    }
};

}
}

#endif // BOOLINSTANTIABLE_H
