#include "type.h"

namespace qyvlik {
namespace lexer {


TypeInstantiableFactory::TypeInstantiableFactory()
{
    registerBaseType();
}

TypeInstantiableFactory::~TypeInstantiableFactory()
{
    auto iter = instancers.begin();
    auto end = instancers.end();
    while(iter != end) {
        qDebug() << iter.key() << ":" << iter.value().get();

        iter++;
    }
}

void TypeInstantiableFactory::registerBaseType()
{
    instancers.insert("bool",   TypeInstantiablePtr(new BoolInstantiable));
    instancers.insert("int",    TypeInstantiablePtr(new IntInstantiable));
    instancers.insert("int8",   TypeInstantiablePtr(new Int8Instantiable));
    instancers.insert("int16",  TypeInstantiablePtr(new Int16Instantiable));
    instancers.insert("string", TypeInstantiablePtr(new StringInstantiable));
}

TypeInstantiablePtr TypeInstantiableFactory::registerType(const QString &name,  TypeInstantiablePtr instancer)
{
    return instancers.insert(name, instancer).value();
}

TypeInstantiablePtr TypeInstantiableFactory::getTypeInstantiable(const QString &name)
{
    auto find = instancers.find(name);
    auto end = instancers.end();
    if(find != end) {
        return find.value();
    } else {
        return nullptr;
    }
}

//----------------------------------------------------------------



}
}


