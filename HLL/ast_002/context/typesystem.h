#ifndef TYPESYSTEM_H
#define TYPESYSTEM_H

#include <QtDebug>
#include <QStack>
#include <QMap>

#include "types/buildintype.h"
#include "../ast/node.h"

namespace qyvlik {
namespace typer {

class TypeMetaData;
class TypeSystemHelper;

//# FixMe? Context or Env
class TypeSystem {
public:
    TypeSystem();

    virtual ~TypeSystem();


    void registerType(const QString &name, TypeMetaData* metaData);

    TemplateTypeMetaDataFactory* getTemplateTypeMetaData(const QString& templateName);

    void mapTypeName(const QString& name, const QString& fullName);

    QString getFullTypeName(const QString& name);

    bool isTemplate(const QString& name);

    bool isRegisterType(const QString& name);

    TypeMetaData* getTypeMetaData(const QString& name);

    int templateArgsCount(const QString& templateName);

    TypeSystemHelper *getHelper() const;

    TypeDefinesNode* createTypeDefinesNode(const QList<TypeDefineNode*>& typeDefines) {
        TypeDefinesNode* n = new TypeDefinesNode();
        n->typeDefines = typeDefines;
        return n;
    }

    TypeDefineNode* createTypeDefineNode( TypeSpecifierNode* typeSpecifierNode, TypeNameNode* typeNameNode) {
        TypeDefineNode* n = new TypeDefineNode();
        n->typeSpecifierNode = typeSpecifierNode;
        n->typeNameNode = typeNameNode;
        return n;
    }

    // TypeSpecifierNode -> TemplateTypeSpecifierNode | TypeName
    TypeSpecifierNode* createTypeSpecifierNode(TypeSpecifierNode* node) {
        return node;
    }

    TemplateTypeSpecifierNode* createTemplateTypeSpecifierNode(const QList<TypeSpecifierNode*>& nodes) {
        TemplateTypeSpecifierNode* n = new TemplateTypeSpecifierNode();
        n->nodes = nodes;
        return n;
    }

    TypeNameNode* createTypeName() {
        return  new TypeNameNode();
    }

    Node* astNode;
    QStack<Node*> nodeVar;

protected:
    QMap<QString, TemplateTypeMetaDataFactory*> unCompleteType;
    QMap<QString, TypeMetaData*> completeType;
    QMap<QString, QString> typeNameMap;
    TypeSystemHelper* helper;


};


}
}

#endif // TYPESYSTEM_H
