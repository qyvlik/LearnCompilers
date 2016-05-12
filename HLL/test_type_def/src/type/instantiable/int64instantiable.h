#ifndef INT64INSTANTIABLE_H
#define INT64INSTANTIABLE_H


#include "typeinstantiable.h"

namespace qyvlik {
namespace typer {


class Int64Instantiable : public TypeInstantiable
{
public:
    Int64Instantiable(){}
    ValueProxy* newInstance() const
    {
        qDebug() << Q_FUNC_INFO;
        return nullptr;
    }
};

}
}
#endif // INT64INSTANTIABLE_H
