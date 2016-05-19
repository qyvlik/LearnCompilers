#include "typeparser.h"

#include "context/typesystem.h"
#include "context/typesystemhelper_p.h"

namespace qyvlik {
namespace typer {


TypeParser::TypeParser():
    astTree(new ASTTree)
{

}

bool TypeParser::start(TokenStream *lexerStream, TypeSystem *__typeSystem)
{
    try {
        TypeDefinesStatement(lexerStream, __typeSystem);
        return true;
    } catch(const ParserError& error) {
        error.pritf();
        return false;
    }
}

void TypeParser::TypeDefinesStatement(TokenStream *lexerStream, TypeSystem *__typeSystem) throw(ParserError)
{

    QList<TypeDefineNode*> typeDefines;

    while(lexerStream->current().value() == "let") {
        TypeDefineStatement(lexerStream, __typeSystem);


        TypeDefineNode* __typeDefine = dynamic_cast<TypeDefineNode*>(astTree->nodeVar.pop());
        typeDefines.push_back(__typeDefine);

        if(lexerStream->current().value() != ";") {
            throw ParserError(-1, "Lost `;");
        } else {
            lexerStream->next();
        }

        TypeDefinesNode* node = astTree->createTypeDefinesNode(typeDefines);
        astTree->astNode = node;


    }
    qDebug() << "OK";
}

// let xx<xx> as xx;
void TypeParser::TypeDefineStatement(TokenStream *lexerStream, TypeSystem *__typeSystem)  throw(ParserError)
{
    // current is `LET
    // read next
    lexerStream->next();

    // 这里指向下一个token
    TypeSpecifier(lexerStream, __typeSystem);

    lexerStream->next();

    TypeSpecifierNode* typeSpecifierNode = dynamic_cast<TypeSpecifierNode*>(astTree->nodeVar.pop());

    if(lexerStream->current().value() == "as") {
//        lexerStream->next();

//        if(__typeSystem->isRegisterType(lexerStream->current().value())) {
//            __typeSystem->registerType(lexerStream->current().value(),
//                                       __typeSystem->getHelper()->takeCurrentArgumentTypeMetaData());
//            __typeSystem->mapTypeName(lexerStream->current().value(),
//                                      __typeSystem->getHelper()->takeFullTypeName());

//        } else {
//            throw ParserError(-1, "Type "+lexerStream->current().value()+" Already exist.");
//        }
// 这一段可能要移除，放到语法树解析上

        lexerStream->next();
        TypeName(lexerStream, __typeSystem);

        TypeNameNode* typeNameNode = astTree->createTypeName();
        typeNameNode->typeName = lexerStream->current().value();
        TypeDefineNode* node = astTree->createTypeDefineNode(typeSpecifierNode, typeNameNode);
        astTree->nodeVar.push(node);

        lexerStream->next();

    } else {
        throw ParserError(-1, "Type "+lexerStream->current().value()+" not a `as key word");
    }

}

// 这里也是耦合严重
// 解耦的方式是，提前向下读取一个 Token ，如果是 `< 就进入 TemplateTypeSpecifier
// 否则进入 TypeName
void TypeParser::TypeSpecifier(TokenStream *lexerStream, TypeSystem *__typeSystem)  throw(ParserError)
{
    lexerStream->next();
    QString ahead = lexerStream->current().value();
    lexerStream->rockBack();

    if(ahead == "<") {
        TemplateTypeSpecifier(lexerStream, __typeSystem);
    } else {
        TypeName(lexerStream, __typeSystem);

        TypeNameNode* node = astTree->createTypeName();
        node->typeName = lexerStream->current().value();
        astTree->nodeVar.push(node);
    }


    //    if( __typeSystem->isTemplate(lexerStream->current().value())) {
    //        TemplateTypeSpecifier(lexerStream, __typeSystem);
    //    } else if( !__typeSystem->isRegisterType(lexerStream->current().value())) {
    //        TypeName(lexerStream, __typeSystem);

    //        __typeSystem->getHelper()->pushArgumentTypeMetaData(__typeSystem->getTypeMetaData(lexerStream->current().value()));
    //        __typeSystem->getHelper()->pushTypeToken(__typeSystem->getFullTypeName(lexerStream->current().value()));

    //        TypeNameNode* node = astTree->createTypeName();
    //        node->typeName = lexerStream->current().value();
    //        astTree->nodeVar.push(node);

    //    } else {
    //        throw ParserError(-1, "Type "+lexerStream->current().value()+" not exist.");            // 这个类型也放到语法树中去。
    //    }
}

// xx <x (,x )* >
void TypeParser::TemplateTypeSpecifier(TokenStream *lexerStream, TypeSystem *__typeSystem)  throw(ParserError)
{

    QString templateTypeName = lexerStream->current().value();

    //! 语法树
    QList<TypeSpecifierNode*> nodes;

    lexerStream->next();

    if(lexerStream->current().value() == "<") {
        lexerStream->next();
        TypeSpecifier(lexerStream, __typeSystem);     // `, or `>,

        lexerStream->next();            // `, or `>, read next
    }  else {
        throw ParserError(-1, "Template Type " + templateTypeName + " lost `< ");
    }

    nodes.push_back(dynamic_cast<TypeSpecifierNode*>(astTree->nodeVar.pop()));


    if(lexerStream->current().value() == ">") {

        // do nothing

    } else if(lexerStream->current().value() == ",") {

        do {
            lexerStream->next();
            TypeSpecifier(lexerStream, __typeSystem);

            lexerStream->next();                // `, or `>

            nodes.push_back(dynamic_cast<TypeSpecifierNode*>(astTree->nodeVar.pop()));


        } while(lexerStream->current().value() == ",");

        if(lexerStream->current().value() == ">") {

            throw ParserError(-1, "Template Type "+templateTypeName + " lost `> ");

        }
    } else {
        throw ParserError(-1, "Template Type "+templateTypeName + " lost `> ");
    }


    TemplateTypeSpecifierNode* node = astTree->createTemplateTypeSpecifierNode(nodes);
    node->templateName = templateTypeName;
    astTree->nodeVar.push(node);


//    QString templateTypeName = lexerStream->current().value();

//    const int templateArgsCount = __typeSystem->templateArgsCount(templateTypeName);

//    Q_ASSERT(templateArgsCount > 0);

//    QVector<TypeMetaData *> thizTemplateArguments;

//    __typeSystem->getHelper()->newCurrentTemplateTypeName();

//    __typeSystem->getHelper()->pushTypeToken(templateTypeName);

//    //! 语法树
//    QList<TypeSpecifierNode*> nodes;

//    lexerStream->next();
//    if(lexerStream->current().value() == "<") {

//        __typeSystem->getHelper()->pushTypeToken(lexerStream->current().value());

//        lexerStream->next();
//        TypeSpecifier(lexerStream, __typeSystem);     // `, or `>,

//        lexerStream->next();            // `, or `>, read next
//    } else {
//        throw ParserError(-1, "Template Type " + templateTypeName+ " lost `< is ");
//    }

//    thizTemplateArguments.push_back(__typeSystem->getHelper()->takeCurrentArgumentTypeMetaData());

//    nodes.push_back(dynamic_cast<TypeSpecifierNode*>(astTree->nodeVar.pop()));

//    switch(templateArgsCount)
//    {
//    case 1: {
//        if(lexerStream->current().value() != ">") {
//            throw ParserError(-1, "Template Type "+
//                              templateTypeName+
//                              " templateArgsCount is "+templateArgsCount);
//        } else {

//            __typeSystem->getHelper()->pushTypeToken(lexerStream->current().value());

//            break;
//        }
//    }       // one template type argument

//    default : {
//        int commaCount = 0;
//        // templateArgsCount-1 个 `,
//        // (,TypeSpecifier)*
//        if(lexerStream->current().value() == ",") {

//            __typeSystem->getHelper()->pushTypeToken(lexerStream->current().value());

//            commaCount = 1;
//            do {
//                lexerStream->next();
//                TypeSpecifier(lexerStream, __typeSystem);

//                lexerStream->next();                // `, or `>

//                commaCount++;
//                if(commaCount > templateArgsCount) {
//                    throw ParserError(-1, "Template Type "+
//                                      templateTypeName+
//                                      " templateArgsCount is "+templateArgsCount);
//                }

//                thizTemplateArguments.push_back(__typeSystem->getHelper()->takeCurrentArgumentTypeMetaData());

//                nodes.push_back(dynamic_cast<TypeSpecifierNode*>(astTree->nodeVar.pop()));


//            } while(lexerStream->current().value() == ",");

//            if(lexerStream->current().value() == ">") {

//                __typeSystem->getHelper()->pushTypeToken(lexerStream->current().value());

//                break;
//            }
//        }
//        throw ParserError(-2, "Template Type "+
//                          templateTypeName+
//                          " templateArgsCount is "+templateArgsCount);

//    }       // two or more templte type arguemnts
//    }   // switch

//    TypeMetaData* templateTypeMeta = nullptr;
//    TemplateTypeMetaDataFactory* templateTypeMetaData = nullptr;
//    QString thizTemplateFullTypeName = __typeSystem->getHelper()->takeCurrentTemplateFullTypeName();
//    if(__typeSystem->isRegisterType(thizTemplateFullTypeName)) {
//        templateTypeMetaData = __typeSystem->getTemplateTypeMetaData(templateTypeName);
//        templateTypeMeta =
//                templateTypeMetaData->templateTypeMetaData(thizTemplateArguments);
//        __typeSystem->registerType(thizTemplateFullTypeName, templateTypeMeta);
//    } else {
//        templateTypeMeta = __typeSystem->getTypeMetaData(thizTemplateFullTypeName);
//    }
//    __typeSystem->getHelper()->pushArgumentTypeMetaData(templateTypeMeta);
//    __typeSystem->getHelper()->fillTemplateArgument(thizTemplateFullTypeName);


//    TemplateTypeSpecifierNode* node = astTree->createTemplateTypeSpecifierNode(nodes);
//    node->templateName = templateTypeName;
//    astTree->nodeVar.push(node);

}

void TypeParser::TypeName(TokenStream *lexerStream, TypeSystem *__typeSystem)  throw(ParserError)
{

    Q_UNUSED(__typeSystem)
    qDebug() << "type :" << lexerStream->current().value();

//    if(__typeSystem->isRegisterType(lexerStream->current().value())) {
//        qDebug() << "Error type";
//        throw ParserError(-1, "Type "+lexerStream->current().value()+" Not Exist.");
//    } else if(__typeSystem->isTemplate(lexerStream->current().value())) {
//    } else {
//        qDebug() << "base type :" << lexerStream->current().value();
//    }
}




ASTTree::ASTTree()
{

}

TypeDefinesNode *ASTTree::createTypeDefinesNode(const QList<TypeDefineNode *> &typeDefines)
{
    TypeDefinesNode* n = new TypeDefinesNode();
    n->typeDefines = typeDefines;
    return n;
}

TypeDefineNode *ASTTree::createTypeDefineNode(TypeSpecifierNode *typeSpecifierNode, TypeNameNode *typeNameNode)
{
    TypeDefineNode* n = new TypeDefineNode();
    n->typeSpecifierNode = typeSpecifierNode;
    n->typeNameNode = typeNameNode;
    return n;
}

TypeSpecifierNode *ASTTree::createTypeSpecifierNode(TypeSpecifierNode *node)
{
    return node;
}

TemplateTypeSpecifierNode *ASTTree::createTemplateTypeSpecifierNode(const QList<TypeSpecifierNode *> &nodes)
{
    TemplateTypeSpecifierNode* n = new TemplateTypeSpecifierNode();
    n->nodes = nodes;
    return n;
}

TypeNameNode *ASTTree::createTypeName()
{
    return new TypeNameNode();
}


}
} // namespace qyvlik::typer

