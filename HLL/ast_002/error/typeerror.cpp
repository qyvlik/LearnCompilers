#include "typeerror.h"


namespace qyvlik {
namespace typer {

TypeError::TypeError(int code, const QString &message):
    Error(code, message)
{}


}
}
