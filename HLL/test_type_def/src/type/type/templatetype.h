#ifndef TEMPLATETYPE_H
#define TEMPLATETYPE_H


#include "type.h"

namespace qyvlik {
namespace typer {



class TemplateType : public Type
{
public:
    virtual ~TemplateType() { }
    //    virtual QString typeName() const = 0;
    //    virtual TypeWeakPtr baseType() const = 0;
    //    virtual TypeInstantiablePtr typeInstantiable() const = 0;

    bool isTemplate() const override;
    virtual bool isComplete() const;
};

}
}
#endif // TEMPLATETYPE_H
