#ifndef TYPEMETADATA_H
#define TYPEMETADATA_H

#include <QtDebug>
#include <QVector>

#include "typeinstanciable.h"

namespace qyvlik {
namespace typer {

class TypeInstanciable;
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
    TemplateTypeMetaData(const QVector<TypeMetaData*>& templateArgs);

    ~TemplateTypeMetaData();
    bool isTemplate() const override;

protected:
    QVector<TypeInstanciable*> getInstanciables() const;
    QVector<TypeMetaData*> templateTypeArguments;
};



}
}

#endif // TYPEMETADATA_H
