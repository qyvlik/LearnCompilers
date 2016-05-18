#ifndef MAPTYPE_H
#define MAPTYPE_H

#include "../templatetypemetadatafactory.h"
#include "../typemetadata.h"

namespace qyvlik {
namespace typer {

class MapInstanciable : public TemplateTypeInstanciable
{
public:
    MapInstanciable(const QVector<TypeInstanciable*>& instanciableArgs);
    int newInstance() override;
};

class MapType : public TemplateTypeMetaData
{
public:
    MapType(const QVector<TypeMetaData*>& templateArgs);
    int templateArgsCount() const override;
    bool isTemplate() const override;
    TypeInstanciable *instanciable() const override;
};


class MapTypeFactory : public TemplateTypeMetaDataFactory
{
public:
    ~MapTypeFactory();
    TypeMetaData *templateTypeMetaData(const QVector<TypeMetaData *> &templateArgs) override;
    int templateArgumentsCount() const override;
};

}
}


#endif // MAPTYPE_H
