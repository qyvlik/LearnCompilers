#ifndef VALUE_H
#define VALUE_H

#include <iostream>
#include <memory>

#include "../type/type.h"

namespace qyvlik {
namespace typer {

class Value
{
public:
    Value()
    { }
    virtual ~Value()
    { }
    virtual TypeWeakPtr type() const = 0;
private:

};


}
}

#endif // VALUE_H
