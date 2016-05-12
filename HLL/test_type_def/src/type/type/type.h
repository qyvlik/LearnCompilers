#ifndef TYPE_H
#define TYPE_H

#include <QDebug>
#include <QString>
#include <QMap>
#include <memory>

#include "../instantiable/typeinstantiable.h"
#include "../instantiable/boolinstantiable.h"
#include "../instantiable/doubleinstantiable.h"
#include "../instantiable/int16instantiable.h"
#include "../instantiable/int32instantiable.h"
#include "../instantiable/int64instantiable.h"
#include "../instantiable/int8instantiable.h"
#include "../instantiable/stackinstantiable.h"
#include "../instantiable/stringinstantiable.h"
#include "../instantiable/vectorinstantiable.h"

namespace qyvlik {
namespace typer {

class Type;
typedef std::weak_ptr<Type> TypeWeakPtr;
typedef std::shared_ptr<Type> TypePtr;

// Type 本身要保存 TypeInstantiablePtr 的引用
class Type {
public:
    virtual ~Type() = 0;
    virtual QString typeName() const = 0;
    virtual TypeWeakPtr baseType() const = 0;
    virtual TypeInstantiablePtr typeInstantiable() const = 0;
    virtual bool isTemplate() const =0 ;
    virtual size_t templateArgsCount() const;
};



}
}
#endif // TYPE_H
