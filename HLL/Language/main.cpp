
#include <iostream>

#include "src/parser/parser.hpp"
#include "src/utility/utility.hpp"
#include "src/parser/lexer.hpp"

#include <map>
#include <algorithm>
using namespace qyvlik;
using namespace std;

void test_1();
void test_2();
void test_3();
void test_4();
void test_5();
void test_6();
void test_7();
void test_8();


int main()
{
//     test_1();
    // test_2();
    // test_3();
    // test_4();
//         test_5();
    //test_6();
//    test_7();
    test_8();

    return 0;

}

void test_1()
{
    std::string code = "A += J * ( K + L ) % (M - N) - (10 * 1); \n";
    CodeStream codeStream(code);
    ParserContext context;

    try {
        TokenStream stream(Lexer::parser(&codeStream));
        std::cout << "Lexer parser Done!" << std::endl;
        Parser::ExpressionStatement(&stream, &context);
        std::cout << "Parser Parser Done!" << std::endl;
    } catch(Throwable e) {
        std::cout << "error:" << std::endl;
        std::cout << e << std::endl;
        e.printTrack();
    }
    std::cout << "-----------------------------------------------" << std::endl;
}

void test_2()
{
    std::string code = "A = B . C . D [ \"E\" ] ( F, G, H ) [ \"I\" ] + J * K / L % M / N - 10; \n";
    CodeStream codeStream(code);
    ParserContext context;

    try {
        TokenStream stream(Lexer::parser(&codeStream));
        std::cout << "Lexer parser Done!" << std::endl;
        Parser::ExpressionStatement(&stream, &context);
        std::cout << "Parser Parser Done!" << std::endl;
    } catch(Throwable e) {
        std::cout << "error:" << std::endl;
        std::cout << e << std::endl;
        e.printTrack();
    }
    std::cout << "-----------------------------------------------" << std::endl;
}


void test_3()
{
    std::string code = "A=C<=D<=-D;";

    CodeStream codeStream(code);
    ParserContext context;

    try {
        TokenStream stream(Lexer::parser(&codeStream));
        std::cout << "Lexer parser Done!" << std::endl;
        Parser::Program(&stream, &context);
        std::cout << "Parser Parser Done!" << std::endl;
    } catch(Throwable e) {
        std::cout << "error:" << std::endl;
        std::cout << e << std::endl;
        e.printTrack();
    }
    std::cout << "-----------------------------------------------" << std::endl;
}

void test_4()
{
    std::string code = "A . B . C [ \"D\" ] ( 1, 2, 3 ) [ \"A\" ] = { \"name\" : 1 , \"age\": 1/2 , \"array\" : [ \"age\", 1, ] } ;";

    CodeStream codeStream(code);
    ParserContext context;

    try {
        TokenStream stream(Lexer::parser(&codeStream));
        std::cout << "Lexer parser Done!" << std::endl;
        Parser::Program(&stream, &context);
        std::cout << "Parser Parser Done!" << std::endl;
    } catch(Throwable e) {
        std::cout << "error:" << std::endl;
        std::cout << e << std::endl;
        e.printTrack();
    }
    std::cout << "-----------------------------------------------" << std::endl;
}


void test_5()
{
    std::string code = " if ( 1 != 1 ) {            \n"
                       "    while ( 1 == 1 )        \n"
                       "       { continue; }        \n"
                       " }                          \n"
                       " do {                       \n"
                       "    break;                  \n"
                       " } while(true);             \n";
    CodeStream codeStream(code);
    ParserContext context;

    try {
        TokenStream stream(Lexer::parser(&codeStream));
        std::cout << "Lexer parser Done!" << std::endl;
        Parser::Program(&stream, &context);
        std::cout << "Parser Parser Done!" << std::endl;
    } catch(Throwable e) {
        std::cout << "error:" << std::endl;
        std::cout << e << std::endl;
        e.printTrack();
    }
    std::cout << "-----------------------------------------------" << std::endl;
}

void test_6()
{
    std::string code = "for(i=0; i<10; i += 1) {\n"
                       "    continue; break;    \n"
                       "}                       \n";
    CodeStream codeStream(code);
    ParserContext context;

    try {
        TokenStream stream(Lexer::parser(&codeStream));
        std::cout << "Lexer parser Done!" << std::endl;
        Parser::Program(&stream, &context);
        std::cout << "Parser Parser Done!" << std::endl;
    } catch(Throwable e) {
        std::cout << "error:" << std::endl;
        std::cout << e << std::endl;
        e.printTrack();
    }
    std::cout << "-----------------------------------------------" << std::endl;
}

void test_7()
{
    std::string code = "int d, a = 1, b = a, c = a + b; { int a; } ";
    CodeStream codeStream(code);
    ParserContext context;

    try {
        TokenStream stream(Lexer::parser(&codeStream));
        std::cout << "Lexer parser Done!" << std::endl;
        Parser::Program(&stream, &context);
        std::cout << "Parser Parser Done!" << std::endl;
    } catch(Throwable e) {
        std::cout << "error:" << std::endl;
        std::cout << e << std::endl;
        e.printTrack();
    }
    std::cout << "-----------------------------------------------" << std::endl;
}

void test_8()
{
    std::string code = "function returnA(int a, int b) -> int { return a+b; } returnA(1, 1);";
    CodeStream codeStream(code);
    ParserContext context;

    try {
        TokenStream stream(Lexer::parser(&codeStream));
        std::cout << "Lexer parser Done!" << std::endl;
        Parser::Program(&stream, &context);
        std::cout << "Parser Parser Done!" << std::endl;
    } catch(Throwable e) {
        std::cout << "error:" << std::endl;
        std::cout << e << std::endl;
        e.printTrack();
    }
    std::cout << "-----------------------------------------------" << std::endl;
}

