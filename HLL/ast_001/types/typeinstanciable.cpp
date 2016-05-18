#include "typeinstanciable.h"

namespace qyvlik {
namespace typer {

TemplateTypeInstanciable::TemplateTypeInstanciable(const QVector<TypeInstanciable *> &instanciableArgs):
    instanciableArguments(instanciableArgs)
{}

TemplateTypeInstanciable::~TemplateTypeInstanciable()
{}



}
}
