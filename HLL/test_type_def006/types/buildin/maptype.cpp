#include "maptype.h"

namespace qyvlik {
namespace typer {

MapInstanciable::MapInstanciable(const QVector<TypeInstanciable *> &instanciableArgs):
    TemplateTypeInstanciable(instanciableArgs)
{}

int MapInstanciable::newInstance()
{
    instanciableArguments.at(0)->newInstance();
    instanciableArguments.at(1)->newInstance();
    qDebug() << Q_FUNC_INFO;
    return 0;
}

MapType::MapType(const QVector<TypeMetaData *> &templateArgs):
    TemplateTypeMetaData(templateArgs)
{}

int MapType::templateArgsCount() const
{
    return 2;
}

bool MapType::isTemplate() const
{
    return true;
}

TypeInstanciable *MapType::instanciable() const
{
    return new MapInstanciable(this->getInstanciables());
}

MapTypeFactory::~MapTypeFactory(){}

TypeMetaData *MapTypeFactory::templateTypeMetaData(const QVector<TypeMetaData *> &templateArgs)
{
    Q_ASSERT(templateArgs.size() > 0 && templateArgs.size() == templateArgumentsCount());
    return new MapType(templateArgs);

}

int MapTypeFactory::templateArgumentsCount() const
{
    return 2;
}



}
}

