#include <QCoreApplication>
#include <QtDebug>

#include "typeparser.h"
#include "src/type/type/vectortype.h"
#include "src/type/type/stacktype.h"

using namespace qyvlik::typer;

class TypeTable
{
public:
    explicit TypeTable()
    {
        TypePtr int32Type = TypePtr(new Int32Type);
        completeTypeTable.insert("int32", int32Type);
        // other base type

        unCompleteTemplateTypeTable.insert("vector", VectorType::unComplete);
        unCompleteTemplateTypeTable.insert("vector", StackType::unComplete);
    }

    virtual ~TypeTable()
    {}

private:
    QMap<QString, TypePtr> completeTypeTable;
    QMap<QString, TypePtr> unCompleteTemplateTypeTable;
};


int main(int argc, char *argv[])
{
    // QCoreApplication app(argc, argv);

    //    test();

    QList<Token> lexers;
    //    lexers << Token(1, "let")
    //           << Token(1, "map")
    //           << Token(1, "<") << Token(1, "string") << Token(1, ",") << Token(1, "string")   << Token(1, ">")
    //           << Token(1, "as")
    //           << Token(1, "Map")
    //           << Token(1, ";");         //

    lexers   << Token(1, "let")
             << Token(1, "vector")
             << Token(1, "<") << Token(1, "int") << Token(1, ">")
             << Token(1, "as")
             << Token(1, "mvect")
             << Token(1, ";") ;


    lexers   << Token(1, "let")
             << Token(1, "vector")
             << Token(1, "<") << Token(1, "mvect") << Token(1, ">")
             << Token(1, "as")
             << Token(1, "mat2")
             << Token(1, ";") ;

//    lexers << Token(1, "let")
//           << Token(1, "stack")
//           << Token(1, "<") << Token(1, "int")  << Token(1, ">")
//           << Token(1, "as")
//           << Token(1, "Stack")
//           << Token(1, ";")
//              ;

//    lexers << Token(1, "let")
//           << Token(1, "stack")
//           << Token(1, "<")
//           << Token(1, "vector")  << Token(1, "<")  << Token(1, "int") << Token(1, ">")
//           << Token(1, ">")
//           << Token(1, "as")
//           << Token(1, "MyStack")
//           << Token(1, ";")
              ;

    TokenStream* lexerStream = TokenStream::getLexerStream(lexers);

    ITypeParser typeParser;
    typeParser.start(lexerStream);

    delete lexerStream;

    //return app.exec();
    return 0;
}

