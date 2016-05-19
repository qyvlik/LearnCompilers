#ifndef TYPEERROR_H
#define TYPEERROR_H
#include "throwable.h"

namespace qyvlik {
namespace typer {


class TypeError : public Error
{
public:
    TypeError(int code, const QString& message);
};

}
}

#endif // TYPEERROR_H
