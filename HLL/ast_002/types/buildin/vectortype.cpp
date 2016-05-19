#include "vectortype.h"

namespace qyvlik {
namespace typer {

VectorInstanciable::VectorInstanciable(const QVector<TypeInstanciable *> &instanciableArgs):
        TemplateTypeInstanciable(instanciableArgs)
{

}

int VectorInstanciable::newInstance()
{
    this->instanciableArguments.at(0)->newInstance();
    qDebug() << Q_FUNC_INFO ;
    return 0;
}

VectorType::VectorType(const QVector<TypeMetaData *> &templateArgs):
    TemplateTypeMetaData(templateArgs)
{

}

int VectorType::templateArgsCount() const
{
    return 1;
}

TypeInstanciable *VectorType::instanciable() const
{
    return new VectorInstanciable(this->getInstanciables());

}

VectorTypeFactory::~VectorTypeFactory()
{

}

TypeMetaData *VectorTypeFactory::templateTypeMetaData(const QVector<TypeMetaData *> &templateArgs)
{
    Q_ASSERT(templateArgs.size() > 0 && templateArgs.size() == templateArgumentsCount());
    // qDebug() << Q_FUNC_INFO ;
    return new VectorType(templateArgs);
}

int VectorTypeFactory::templateArgumentsCount() const
{
    return 1;
}


}
}
