#include "typeparser.h"

#include "ast/node.h"

namespace qyvlik {
namespace typer {


TypeParser::TypeParser()
{

}

bool TypeParser::start(TokenStream *lexerStream, ASTTree *__astTree)
{
    try {
        TypeDefinesStatement(lexerStream, __astTree);
        return true;
    } catch(const ParserError& error) {
        error.pritf();
        return false;
    }
}

void TypeParser::TypeDefinesStatement(TokenStream *lexerStream, ASTTree *__astTree) throw(ParserError)
{

    QList<TypeDefineNode*> typeDefines;

    while(lexerStream->current().value() == "let") {

        TypeDefineStatement(lexerStream, __astTree);


        TypeDefineNode* __typeDefine = dynamic_cast<TypeDefineNode*>(__astTree->nodeVar.pop());
        typeDefines.push_back(__typeDefine);

        if(lexerStream->current().value() != ";") {
            throw ParserError(-1, "Lost `;");
        } else {
            lexerStream->next();
        }

        TypeDefinesNode* node = __astTree->createTypeDefinesNode(typeDefines);
        __astTree->astNode = node;


    }
    qDebug() << "OK";
}

// let xx<xx> as xx;
void TypeParser::TypeDefineStatement(TokenStream *lexerStream, ASTTree *__astTree)  throw(ParserError)
{
    // current is `LET
    // read next
    lexerStream->next();

    // 这里指向下一个token
    TypeSpecifier(lexerStream, __astTree);

    lexerStream->next();

    TypeSpecifierNode* typeSpecifierNode = dynamic_cast<TypeSpecifierNode*>(__astTree->nodeVar.pop());

    if(lexerStream->current().value() == "as") {

        lexerStream->next();

        TypeName(lexerStream, __astTree);

        TypeNameNode* typeNameNode = __astTree->createTypeName();
        typeNameNode->typeName = lexerStream->current().value();
        TypeDefineNode* node = __astTree->createTypeDefineNode(typeSpecifierNode, typeNameNode);
        __astTree->nodeVar.push(node);

        lexerStream->next();

    } else {
        throw ParserError(-1, "Type "+lexerStream->current().value()+" not a `as key word");
    }

}

void TypeParser::TypeSpecifier(TokenStream *lexerStream, ASTTree *__astTree)  throw(ParserError)
{
    lexerStream->next();
    QString ahead = lexerStream->current().value();
    lexerStream->rockBack();

    if(ahead == "<") {
        TemplateTypeSpecifier(lexerStream, __astTree);
    } else {
        TypeName(lexerStream, __astTree);

        TypeNameNode* node = __astTree->createTypeName();
        node->typeName = lexerStream->current().value();
        __astTree->nodeVar.push(node);
    }
}

// xx <x (,x )* >
void TypeParser::TemplateTypeSpecifier(TokenStream *lexerStream, ASTTree *__astTree)  throw(ParserError)
{

    QString templateTypeName = lexerStream->current().value();

    //! 语法树
    QList<TypeSpecifierNode*> nodes;

    lexerStream->next();

    if(lexerStream->current().value() == "<") {
        lexerStream->next();
        TypeSpecifier(lexerStream, __astTree);     // `, or `>,

        lexerStream->next();            // `, or `>, read next
    }  else {
        throw ParserError(-1, "Template Type " + templateTypeName + " lost `< ");
    }

    nodes.push_back(dynamic_cast<TypeSpecifierNode*>(__astTree->nodeVar.pop()));


    if(lexerStream->current().value() == ">") {

        // do nothing

    } else if(lexerStream->current().value() == ",") {

        do {
            lexerStream->next();
            TypeSpecifier(lexerStream, __astTree);

            lexerStream->next();                // `, or `>

            nodes.push_back(dynamic_cast<TypeSpecifierNode*>(__astTree->nodeVar.pop()));


        } while(lexerStream->current().value() == ",");

        if(lexerStream->current().value() != ">") {

            throw ParserError(-1, "Template Type "+templateTypeName + " lost `> ");

        }
    } else {
        throw ParserError(-1, "Template Type "+templateTypeName + " lost `> ");
    }


    TemplateTypeSpecifierNode* node = __astTree->createTemplateTypeSpecifierNode(nodes);
    node->templateName = templateTypeName;
    __astTree->nodeVar.push(node);

}

void TypeParser::TypeName(TokenStream *lexerStream, ASTTree *__astTree)  throw(ParserError)
{

    Q_UNUSED(__astTree)
    qDebug() << "type :" << lexerStream->current().value();
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

