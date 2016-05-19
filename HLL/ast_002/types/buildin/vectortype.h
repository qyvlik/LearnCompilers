#ifndef VECTORTYPE_H
#define VECTORTYPE_H

#include "../templatetypemetadatafactory.h"
#include "../typemetadata.h"

namespace qyvlik {
namespace typer {

class VectorInstanciable : public TemplateTypeInstanciable
{
public:
    VectorInstanciable(const QVector<TypeInstanciable*>& instanciableArgs);
    int newInstance() override;
};

class VectorType : public TemplateTypeMetaData
{
public:
    VectorType(const QVector<TypeMetaData*>& templateArgs);

    int templateArgsCount() const override;
    TypeInstanciable* instanciable() const;
};


class VectorTypeFactory : public TemplateTypeMetaDataFactory
{
public:
    ~VectorTypeFactory();
    TypeMetaData *templateTypeMetaData(const QVector<TypeMetaData *> &templateArgs) override;

    int templateArgumentsCount()const override;
};


}
}

#endif // VECTORTYPE_H
