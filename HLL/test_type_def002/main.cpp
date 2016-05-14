#include <QCoreApplication>
#include <QtDebug>

#include "typeparser.h"


using namespace qyvlik::typer;

void test();

int main(int
         // argc
         , char **
         // argv
         )
{
    // QCoreApplication app(argc, argv);

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
             << Token(1, "<") << Token(1, "MyIntStack") << Token(1, ">")
             << Token(1, "as")
             << Token(1, "IntStack")
             << Token(1, ";") ;

    //    lexers   << Token(1, "let")
    //             << Token(1, "stack")
    //             << Token(1, "<") << Token(1, "int") << Token(1, ">")
    //             << Token(1, "as")
    //             << Token(1, "IntStack")
    //             << Token(1, ";") ;
    //              ;


    TokenStream* lexerStream = TokenStream::getLexerStream(lexers);

    ITypeParser typeParser;
    typeParser.start(lexerStream);

    delete lexerStream;

    //return app.exec();
    return 0;
}


void test(){

    QList<Token> lexers;

    //    lexers   << Token(1, "let")
    //             <<  Token(1, "int")
    //             << Token(1, "as")
    //             << Token(1, "int32")
    //             << Token(1, ";") ;

    lexers   << Token(1, "let")
             << Token(1, "vector")
             << Token(1, "<") << Token(1, "int") << Token(1, ">")
             << Token(1, "as")
             << Token(1, "ivect")
             << Token(1, ";") ;


    //    lexers   << Token(1, "let")
    //             << Token(1, "vector")
    //             << Token(1, "<") << Token(1, "ivect") << Token(1, ">")
    //             << Token(1, "as")
    //             << Token(1, "imat2")
    //             << Token(1, ";") ;
    //              ;


    //    lexers   << Token(1, "let")
    //             << Token(1, "map")
    //             << Token(1, "<") << Token(1, "double") << Token(1, ",") << Token(1, "int")  << Token(1, ">")
    //             << Token(1, "as")
    //             << Token(1, "map2")
    //             << Token(1, ";") ;
    //              ;

    //    lexers   << Token(1, "let")
    //             << Token(1, "function")
    //             << Token(1, "<") << Token(1, "int") << Token(1, ",") << Token(1, "double") << Token(1, ",") << Token(1, "string") << Token(1, ">")
    //             << Token(1, "as")
    //             << Token(1, "callable")
    //             << Token(1, ";") ;
    ;

    TokenStream* lexerStream = TokenStream::getLexerStream(lexers);

    ITypeParser typeParser;
    typeParser.start(lexerStream);

    delete lexerStream;
}
