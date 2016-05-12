#ifndef DOUBLEINSTANTIABLE_H
#define DOUBLEINSTANTIABLE_H

#include "typeinstantiable.h"

namespace qyvlik {
namespace typer {

class DoubleInstantiable : public TypeInstantiable
{
public:
    DoubleInstantiable()
    {}
    ValueProxy* newInstance() const
    {
        qDebug() << Q_FUNC_INFO;
        return nullptr;
    }
};

}
}
#endif // DOUBLEINSTANTIABLE_H
