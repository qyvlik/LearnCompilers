#include "typesystemhelper_p.h"

#include <QtDebug>

namespace qyvlik {
namespace typer {

void TypeSystemHelper::newCurrentTemplateTypeName() {
    thizTemplateTypeNameStack.push("");
}

void TypeSystemHelper::pushTypeToken(const QString &typeToken) {
    fullTypeName.push_back(typeToken);
    if(!thizTemplateTypeNameStack.isEmpty()) {
        thizTemplateTypeNameStack.top().push_back(typeToken);
    } else {
        qWarning( ) << __FILE__ << __LINE__<< Q_FUNC_INFO;
    }
}

QString TypeSystemHelper::takeCurrentTemplateFullTypeName() {
    return thizTemplateTypeNameStack.pop();
}

void TypeSystemHelper::fillTemplateArgument(const QString &thizTemplateFullTypeName) {
    if(!thizTemplateTypeNameStack.isEmpty()) {
        thizTemplateTypeNameStack.top().push_back(thizTemplateFullTypeName);
    }
}

void TypeSystemHelper::pushArgumentTypeMetaData(TypeMetaData *argType) {
    typeBuffer.push(argType);
}

TypeMetaData *TypeSystemHelper::takeCurrentArgumentTypeMetaData() {
    // TODO
    // qDebug() << Q_FUNC_INFO;
    if(!typeBuffer.isEmpty()) {
        return typeBuffer.pop();
    } else {
        qWarning() << __FILE__ << __LINE__ << Q_FUNC_INFO;
        return nullptr;
    }
}

QString TypeSystemHelper::takeFullTypeName() {
    QString t = fullTypeName;
    fullTypeName.clear();
    return t;
}




}
}
