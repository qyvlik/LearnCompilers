#ifndef TYPEINSTANCIABLE_H
#define TYPEINSTANCIABLE_H

#include <QtDebug>
#include <QVector>

namespace qyvlik {
namespace typer {


class TypeInstanciable {
public:
    virtual ~TypeInstanciable(){}
    virtual int newInstance() = 0;
};

template<typename T>
class BaseTypeInstanciable : public TypeInstanciable
{
public:
    ~BaseTypeInstanciable(){}
    int newInstance() {
        qDebug() << Q_FUNC_INFO;
        return 0;
    }
};


class TemplateTypeInstanciable : public TypeInstanciable
{
public:
    TemplateTypeInstanciable(const QVector<TypeInstanciable*>& instanciableArgs):
        instanciableArguments(instanciableArgs)
    {}
    ~TemplateTypeInstanciable()
    {}
protected:
    QVector<TypeInstanciable*> instanciableArguments;
};


class StackInstanciable : public TemplateTypeInstanciable
{
public:
    StackInstanciable(const QVector<TypeInstanciable*>& instanciableArgs):
        TemplateTypeInstanciable(instanciableArgs)
    {}

    int newInstance() override
    {
        this->instanciableArguments.at(0)->newInstance();
        qDebug() << Q_FUNC_INFO ;
        return 0;
    }
};

class MapInstanciable : public TemplateTypeInstanciable
{
public:
    MapInstanciable(const QVector<TypeInstanciable*>& instanciableArgs):
        TemplateTypeInstanciable(instanciableArgs)
    {}
    int newInstance() override
    {
        instanciableArguments.at(0)->newInstance();
        instanciableArguments.at(1)->newInstance();
        qDebug() << Q_FUNC_INFO;
        return 0;
    }
};

}
}

#endif // TYPEINSTANCIABLE_H
