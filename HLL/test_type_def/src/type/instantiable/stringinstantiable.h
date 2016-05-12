#ifndef STRINGINSTANTIABLE_H
#define STRINGINSTANTIABLE_H

#include "typeinstantiable.h"

namespace qyvlik {
namespace typer {


class StringInstantiable : public TypeInstantiable
{
public:
    StringInstantiable(){}
    ValueProxy* newInstance() const
    {
        qDebug() << Q_FUNC_INFO;
        // check exprise
        return nullptr;
    }
};

}
}
#endif // STRINGINSTANTIABLE_H
