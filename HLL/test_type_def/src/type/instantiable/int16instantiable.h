#ifndef INT16INSTANTIABLE_H
#define INT16INSTANTIABLE_H

#include "typeinstantiable.h"

namespace qyvlik {
namespace typer {


class Int16Instantiable : public TypeInstantiable
{
public:
    Int16Instantiable()
    {

    }
    ValueProxy* newInstance() const
    {
        qDebug() << Q_FUNC_INFO;
        return nullptr;
    }
};

}
}

#endif // INT16INSTANTIABLE_H
