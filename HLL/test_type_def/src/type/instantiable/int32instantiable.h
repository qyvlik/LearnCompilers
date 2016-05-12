#ifndef INT32INSTANTIABLE_H
#define INT32INSTANTIABLE_H

#include "typeinstantiable.h"

namespace qyvlik {
namespace typer {


class Int32Instantiable : public TypeInstantiable
{
public:
    Int32Instantiable() {}
    ValueProxy* newInstance() const
    {
        qDebug() << Q_FUNC_INFO;
        return nullptr;
    }
};

}
}

#endif // INT32INSTANTIABLE_H
