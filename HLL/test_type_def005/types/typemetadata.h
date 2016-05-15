#ifndef TYPEMETADATA_H
#define TYPEMETADATA_H

#include <QtDebug>
#include <QVector>

#include "typeinstanciable.h"

namespace qyvlik {
namespace typer {

class TypeMetaData
{
public:
    virtual ~TypeMetaData() {}
    virtual int templateArgsCount() const = 0;
    virtual bool isTemplate() const = 0;
    virtual TypeInstanciable* instanciable() const = 0;
};

template<typename T>
class BaseTypeMetaData : public TypeMetaData
{
public:
    ~BaseTypeMetaData(){}
    int templateArgsCount() const
    { return 0; }
    bool isTemplate() const
    { return false; }
    TypeInstanciable* instanciable() const {
        return new BaseTypeInstanciable<T>();
    }
};

class TemplateTypeMetaData : public TypeMetaData
{
public:
    TemplateTypeMetaData(const QVector<TypeMetaData*>& templateArgs):
        templateTypeArguments(templateArgs)
    {}

    ~TemplateTypeMetaData()
    {}
    bool isTemplate() const override
    { return true; }

protected:
    QVector<TypeInstanciable*> getInstanciables() const {
        QVector<TypeInstanciable*> instanciableArgs;
        QVector<TypeMetaData*>::ConstIterator ii = templateTypeArguments.begin();
        QVector<TypeMetaData*>::ConstIterator end = templateTypeArguments.end();
        while(ii != end) {
            instanciableArgs.push_back((*ii)->instanciable());
            ii++;
        }
        return instanciableArgs;
    }
    QVector<TypeMetaData*> templateTypeArguments;
};


class StackType : public TemplateTypeMetaData
{
public:
    StackType(const QVector<TypeMetaData*>& templateArgs):
        TemplateTypeMetaData(templateArgs)
    {}

    int templateArgsCount() const override
    {
        return 1;
    }
    bool isTemplate() const override
    {
        return true;
    }

    TypeInstanciable* instanciable() const {
        return new StackInstanciable(this->getInstanciables());
    }
};


class MapType : public TemplateTypeMetaData
{
public:
    MapType(const QVector<TypeMetaData*>& templateArgs):
        TemplateTypeMetaData(templateArgs)
    {}
    int templateArgsCount() const override
    {
        return 2;
    }
    bool isTemplate() const override
    {
        return true;
    }
    TypeInstanciable *instanciable() const override
    {
        return new MapInstanciable(this->getInstanciables());
    }
};

}
}

#endif // TYPEMETADATA_H
