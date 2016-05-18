#include <QCoreApplication>

#include "ast/node.h"
#include "ast/visitor.h"

#include "typeparser.h"
#include "context/typesystem.h"

using namespace qyvlik::typer;

void test();
void test2();
void test3();

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // test2();
    test3();
    return 0;

    //return a.exec();
}

void test()
{
    Visitor visitor;

    // let int as int8 ;

    qDebug() << "let int as int8 ;" ;

    TypeNameNode int_type ;
    TypeNameNode int8_type ;

    TypeDefineNode typeDefineNode;
    typeDefineNode.typeSpecifierNode = &int_type;
    typeDefineNode.typeNameNode = &int8_type;
    typeDefineNode.accept(&visitor);
}

void test2()
{
    Visitor visitor;

    // let stack<int> as IntStack ;
    qDebug() << "let stack<int> as IntStack ;";

    TemplateTypeSpecifierNode stack_int;
    TypeNameNode int_type;
    stack_int.nodes.push_back(&int_type);

    TypeNameNode int_stack_type;

    TypeDefineNode typeDefineNode;
    typeDefineNode.typeSpecifierNode = &stack_int;
    typeDefineNode.typeNameNode = &int_stack_type;

    typeDefineNode.accept(&visitor);
}

void test3()
{

    QList<Token> lexers;

        lexers   << Token(1, "let")
                 << Token(1, "stack")
                 << Token(1, "<") << Token(1, "int") << Token(1, ">")
                 << Token(1, "as")
                 << Token(1, "MyIntStack")
                 << Token(1, ";") ;

//        lexers   << Token(1, "let")
//                 << Token(1, "stack")
//                 << Token(1, "<") << Token(1, "int") << Token(1, ">")
//                 << Token(1, "as")
//                 << Token(1, "MyIntStack2")
//                 << Token(1, ";") ;

//        lexers   << Token(1, "let")
//                 << Token(1, "stack")
//                 << Token(1, "<")
//                 << Token(1, "stack") << Token(1, "<") << Token(1, "int") << Token(1, ">")
//                 << Token(1, ">")
//                 << Token(1, "as")
//                 << Token(1, "IntStack")
//                 << Token(1, ";") ;

//        lexers   << Token(1, "let")
//                 << Token(1, "stack")
//                 << Token(1, "<") << Token(1, "int") << Token(1, ">")
//                 << Token(1, "as")
//                 << Token(1, "IntStack2")
//                 << Token(1, ";") ;

//    lexers   << Token(1, "let")
//             << Token(1, "map")
//             << Token(1, "<") << Token(1, "int") << Token(1, ",") << Token(1, "int")  << Token(1, ">")
//             << Token(1, "as")
//             << Token(1, "map1")
//             << Token(1, ";")
//                ;

//    lexers   << Token(1, "let")
//             << Token(1, "map")
//             << Token(1, "<") << Token(1, "int") << Token(1, ",") << Token(1, "map1")  << Token(1, ">")
//             << Token(1, "as")
//             << Token(1, "map2")
//             << Token(1, ";")
//                ;

    TokenStream* lexerStream = TokenStream::getLexerStream(lexers);

    TypeSystem* typeSystem = new TypeSystem();
    TypeParser typeParser;
    typeParser.start(lexerStream, typeSystem);


//    TypeMetaData* map2 = typeSystem->getTypeMetaData("map2");
//    map2->instanciable()->newInstance();

    TypeSystemVisitor visitor;
    typeSystem->astNode->accept(&visitor);



    delete lexerStream;
    delete typeSystem;
}
