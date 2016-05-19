#include "typeparser.h"

#include "context/typesystem.h"
#include "context/typesystemhelper_p.h"

namespace qyvlik {
namespace typer {

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

        TypeDefineNode* typeDefine =  dynamic_cast<TypeDefineNode*>(__typeSystem->nodeVar.pop());
        typeDefines.push_back(typeDefine);


        if(lexerStream->current().value() != ";") {
            throw ParserError(-1, "Lost `;");
        } else {
            lexerStream->next();
        }

        TypeDefinesNode* node = __typeSystem->createTypeDefinesNode(typeDefines);
        __typeSystem->astNode = node;

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

    TypeSpecifierNode* typeSpecifierNode =  dynamic_cast<TypeSpecifierNode*>(__typeSystem->nodeVar.pop());


    if(lexerStream->current().value() == "as") {
        lexerStream->next();

        if(__typeSystem->isRegisterType(lexerStream->current().value())) {
            __typeSystem->registerType(lexerStream->current().value(),
                                       __typeSystem->getHelper()->takeCurrentArgumentTypeMetaData());
            __typeSystem->mapTypeName(lexerStream->current().value(),
                                      __typeSystem->getHelper()->takeFullTypeName());



        } else {
            throw ParserError(-1, "Type "+lexerStream->current().value()+" Already exist.");
        }
        TypeName(lexerStream, __typeSystem);


        //! 语法树
        TypeNameNode* typeNameNode = __typeSystem->createTypeName();
        typeNameNode->typeName = lexerStream->current().value();

        TypeDefineNode* node = __typeSystem->createTypeDefineNode(typeSpecifierNode, typeNameNode);
        __typeSystem->nodeVar.push(node);

        // 由于忘记设计 TypeDefinesStatement 了。。。所以直接获取一条定义语句
//        __typeSystem->astNode = node;

        lexerStream->next();

    }

}

void TypeParser::TypeSpecifier(TokenStream *lexerStream, TypeSystem *__typeSystem)  throw(ParserError)
{
    if( __typeSystem->isTemplate(lexerStream->current().value())) {
        TemplateTypeSpecifier(lexerStream, __typeSystem);
    } else if( !__typeSystem->isRegisterType(lexerStream->current().value())) {
        TypeName(lexerStream, __typeSystem);

        __typeSystem->getHelper()->pushArgumentTypeMetaData(__typeSystem->getTypeMetaData(lexerStream->current().value()));
        __typeSystem->getHelper()->pushTypeToken(__typeSystem->getFullTypeName(lexerStream->current().value()));


        //! 语法树
        TypeNameNode* node = __typeSystem->createTypeName();
        node->typeName = lexerStream->current().value();
        __typeSystem->nodeVar.push(node);


    } else {
        throw ParserError(-1, "Type "+lexerStream->current().value()+" not exist.");
    }
}

// xx <x (,x )* >
void TypeParser::TemplateTypeSpecifier(TokenStream *lexerStream, TypeSystem *__typeSystem)  throw(ParserError)
{
    QString templateTypeName = lexerStream->current().value();

    const int templateArgsCount = __typeSystem->templateArgsCount(templateTypeName);

    Q_ASSERT(templateArgsCount > 0);

    QVector<TypeMetaData *> thizTemplateArguments;

    __typeSystem->getHelper()->newCurrentTemplateTypeName();

    __typeSystem->getHelper()->pushTypeToken(templateTypeName);

    //! 语法树
    QList<TypeSpecifierNode*> nodes;

    lexerStream->next();
    if(lexerStream->current().value() == "<") {

        __typeSystem->getHelper()->pushTypeToken(lexerStream->current().value());

        lexerStream->next();
        TypeSpecifier(lexerStream, __typeSystem);     // `, or `>,

        lexerStream->next();            // `, or `>, read next
    } else {
        throw ParserError(-1, "Template Type " + templateTypeName+ " lost `< is ");
    }

    thizTemplateArguments.push_back(__typeSystem->getHelper()->takeCurrentArgumentTypeMetaData());

    //! 语法树
    nodes.push_back(dynamic_cast<TypeSpecifierNode*>(__typeSystem->nodeVar.pop()));

    switch(templateArgsCount)
    {
    case 1: {
        if(lexerStream->current().value() != ">") {
            throw ParserError(-1, "Template Type "+
                              templateTypeName+
                              " templateArgsCount is "+templateArgsCount);
        } else {

            __typeSystem->getHelper()->pushTypeToken(lexerStream->current().value());

            break;
        }
    }       // one template type argument

    default : {
        int commaCount = 0;
        // templateArgsCount-1 个 `,
        // (,TypeSpecifier)*
        if(lexerStream->current().value() == ",") {

            __typeSystem->getHelper()->pushTypeToken(lexerStream->current().value());

            commaCount = 1;
            do {
                lexerStream->next();
                TypeSpecifier(lexerStream, __typeSystem);

                lexerStream->next();                // `, or `>

                commaCount++;
                if(commaCount > templateArgsCount) {
                    throw ParserError(-1, "Template Type "+
                                      templateTypeName+
                                      " templateArgsCount is "+templateArgsCount);
                }

                thizTemplateArguments.push_back(__typeSystem->getHelper()->takeCurrentArgumentTypeMetaData());

                //! 语法树
                nodes.push_back(dynamic_cast<TypeSpecifierNode*>(__typeSystem->nodeVar.pop()));


            } while(lexerStream->current().value() == ",");

            if(lexerStream->current().value() == ">") {

                __typeSystem->getHelper()->pushTypeToken(lexerStream->current().value());

                break;
            }
        }
        throw ParserError(-2, "Template Type "+
                          templateTypeName+
                          " templateArgsCount is "+templateArgsCount);

    }       // two or more templte type arguemnts
    }   // switch

    TypeMetaData* templateTypeMeta = nullptr;
    TemplateTypeMetaDataFactory* templateTypeMetaData = nullptr;
    QString thizTemplateFullTypeName = __typeSystem->getHelper()->takeCurrentTemplateFullTypeName();
    if(__typeSystem->isRegisterType(thizTemplateFullTypeName)) {
        templateTypeMetaData = __typeSystem->getTemplateTypeMetaData(templateTypeName);
        templateTypeMeta =
                templateTypeMetaData->templateTypeMetaData(thizTemplateArguments);
        __typeSystem->registerType(thizTemplateFullTypeName, templateTypeMeta);
    } else {
        templateTypeMeta = __typeSystem->getTypeMetaData(thizTemplateFullTypeName);
    }
    __typeSystem->getHelper()->pushArgumentTypeMetaData(templateTypeMeta);
    __typeSystem->getHelper()->fillTemplateArgument(thizTemplateFullTypeName);


    //! 语法树
    TemplateTypeSpecifierNode* node = __typeSystem->createTemplateTypeSpecifierNode(nodes);
    node->templateName = templateTypeName;
    __typeSystem->nodeVar.push(node);
}

void TypeParser::TypeName(TokenStream *lexerStream, TypeSystem *__typeSystem)  throw(ParserError)
{

    if(__typeSystem->isRegisterType(lexerStream->current().value())) {
        qDebug() << "Error type";
        throw ParserError(-1, "Type "+lexerStream->current().value()+" Not Exist.");
    } else if(__typeSystem->isTemplate(lexerStream->current().value())) {
        qDebug() << "base template type :" << lexerStream->current().value();
    } else {
        qDebug() << "base type :" << lexerStream->current().value();
    }
}





}
} // namespace qyvlik::typer

