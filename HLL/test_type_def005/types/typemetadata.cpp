#include "typemetadata.h"


namespace qyvlik {
namespace typer {

TemplateTypeMetaData::TemplateTypeMetaData(const QVector<TypeMetaData *> &templateArgs):
    templateTypeArguments(templateArgs)
{}

TemplateTypeMetaData::~TemplateTypeMetaData()
{}

bool TemplateTypeMetaData::isTemplate() const
{ return true; }

QVector<TypeInstanciable *> TemplateTypeMetaData::getInstanciables() const {
    QVector<TypeInstanciable*> instanciableArgs;
    QVector<TypeMetaData*>::ConstIterator ii = templateTypeArguments.begin();
    QVector<TypeMetaData*>::ConstIterator end = templateTypeArguments.end();
    while(ii != end) {
        instanciableArgs.push_back((*ii)->instanciable());
        ii++;
    }
    return instanciableArgs;
}



}
}
