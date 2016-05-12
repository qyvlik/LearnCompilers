#ifndef STACKTYPE_H
#define STACKTYPE_H


#include "templatetype.h"

namespace qyvlik {
namespace typer {


class StackType : public TemplateType
{
    // Type interface
public:

    StackType()
    { }

    StackType(TypePtr templateTypeArg0):
        templateTypeArg0(templateTypeArg0),
        instantiable(TypeInstantiablePtr( getInstantiableHelpter(templateTypeArg0)))
    { }

    TypeWeakPtr baseType() const override
    { return StackType::unComplete; }

    size_t templateArgsCount() const override
    { return 1; }

    void setTemplateTypeArg0(TypePtr templateTypeArg0 ) {
        this->templateTypeArg0 = templateTypeArg0;
        this->instantiable = getInstantiableHelpter(templateTypeArg0);
    }

    QString typeName() const override
    {
        if(templateTypeArg0.expired()) {
            TypePtr typeEntity = templateTypeArg0.lock();
            return "stack<" + typeEntity->typeName() + ">";
        } else {
            qDebug() << "templateTypeArg0 not expired";
            return "stack";
        }
    }

    TypeInstantiablePtr typeInstantiable() const override
    {
        return instantiable;
    }

    bool isComplete() const override
    {
        return templateTypeArg0.expired();
    }

protected:
    static TypeInstantiablePtr getInstantiableHelpter(TypePtr templateTypeArg0) {
        return TypeInstantiablePtr(new StackInstantiable(
                                       templateTypeArg0->typeInstantiable()));
    }

private:
    // 因为是模板类的参数，所以不要保存引用
    TypeWeakPtr templateTypeArg0;
    // 自身的 vector<?> 的构造器
    TypeInstantiablePtr instantiable;
public:
    static TypePtr unComplete;
};

}
}


#endif // STACKTYPE_H
