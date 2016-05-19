#include "visitor.h"
#include "node.h"

#include <QtDebug>

#include "types/buildintype.h"

namespace qyvlik {
namespace typer {

Visitor::~Visitor()
{}

void Visitor::visit(Node * node)
{
    Q_ASSERT(node != nullptr);
    qDebug() << "Node(" << node << ")";
}

void Visitor::visit(TypeDefinesNode * node)
{
    Q_ASSERT(node != nullptr);
    qDebug() << "TypeDefinesNode(" << node << ")" ;

    auto iter = node->typeDefines.begin();
    auto end = node->typeDefines.end();
    while(iter != end) {
        this->visit(*iter);
        iter++;
    }
}

void Visitor::visit(TypeDefineNode * node)
{
    Q_ASSERT(node != nullptr);
    qDebug() << "TypeDefineNode(" << node << ")" ;
    this->visit(node->typeSpecifierNode);
    this->visit(node->typeNameNode);
}

void Visitor::visit(TypeSpecifierNode * node)
{
    Q_ASSERT(node != nullptr);
    //    std::cout << "TypeSpecifierNode(" << node << ")" << std::endl;
    if(node->kind == Node::Kind_TypeNameNode) {
        this->visit(dynamic_cast<TypeNameNode*>(node));
    } else if(node->kind == Node::Kind_TemplateTypeSpecifier) {
        this->visit(dynamic_cast<TemplateTypeSpecifierNode*>(node));
    }

}

void Visitor::visit(TemplateTypeSpecifierNode * node)
{
    Q_ASSERT(node != nullptr);
    qDebug() << "TemplateTypeSpecifierNode(" << node << ")" ;
    auto iter = node->nodes.begin();
    auto end = node->nodes.end();
    while(iter != end) {
        this->visit(*iter);
        iter++;
    }
}

void Visitor::visit(TypeNameNode * node)
{
    Q_ASSERT(node != nullptr);
    qDebug() << "TypeNameNode(" << node << ")";
}

/////////////TypeSystemVisitor///////////

TypeSystemVisitor::TypeSystemVisitor():
    typeSystem(new TypeSystem)
{
}

TypeSystemVisitor::~TypeSystemVisitor()
{
   delete typeSystem;
}

void TypeSystemVisitor::visit(Node * node)
{
    Q_ASSERT(node != nullptr);
    qDebug() << "Node(" << node << ")";
}

void TypeSystemVisitor::visit(TypeDefinesNode *node)
{
    Q_ASSERT(node != nullptr);
    auto iter = node->typeDefines.begin();
    auto end = node->typeDefines.end();
    while(iter != end) {
        this->visit(*iter);
        iter++;
    }
}

// 定义类型
// let xx<xx> as x;
void TypeSystemVisitor::visit(TypeDefineNode * node)
{
    Q_ASSERT(node != nullptr);
    qDebug() << "TypeDefineNode(" << node << ")" ;

    this->visit(node->typeSpecifierNode);

    this->visit(node->typeNameNode);


    if(typeSystem->isRegisterType(node->typeNameNode->typeName)) {
        typeSystem->registerType(node->typeNameNode->typeName,
                                 typeSystem->getHelper()->takeCurrentArgumentTypeMetaData());
        typeSystem->mapTypeName(node->typeNameNode->typeName,
                                typeSystem->getHelper()->takeFullTypeName());
    } else {
        qWarning() << Q_FUNC_INFO << __FILE__ << __LINE__;
        qWarning() << "Type "+node->typeNameNode->typeName + " Already exist.";
    }

}

// 模板类型
void TypeSystemVisitor::visit(TypeSpecifierNode * node)
{
    Q_ASSERT(node != nullptr);
    //    std::cout << "TypeSpecifierNode(" << node << ")" << std::endl;
    if(node->kind == Node::Kind_TypeNameNode) {
        TypeNameNode* n = dynamic_cast<TypeNameNode*>(node);
        this->visit(n);

        typeSystem->getHelper()->pushArgumentTypeMetaData(typeSystem->getTypeMetaData(n->typeName));
        typeSystem->getHelper()->pushTypeToken(typeSystem->getFullTypeName(n->typeName));

    } else if(node->kind == Node::Kind_TemplateTypeSpecifier) {
        TemplateTypeSpecifierNode* n = dynamic_cast<TemplateTypeSpecifierNode*>(node);
        this->visit(n);
    }
}

// xx xx (xx)*
void TypeSystemVisitor::visit(TemplateTypeSpecifierNode * node)
{
    Q_ASSERT(node != nullptr);
    qDebug() << "TemplateTypeSpecifierNode(" << node << "," << node->templateName << ")" ;


    QString templateTypeName = node->templateName;
    const int templateArgsCount = typeSystem->templateArgsCount(templateTypeName);

    Q_ASSERT(templateArgsCount > 0);

    QVector<TypeMetaData *> thizTemplateArguments;

    typeSystem->getHelper()->newCurrentTemplateTypeName();

    typeSystem->getHelper()->pushTypeToken(templateTypeName);

    typeSystem->getHelper()->pushTypeToken("<");

    // <arg>
    // <arg0 ,arg1 ,arg2 >
    if(templateArgsCount == node->nodes.size()) {
        const int nodeSize = node->nodes.size();
        TypeSpecifierNode* nodeVar;
        for(int iter = 0; iter < nodeSize; iter++) {
            nodeVar = node->nodes.at(iter);
            if(iter != 0) {
                typeSystem->getHelper()->pushTypeToken(",");
            }

//            if(nodeVar->kind == Node::Kind_TypeNameNode) {
//                typeSystem->getHelper()->pushTypeToken(dynamic_cast<TypeNameNode*>(nodeVar)->typeName);
//            }
//            else if (nodeVar->kind == Node::Kind_TemplateTypeSpecifier) {
//                typeSystem->getHelper()->pushTypeToken(dynamic_cast<TemplateTypeSpecifierNode*>(nodeVar)->templateName);
//            }


            this->visit(nodeVar);

            thizTemplateArguments.push_back(typeSystem->getHelper()->takeCurrentArgumentTypeMetaData());

        }
    }

    typeSystem->getHelper()->pushTypeToken(">");

    TypeMetaData* templateTypeMeta = nullptr;
    TemplateTypeMetaDataFactory* templateTypeMetaData = nullptr;
    QString thizTemplateFullTypeName = typeSystem->getHelper()->takeCurrentTemplateFullTypeName();
    if(typeSystem->isRegisterType(thizTemplateFullTypeName)) {
        templateTypeMetaData = typeSystem->getTemplateTypeMetaData(templateTypeName);
        templateTypeMeta =
                templateTypeMetaData->templateTypeMetaData(thizTemplateArguments);
        typeSystem->registerType(thizTemplateFullTypeName, templateTypeMeta);
    } else {
        templateTypeMeta = typeSystem->getTypeMetaData(thizTemplateFullTypeName);
    }
    typeSystem->getHelper()->pushArgumentTypeMetaData(templateTypeMeta);
    typeSystem->getHelper()->fillTemplateArgument(thizTemplateFullTypeName);

}

void TypeSystemVisitor::visit(TypeNameNode * node)
{
    Q_ASSERT(node != nullptr);
    qDebug() << "TypeNameNode(" << node << "," << node->typeName << ")";
}

}
}
