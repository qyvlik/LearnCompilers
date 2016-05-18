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
    TemplateTypeInstanciable(const QVector<TypeInstanciable*>& instanciableArgs);
    ~TemplateTypeInstanciable();
protected:
    QVector<TypeInstanciable*> instanciableArguments;
};



}
}

#endif // TYPEINSTANCIABLE_H
