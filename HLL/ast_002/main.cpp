#include <QCoreApplication>

#include <sys/time.h>
#include "ast/node.h"
#include "ast/visitor.h"
#include "typeparser.h"
#include "context/typesystem.h"

#include <memory>


template<typename T>
inline std::shared_ptr<T> weak_lock(const std::weak_ptr<T>& d) throw(std::string)
{
    if(d.expired()) {
        return d.lock();
    } else {
        throw "expired";
    }
}

using namespace qyvlik::typer;

void test();

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    test();

    return 0;

    //return a.exec();
}

void test()
{

    QList<Token> lexers;

    lexers   << Token(1, "let")
             << Token(1, "stack")
             << Token(1, "<") << Token(1, "int") << Token(1, ">")
             << Token(1, "as")
             << Token(1, "MyIntStack")
             << Token(1, ";") ;

    lexers   << Token(1, "let")
             << Token(1, "stack")
             << Token(1, "<") << Token(1, "int") << Token(1, ">")
             << Token(1, "as")
             << Token(1, "MyIntStack2")
             << Token(1, ";") ;

    lexers   << Token(1, "let")
             << Token(1, "stack")
             << Token(1, "<")
             << Token(1, "stack") << Token(1, "<") << Token(1, "int") << Token(1, ">")
             << Token(1, ">")
             << Token(1, "as")
             << Token(1, "IntStack")
             << Token(1, ";") ;

    lexers   << Token(1, "let")
             << Token(1, "stack")
             << Token(1, "<") << Token(1, "int") << Token(1, ">")
             << Token(1, "as")
             << Token(1, "IntStack2")
             << Token(1, ";") ;

    lexers   << Token(1, "let")
             << Token(1, "map")
             << Token(1, "<") << Token(1, "int") << Token(1, ",") << Token(1, "int")  << Token(1, ">")
             << Token(1, "as")
             << Token(1, "map1")
             << Token(1, ";")
                ;

    lexers   << Token(1, "let")
             << Token(1, "map")
             << Token(1, "<") << Token(1, "int") << Token(1, ",") << Token(1, "map1")  << Token(1, ">")
             << Token(1, "as")
             << Token(1, "map2")
             << Token(1, ";")
                ;

    lexers   << Token(1, "let")
             << Token(1, "vector")
             << Token(1, "<") << Token(1, "map2") << Token(1, ">")
             << Token(1, "as")
             << Token(1, "map2Vector")
             << Token(1, ";")
                ;

    ASTTree* astTree = new ASTTree;
    TokenStream* lexerStream = TokenStream::getLexerStream(lexers);

    TypeParser typeParser;
    typeParser.start(lexerStream, astTree);


    qDebug() << "=======================";

    TypeSystemVisitor visitor;

    try {
        timeval start, end;

        gettimeofday( &start, NULL );

        astTree->astNode->accept(&visitor);

        gettimeofday( &end, NULL );

        //long timeuse = 1000000 * ( end.tv_sec - start.tv_sec ) + end.tv_usec - start.tv_usec;
        qDebug() << "timeuse" << end.tv_usec - start.tv_usec;


        TypeMetaData* map2Vector = visitor.typeSystem->getTypeMetaData("map2Vector");
        if(map2Vector) {
            map2Vector->instanciable()->newInstance();
        } else {
            qDebug() << "map2Vector not exists";
        }

    } catch(const ParserError& e) {
        e.pritf();
    } catch(const TypeError& e) {
        e.pritf();
    }

    delete astTree->astNode;

    delete lexerStream;
}
