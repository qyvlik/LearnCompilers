
#include "src/lexerstream.hpp"
#include "src/statementparser.h"

#include "src/utility.hpp"

using namespace std;

int main()
{

//    string code = "if ( true ) {"
//                  " echo true ;"
//                  " } else {"
//                  " echo false ;"
//                  " }";

    // for ( true ; true ; true ) { do { } while ( true ) ; }
     string code = "for ( true ; true ; true ) { do { } while ( true ) ; }";
    // string code = "try { if ( true ) { while ( true ) { } } } catch ( true ) { }";

    vector<string> tokens = qyvlik::split(code, " ");

    LexerStream lexers(tokens);

    try {
        StatementParser::Statements(&lexers);
    } catch(std::string error) {
        cout << "error: " << error << endl;
    }

    cout << endl << "parer done" << endl;

    return 0;
}

