#ifndef PARSERERROR_H
#define PARSERERROR_H

#include "throwable.h"

namespace qyvlik {
namespace typer {


class ParserError : public Error
{
public:
    ParserError(int code, const QString& message);
};

}
}
#endif // PARSERERROR_H
