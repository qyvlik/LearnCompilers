#ifndef INT8INSTANTIABLE_H
#define INT8INSTANTIABLE_H

#include "typeinstantiable.h"

namespace qyvlik {
namespace typer {


class Int8Instantiable : public TypeInstantiable
{
public:
    Int8Instantiable()
    {
    }
    ValueProxy* newInstance() const
    {
        qDebug() << Q_FUNC_INFO;
        // check exprise
        return nullptr;
    }

};

}
}

#endif // INT8INSTANTIABLE_H
