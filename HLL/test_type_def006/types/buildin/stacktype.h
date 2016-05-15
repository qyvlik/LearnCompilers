#ifndef STACKTYPE_H
#define STACKTYPE_H

#include "../templatetypemetadatafactory.h"
#include "../typemetadata.h"

namespace qyvlik {
namespace typer {

class StackInstanciable : public TemplateTypeInstanciable
{
public:
    StackInstanciable(const QVector<TypeInstanciable*>& instanciableArgs);
    int newInstance() override;
};

class StackType : public TemplateTypeMetaData
{
public:
    StackType(const QVector<TypeMetaData*>& templateArgs);

    int templateArgsCount() const override;
    bool isTemplate() const override;

    TypeInstanciable* instanciable() const;
};


class StackTypeFactory : public TemplateTypeMetaDataFactory
{
public:
    ~StackTypeFactory();
    TypeMetaData *templateTypeMetaData(const QVector<TypeMetaData *> &templateArgs) override;

    int templateArgumentsCount()const override;
};


}
}


#endif // STACKTYPE_H
