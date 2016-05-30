#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <stack>
#include <assert.h>

#include "src/utility.h"
#include "src/calculator.h"
#include "src/lexerstream.h"
#include "src/parser.h"
#include "src/ast.h"

using namespace std;


int main()
{

    // 1 + 1
    //    vector<string> l;
    //    l.push_back("1");
    //    l.push_back("+");
    //    l.push_back("1");

    // 2 * ( 1 + 1 )
    vector<string> l;
    l.push_back("2");
    l.push_back("*");
    l.push_back("(");
    l.push_back("1");
    l.push_back("+");
    l.push_back("1");
    l.push_back(")");


    LexerStream stream(l);

    Parser parser;
    parser.expr(&stream);

    //    cout << endl;
    //    cout << parser.code.front();


    long usetime = timeTick([&](){
        Calculator calculator;
        int count = 100000;
        while(count-- != 0) {
            calculator.code = parser.code;
            calculator.result();
        }
    });

    cout << endl;

    cout << "usetime:" << usetime;

    cout << endl;


    usetime = timeTick([&](){
        int count = 100000;
        while(count-- != 0) {
            Visitor visitor;
            parser.asttree.astNode->accept(&visitor);
        }
    });

    cout << endl;

    cout << "usetime:" << usetime;

    cout << endl;

}
