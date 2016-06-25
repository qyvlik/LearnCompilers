#include <iostream>
#include "src/parser.hpp"
#include "src/utility.hpp"
#include "src/lexer.hpp"

using namespace std;

void test_1();
void test_2();
void test_3();
void test_4();
void test_5();
void test_6();
void test_7();
void test_8();
void test_9();
void test_10();
void test_11();
void test_12();
void test_13();
void test_14();
void test_15();
void test_16();
void test_17();
void test_18();
void test_19();
void test_20();
void test_21();
void test_22();

int main()
{
//    test_1();
//    test_2();
//    test_3();
    test_4();
//    test_5();
//    test_6();
//    test_7();
//    test_8();
//    test_9();
//    test_10();
//    test_11();
//    test_12();
//    test_13();
//    test_14();
//    test_15();
//    test_16();
//    test_17();
//    test_18();
//    test_19();
//    test_20();
//    test_21();
//    test_22();
    return 0;
}


void test_1()
{
    std::string code = "A = A . B . C [ \"D\" ] ( 1, 2, 3 ) [ \"A\" ] + A * A / A % A / A - 10; \n"
                       "A = A - 10;\n";
    CodeStream codeStream(code);
    Static<ParserContext>::Object.clear();

    try {
        TokenStream stream(Lexer::parser(&codeStream));
        std::cout << "Lexer parser Done!" << std::endl;
        Parser::Program(&stream);
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
    std::string code = "A = { \"name\" : 1 , \"age\": 1/1 } ;";
    CodeStream codeStream(code);
    Static<ParserContext>::Object.clear();

    try {
        TokenStream stream(Lexer::parser(&codeStream));
        std::cout << "Lexer parser Done!" << std::endl;
        Parser::ExpressionStatement(&stream);
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
    std::string code = " A = [ \"age\", 1, ] ;";
    CodeStream codeStream(code);
    Static<ParserContext>::Object.clear();

    try {
        TokenStream stream(Lexer::parser(&codeStream));
        std::cout << "Lexer parser Done!" << std::endl;
        Parser::ExpressionStatement(&stream);
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
    std::string code = "A . B . C [ \"D\" ] ( 1, 2, 3 ) [ \"A\" ] = { \"name\" : 1 , \"age\": 1/1 , \"array\" : [ \"age\", 1, ] } ;";
    CodeStream codeStream(code);
    Static<ParserContext>::Object.clear();

    try {
        TokenStream stream(Lexer::parser(&codeStream));
        std::cout << "Lexer parser Done!" << std::endl;
        Parser::ExpressionStatement(&stream);
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
    std::string code = "try { if ( 1 ) { while ( 1 ) { } } } catch ( 1 ) { for ( 1 ; 1 ; 1 ) { do { } while ( 1 ) ; } }";
    CodeStream codeStream(code);
    Static<ParserContext>::Object.clear();

    try {
        TokenStream stream(Lexer::parser(&codeStream));
        std::cout << "Lexer parser Done!" << std::endl;
        Parser::Statements(&stream);
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
    std::string code = "     try { if ( 1 ) { while ( 1 ) {"
                       "     A . B . C [ \"D\" ] ( 1, 2, 3 ) [ \"A\" ] = { \"name\" : 1 , \"age\": 1/1 , \"array\" : [ \"age\", 1, ] } ;"
                       "     } } } catch ( 1 ) { for ( 1 ; 1 ; 1 ) { do { } while ( 1 ) ; } }";
    CodeStream codeStream(code);
    Static<ParserContext>::Object.clear();

    try {
        TokenStream stream(Lexer::parser(&codeStream));
        std::cout << "Lexer parser Done!" << std::endl;
        Parser::Statements(&stream);
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
    std::string code = "{ \"name\" : 1  \"age\": 1/1 } ;";
    CodeStream codeStream(code);
    Static<ParserContext>::Object.clear();

    try {
        TokenStream stream(Lexer::parser(&codeStream));
        std::cout << "Lexer parser Done!" << std::endl;
        Parser::ExpressionStatement(&stream);
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
    std::string code = "function (int a, int b) -> int { return a; }";
    CodeStream codeStream(code);
    Static<ParserContext>::Object.clear();

    try {
        TokenStream stream(Lexer::parser(&codeStream));
        std::cout << "Lexer parser Done!" << std::endl;
        Parser::Lambda(&stream);
        std::cout << "Parser Parser Done!" << std::endl;
    } catch(Throwable e) {
        std::cout << "error:" << std::endl;
        std::cout << e << std::endl;
        e.printTrack();
    }
    std::cout << "-----------------------------------------------" << std::endl;
}

void test_9()
{
    std::string code = "int a = 10, b = 11;";
    CodeStream codeStream(code);
    Static<ParserContext>::Object.clear();

    try {
        TokenStream stream(Lexer::parser(&codeStream));
        std::cout << "Lexer parser Done!" << std::endl;
        Parser::DeclarationStatement(&stream);
        std::cout << "Parser Parser Done!" << std::endl;
    } catch(Throwable e) {
        std::cout << "error:" << std::endl;
        std::cout << e << std::endl;
        e.printTrack();
    }
    std::cout << "-----------------------------------------------" << std::endl;
}

void test_10()
{
    std::string code = "    function add(int a, int b) -> int { return a + b ; } int result = add(1, 1);";
    CodeStream codeStream(code);
    Static<ParserContext>::Object.clear();

    try {
        TokenStream stream(Lexer::parser(&codeStream));
        std::cout << "Lexer parser Done!" << std::endl;
        Parser::Program(&stream);
        std::cout << "Parser Parser Done!" << std::endl;
    } catch(Throwable e) {
        std::cout << "error:" << std::endl;
        std::cout << e << std::endl;
        e.printTrack();
    }
    std::cout << "-----------------------------------------------" << std::endl;
}

void test_11()
{
    std::string code = "function add(int a, int b) -> var { return function () -> int { return a + b ; } ; } int result = add(1, 1)(); ";
    CodeStream codeStream(code);
    Static<ParserContext>::Object.clear();

    try {
        TokenStream stream(Lexer::parser(&codeStream));
        std::cout << "Lexer parser Done!" << std::endl;
        Parser::Program(&stream);
        std::cout << "Parser Parser Done!" << std::endl;
    } catch(Throwable e) {
        std::cout << "error:" << std::endl;
        std::cout << e << std::endl;
        e.printTrack();
    }
    std::cout << "-----------------------------------------------" << std::endl;
}


void test_12()
{
    std::string code = "     function add(int a, int b) -> var { return function () -> int { return a + b ; } ; }"
                       "     function sub(int a, int b) -> var {  return a - b ; }"
                       "     int result = add(1, 1)();"
                       "     int result2 = sub(result, result);";
    CodeStream codeStream(code);
    Static<ParserContext>::Object.clear();

    try {
        TokenStream stream(Lexer::parser(&codeStream));
        std::cout << "Lexer parser Done!" << std::endl;
        Parser::Program(&stream);
        std::cout << "Parser Parser Done!" << std::endl;
    } catch(Throwable e) {
        std::cout << "error:" << std::endl;
        std::cout << e << std::endl;
        e.printTrack();
    }
    std::cout << "-----------------------------------------------" << std::endl;
}

void test_13()
{
    std::string code = "a = b;";
    CodeStream codeStream(code);
    Static<ParserContext>::Object.clear();

    try {
        TokenStream stream(Lexer::parser(&codeStream));
        std::cout << "Lexer parser Done!" << std::endl;
        Parser::Program(&stream);
        std::cout << "Parser Parser Done!" << std::endl;
    } catch(Throwable e) {
        std::cout << "error:" << std::endl;
        std::cout << e << std::endl;
        e.printTrack();
    }
    std::cout << "-----------------------------------------------" << std::endl;
}

void test_14()
{
    std::string code = "     try { if ( 1 ) { while ( 1.02312 ) {"
                       "     A . B . C [ \"D\" ] ( 1, 2, 3 ) [ \"A\" ] = { \"name\" : 1 , \"age\": 1/1 , \"array\" : [ \"age\", 1, ] } ;"
                       "     } } } catch ( 1 ) { for ( 1 ; 1 ; 1 ) { do { } while ( 1 ) ; } } ";
    CodeStream stream(code);
    Static<ParserContext>::Object.clear();

    try {
        std::vector<Token> tokens = Lexer::parser(&stream);
        TokenStream stream(tokens);
        std::cout << "Lexer Done!" << std::endl;
        Parser::Program(&stream);
        std::cout << "Parser Parser Done!" << std::endl;
    } catch(Throwable e) {
        e.printTrack();
        std::cout << e.getError()  << std::endl;
    }
    std::cout << "-----------------------------------------------" << std::endl;
}

void test_15()
{
    std::string code = "     function add(int a, int b) -> var { return function () -> int { return a + b ; } ; }"
                       "     function sub(int a, int b) -> var {  return a - b ; }"
                       "     int result = add(1, 1)();"
                       "     int result2 = sub(result, result); ";
    CodeStream stream(code);
    Static<ParserContext>::Object.clear();

    try {
        std::vector<Token> tokens = Lexer::parser(&stream);
        TokenStream stream(tokens);
        std::cout << "Lexer Done!" << std::endl;
        Parser::Program(&stream);
        std::cout << "Parser Parser Done!" << std::endl;
    } catch(Throwable e) {
        e.printTrack();
        std::cout << e.getError()  << std::endl;
    }
    std::cout << "-----------------------------------------------" << std::endl;
}

void test_16()
{
    std::string code = "     function add(int a, int b) -> var { return function () -> int { return - a + b ; } ; }"
                       "     function more_than(int a, int b) -> var {  return !!(a > b) ; }";
    CodeStream stream(code);
    Static<ParserContext>::Object.clear();

    try {
        std::vector<Token> tokens = Lexer::parser(&stream);
        TokenStream stream(tokens);
        std::cout << "Lexer Done!" << std::endl;

        Parser::Program(&stream);
        std::cout << "Parser Parser Done!" << std::endl;
    } catch(Throwable e) {
        e.printTrack();
        std::cout << e.getError()  << std::endl;
    }
    std::cout << "-----------------------------------------------" << std::endl;
}

void test_17()
{
    std::string code = "!a = !a; ";
    CodeStream stream(code);
    Static<ParserContext>::Object.clear();

    try {
        std::vector<Token> tokens = Lexer::parser(&stream);
        TokenStream stream(tokens);
        std::cout << "Lexer Done!" << std::endl;

        Parser::Program(&stream);
        std::cout << "Parser Parser Done!" << std::endl;
    } catch(Throwable e) {
        e.printTrack();
        std::cout << e.getError()  << std::endl;
    }
    std::cout << "-----------------------------------------------" << std::endl;
}

void test_18()
{
    std::string code = " if(1){ continue; } ";
    CodeStream stream(code);
    Static<ParserContext>::Object.clear();

    try {
        std::vector<Token> tokens = Lexer::parser(&stream);
        TokenStream stream(tokens);
        std::cout << "Lexer Done!" << std::endl;

        Parser::Program(&stream);
        std::cout << "Parser Parser Done!" << std::endl;
    } catch(Throwable e) {
        e.printTrack();
        std::cout << e.getError()  << std::endl;
    }
    std::cout << "-----------------------------------------------" << std::endl;
}

void test_19()
{
    std::string code = " if(1){ } if(1) { if(1){ } else { if(1){ } else { if(1){ } else { } } } }";
    CodeStream stream(code);
    Static<ParserContext>::Object.clear();

    try {
        std::vector<Token> tokens = Lexer::parser(&stream);
        TokenStream stream(tokens);
        std::cout << "Lexer Done!" << std::endl;

        Parser::Program(&stream);
        std::cout << "Parser Parser Done!" << std::endl;
    } catch(Throwable e) {
        e.printTrack();
        std::cout << e.getError()  << std::endl;
    }
    std::cout << "-----------------------------------------------" << std::endl;
}

void test_20()
{
    std::string code = " do {                   \n"
                       "        do {            \n"
                       "            continue;   \n"
                       "            break;      \n"
                       "        } while(0);     \n"
                       " }while(1);             \n";
    CodeStream stream(code);
    Static<ParserContext>::Object.clear();

    try {
        std::vector<Token> tokens = Lexer::parser(&stream);
        TokenStream stream(tokens);
        std::cout << "Lexer Done!" << std::endl;

        Parser::Program(&stream);
        std::cout << "Parser Parser Done!" << std::endl;
    } catch(Throwable e) {
        e.printTrack();
        std::cout << e.getError()  << std::endl;
    }
    std::cout << "-----------------------------------------------" << std::endl;
}

void test_21()
{
    std::string code =  "        while(0) {      \n"
                        "            continue;   \n"
                        "            break;      \n"
                        "        }               \n";
    CodeStream stream(code);
    Static<ParserContext>::Object.clear();

    try {
        std::vector<Token> tokens = Lexer::parser(&stream);
        TokenStream stream(tokens);
        std::cout << "Lexer Done!" << std::endl;

        Parser::Program(&stream);
        std::cout << "Parser Parser Done!" << std::endl;
    } catch(Throwable e) {
        e.printTrack();
        std::cout << e.getError()  << std::endl;
    }
    std::cout << "-----------------------------------------------" << std::endl;
}

void test_22()
{
    std::string code = " for(i=0; i<10; i += 1){            \n"
                       "    a=i;                            \n"
                       "    for(j=0; j<10; j += 1){         \n"
                       "       a=j;                         \n"
                       "    }                               \n"
                       " }                                  \n";
    CodeStream stream(code);
    Static<ParserContext>::Object.clear();

    try {
        std::vector<Token> tokens = Lexer::parser(&stream);
        TokenStream stream(tokens);
        std::cout << "Lexer Done!" << std::endl;

        Parser::Program(&stream);
        std::cout << "Parser Parser Done!" << std::endl;
    } catch(Throwable e) {
        e.printTrack();
        std::cout << e.getError()  << std::endl;
    }
    std::cout << "-----------------------------------------------" << std::endl;
}
