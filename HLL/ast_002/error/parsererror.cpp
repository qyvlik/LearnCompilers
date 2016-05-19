#include "parsererror.h"
namespace qyvlik {
namespace typer {

ParserError::ParserError(int code, const QString &message):
    Error(code, message)
{ }

}
}
