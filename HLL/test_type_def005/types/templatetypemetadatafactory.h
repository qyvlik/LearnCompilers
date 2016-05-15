#ifndef TEMPLATETYPEMETADATAFACTORY_H
#define TEMPLATETYPEMETADATAFACTORY_H

#include <QVector>

#include "typemetadata.h"

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

class StackTypeFactory : public TemplateTypeMetaDataFactory
{
public:
    ~StackTypeFactory(){}
    TypeMetaData *templateTypeMetaData(const QVector<TypeMetaData *> &templateArgs) override
    {
        Q_ASSERT(templateArgs.size() > 0 && templateArgs.size() == templateArgumentsCount());
        // qDebug() << Q_FUNC_INFO ;
        return new StackType(templateArgs);
    }

    int templateArgumentsCount()const override {
        return 1;
    }
};

class MapTypeFactory : public TemplateTypeMetaDataFactory
{
public:
    ~MapTypeFactory(){}
    TypeMetaData *templateTypeMetaData(const QVector<TypeMetaData *> &templateArgs) override
    {
        Q_ASSERT(templateArgs.size() > 0 && templateArgs.size() == templateArgumentsCount());
        return new MapType(templateArgs);

    }
    int templateArgumentsCount() const override
    {
        return 2;
    }
};

}
}

#endif // TEMPLATETYPEMETADATAFACTORY_H
