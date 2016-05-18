#include "stacktype.h"

namespace qyvlik {
namespace typer {

StackInstanciable::StackInstanciable(const QVector<TypeInstanciable *> &instanciableArgs):
    TemplateTypeInstanciable(instanciableArgs)
{}

int StackInstanciable::newInstance()
{
    this->instanciableArguments.at(0)->newInstance();
    qDebug() << Q_FUNC_INFO ;
    return 0;
}

StackType::StackType(const QVector<TypeMetaData *> &templateArgs):
    TemplateTypeMetaData(templateArgs)
{}

int StackType::templateArgsCount() const
{
    return 1;
}

bool StackType::isTemplate() const
{
    return true;
}

TypeInstanciable *StackType::instanciable() const {
    return new StackInstanciable(this->getInstanciables());
}

StackTypeFactory::~StackTypeFactory(){}

TypeMetaData *StackTypeFactory::templateTypeMetaData(const QVector<TypeMetaData *> &templateArgs)
{
    Q_ASSERT(templateArgs.size() > 0 && templateArgs.size() == templateArgumentsCount());
    // qDebug() << Q_FUNC_INFO ;
    return new StackType(templateArgs);
}

int StackTypeFactory::templateArgumentsCount() const {
    return 1;
}



}
}
