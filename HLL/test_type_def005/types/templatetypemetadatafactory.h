#ifndef TEMPLATETYPEMETADATAFACTORY_H
#define TEMPLATETYPEMETADATAFACTORY_H

#include <QVector>

namespace qyvlik {
namespace typer {

class TypeMetaData;
class TemplateTypeMetaDataFactory
{
public:
    virtual ~TemplateTypeMetaDataFactory()
    {}
    virtual TypeMetaData* templateTypeMetaData(const QVector<TypeMetaData*>& templateArgs) = 0 ;
    virtual int templateArgumentsCount() const = 0;
};


}
}

#endif // TEMPLATETYPEMETADATAFACTORY_H
