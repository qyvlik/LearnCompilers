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

int main()
{
    test_1();
    test_2();
    test_3();
    test_4();
    test_5();
    test_6();
    test_7();
    test_8();
    test_9();
    test_10();
    test_11();
    test_12();
    test_13();
    test_14();
    test_15();
    return 0;
}


void test_1()
{
    vector<Token> tokens;

    // A . B . C [ "D" ] ( 1, 2, 3 ) [ "A" ] ;
    tokens.push_back(Token(Token::Identity, "A"));
    tokens.push_back(Token(Token::Delimiter, "."));
    tokens.push_back(Token(Token::Identity, "B"));
    tokens.push_back(Token(Token::Delimiter, "."));
    tokens.push_back(Token(Token::Identity, "C"));
    tokens.push_back(Token(Token::Delimiter, "["));
    tokens.push_back(Token(Token::StringLiteral, "\"D\""));
    tokens.push_back(Token(Token::Delimiter, "]"));
    tokens.push_back(Token(Token::Delimiter, "("));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Delimiter, ","));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Delimiter, ","));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Delimiter, ")"));
    tokens.push_back(Token(Token::Delimiter, "["));
    tokens.push_back(Token(Token::StringLiteral, "\"A\""));
    tokens.push_back(Token(Token::Delimiter, "]"));
    tokens.push_back(Token(Token::Delimiter, ";"));

    TokenStream stream(tokens);

    try {
        Parser::ExpressionStatement(&stream);
        std::cout << "Done!" << std::endl;
    } catch(Throwable e) {
        std::cout << "error:" << std::endl;
        std::cout << e << std::endl;
        e.printTrack();
    }
}

void test_2()
{
    vector<Token> tokens;

    // A = { "name" : 1 , "age": 1/1 } ;

    tokens.push_back(Token(Token::Identity, "A"));
    tokens.push_back(Token(Token::Operator, "="));
    tokens.push_back(Token(Token::Delimiter, "{"));
    tokens.push_back(Token(Token::StringLiteral, "\"name\""));
    tokens.push_back(Token(Token::Delimiter, ":"));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Delimiter, ","));
    tokens.push_back(Token(Token::StringLiteral, "\"age\""));
    tokens.push_back(Token(Token::Delimiter, ":"));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Operator, "/"));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Delimiter, "}"));
    tokens.push_back(Token(Token::Delimiter, ";"));

    TokenStream stream(tokens);

    try {
        Parser::ExpressionStatement(&stream);
        std::cout << "Done!" << std::endl;
    } catch(Throwable e) {
        std::cout << "error:" << std::endl;
        std::cout << e << std::endl;
        e.printTrack();
    }
}

void test_3()
{
    vector<Token> tokens;

    // A = [ "age", 1, ] ;

    tokens.push_back(Token(Token::Identity, "A"));
    tokens.push_back(Token(Token::Operator, "="));
    tokens.push_back(Token(Token::Delimiter, "["));
    tokens.push_back(Token(Token::StringLiteral, "\"age\""));
    tokens.push_back(Token(Token::Delimiter, ","));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Delimiter, ","));
    tokens.push_back(Token(Token::Delimiter, "]"));
    tokens.push_back(Token(Token::Delimiter, ";"));

    TokenStream stream(tokens);

    try {
        Parser::ExpressionStatement(&stream);
        std::cout << "Done!" << std::endl;
    } catch(Throwable e) {
        std::cout << "error:" << std::endl;
        std::cout << e << std::endl;
        e.printTrack();
    }
}

void test_4()
{
    vector<Token> tokens;

    // A . B . C [ "D" ] ( 1, 2, 3 ) [ "A" ] = { "name" : 1 , "age": 1/1 , "array" : [ "age", 1, ] } ;
    tokens.push_back(Token(Token::Identity, "A"));
    tokens.push_back(Token(Token::Delimiter, "."));
    tokens.push_back(Token(Token::Identity, "B"));
    tokens.push_back(Token(Token::Delimiter, "."));
    tokens.push_back(Token(Token::Identity, "C"));
    tokens.push_back(Token(Token::Delimiter, "["));
    tokens.push_back(Token(Token::StringLiteral, "\"D\""));
    tokens.push_back(Token(Token::Delimiter, "]"));
    tokens.push_back(Token(Token::Delimiter, "("));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Delimiter, ","));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Delimiter, ","));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Delimiter, ")"));
    tokens.push_back(Token(Token::Delimiter, "["));
    tokens.push_back(Token(Token::StringLiteral, "\"A\""));
    tokens.push_back(Token(Token::Delimiter, "]"));

    tokens.push_back(Token(Token::Operator, "="));

    tokens.push_back(Token(Token::Delimiter, "{"));
    tokens.push_back(Token(Token::StringLiteral, "\"name\""));
    tokens.push_back(Token(Token::Delimiter, ":"));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Delimiter, ","));
    tokens.push_back(Token(Token::StringLiteral, "\"age\""));
    tokens.push_back(Token(Token::Delimiter, ":"));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Operator, "/"));
    tokens.push_back(Token(Token::NumberLiteral, "1"));

    tokens.push_back(Token(Token::Delimiter, ","));

    tokens.push_back(Token(Token::StringLiteral, "\"array\""));
    tokens.push_back(Token(Token::Delimiter, ":"));

    tokens.push_back(Token(Token::Delimiter, "["));
    tokens.push_back(Token(Token::StringLiteral, "\"age\""));
    tokens.push_back(Token(Token::Delimiter, ","));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Delimiter, ","));
    tokens.push_back(Token(Token::Delimiter, "]"));

    tokens.push_back(Token(Token::Delimiter, "}"));

    tokens.push_back(Token(Token::Delimiter, ";"));

    TokenStream stream(tokens);

    try {
        Parser::ExpressionStatement(&stream);
        std::cout << "Done!" << std::endl;
    } catch(Throwable e) {
        std::cout << "error:" << std::endl;
        std::cout << e << std::endl;
        e.printTrack();
    }
}

void test_5()
{
    vector<Token> tokens;

    // try { if ( 1 ) { while ( 1 ) { } } } catch ( 1 ) { for ( 1 ; 1 ; 1 ) { do { } while ( 1 ) ; } }
    tokens.push_back(Token(Token::KeyWord, "try"));
    tokens.push_back(Token(Token::Delimiter, "{"));  // try
    tokens.push_back(Token(Token::KeyWord, "if"));
    tokens.push_back(Token(Token::Delimiter, "("));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Delimiter, ")"));
    tokens.push_back(Token(Token::Delimiter, "{"));  // if
    tokens.push_back(Token(Token::KeyWord, "while"));
    tokens.push_back(Token(Token::Delimiter, "("));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Delimiter, ")"));
    tokens.push_back(Token(Token::Delimiter, "{"));
    tokens.push_back(Token(Token::Delimiter, "}"));  // while
    tokens.push_back(Token(Token::Delimiter, "}"));  // if
    tokens.push_back(Token(Token::Delimiter, "}"));  // try
    tokens.push_back(Token(Token::KeyWord, "catch"));
    tokens.push_back(Token(Token::Delimiter, "("));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Delimiter, ")"));
    tokens.push_back(Token(Token::Delimiter, "{"));  // catch
    tokens.push_back(Token(Token::KeyWord, "for"));
    tokens.push_back(Token(Token::Delimiter, "("));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Delimiter, ";"));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Delimiter, ";"));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Delimiter, ")"));
    tokens.push_back(Token(Token::Delimiter, "{"));  // for
    tokens.push_back(Token(Token::KeyWord, "do"));
    tokens.push_back(Token(Token::Delimiter, "{"));  // do-while
    tokens.push_back(Token(Token::Delimiter, "}"));  // do-while
    tokens.push_back(Token(Token::KeyWord, "while"));
    tokens.push_back(Token(Token::Delimiter, "("));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Delimiter, ")"));
    tokens.push_back(Token(Token::Delimiter, ";"));
    tokens.push_back(Token(Token::Delimiter, "}"));  // for
    tokens.push_back(Token(Token::Delimiter, "}"));  // catch


    TokenStream stream(tokens);

    try {
        Parser::Statements(&stream);
        std::cout << "Done!" << std::endl;
    } catch(Throwable e) {
        std::cout << "error:" << std::endl;
        std::cout << e << std::endl;
        e.printTrack();
    }
}


void test_6()
{
    vector<Token> tokens;

    // try { if ( 1 ) { while ( 1 ) {
    // A . B . C [ "D" ] ( 1, 2, 3 ) [ "A" ] = { "name" : 1 , "age": 1/1 , "array" : [ "age", 1, ] } ;
    // } } } catch ( 1 ) { for ( 1 ; 1 ; 1 ) { do { } while ( 1 ) ; } }
    tokens.push_back(Token(Token::KeyWord, "try"));
    tokens.push_back(Token(Token::Delimiter, "{"));  // try
    tokens.push_back(Token(Token::KeyWord, "if"));
    tokens.push_back(Token(Token::Delimiter, "("));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Delimiter, ")"));
    tokens.push_back(Token(Token::Delimiter, "{"));  // if
    tokens.push_back(Token(Token::KeyWord, "while"));
    tokens.push_back(Token(Token::Delimiter, "("));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Delimiter, ")"));
    tokens.push_back(Token(Token::Delimiter, "{"));  // while

    tokens.push_back(Token(Token::Identity, "A"));
    tokens.push_back(Token(Token::Delimiter, "."));
    tokens.push_back(Token(Token::Identity, "B"));
    tokens.push_back(Token(Token::Delimiter, "."));
    tokens.push_back(Token(Token::Identity, "C"));
    tokens.push_back(Token(Token::Delimiter, "["));
    tokens.push_back(Token(Token::StringLiteral, "\"D\""));
    tokens.push_back(Token(Token::Delimiter, "]"));
    tokens.push_back(Token(Token::Delimiter, "("));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Delimiter, ","));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Delimiter, ","));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Delimiter, ")"));
    tokens.push_back(Token(Token::Delimiter, "["));
    tokens.push_back(Token(Token::StringLiteral, "\"A\""));
    tokens.push_back(Token(Token::Delimiter, "]"));

    tokens.push_back(Token(Token::Operator, "="));

    tokens.push_back(Token(Token::Delimiter, "{"));
    tokens.push_back(Token(Token::StringLiteral, "\"name\""));
    tokens.push_back(Token(Token::Delimiter, ":"));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Delimiter, ","));
    tokens.push_back(Token(Token::StringLiteral, "\"age\""));
    tokens.push_back(Token(Token::Delimiter, ":"));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Operator, "/"));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Delimiter, ","));
    tokens.push_back(Token(Token::StringLiteral, "\"array\""));
    tokens.push_back(Token(Token::Delimiter, ":"));
    tokens.push_back(Token(Token::Delimiter, "["));
    tokens.push_back(Token(Token::StringLiteral, "\"age\""));
    tokens.push_back(Token(Token::Delimiter, ","));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Delimiter, ","));
    tokens.push_back(Token(Token::Delimiter, "]"));
    tokens.push_back(Token(Token::Delimiter, "}"));
    tokens.push_back(Token(Token::Delimiter, ";"));

    tokens.push_back(Token(Token::Delimiter, "}"));  // while
    tokens.push_back(Token(Token::Delimiter, "}"));  // if
    tokens.push_back(Token(Token::Delimiter, "}"));  // try
    tokens.push_back(Token(Token::KeyWord, "catch"));
    tokens.push_back(Token(Token::Delimiter, "("));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Delimiter, ")"));
    tokens.push_back(Token(Token::Delimiter, "{"));  // catch
    tokens.push_back(Token(Token::KeyWord, "for"));
    tokens.push_back(Token(Token::Delimiter, "("));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Delimiter, ";"));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Delimiter, ";"));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Delimiter, ")"));
    tokens.push_back(Token(Token::Delimiter, "{"));  // for
    tokens.push_back(Token(Token::KeyWord, "do"));
    tokens.push_back(Token(Token::Delimiter, "{"));  // do-while
    tokens.push_back(Token(Token::Delimiter, "}"));  // do-while
    tokens.push_back(Token(Token::KeyWord, "while"));
    tokens.push_back(Token(Token::Delimiter, "("));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Delimiter, ")"));
    tokens.push_back(Token(Token::Delimiter, ";"));
    tokens.push_back(Token(Token::Delimiter, "}"));  // for
    tokens.push_back(Token(Token::Delimiter, "}"));  // catch


    TokenStream stream(tokens);

    try {
        Parser::Statements(&stream);
        std::cout << "Done!" << std::endl;
    } catch(Throwable e) {
        std::cout << "error:" << std::endl;
        std::cout << e << std::endl;
        e.printTrack();
    }
}

void test_7()
{
    vector<Token> tokens;

    //{ "name" : 1  "age": 1/1 } ;

    tokens.push_back(Token(Token::Delimiter, "{"));
    tokens.push_back(Token(Token::StringLiteral, "\"name\""));
    tokens.push_back(Token(Token::Delimiter, ":"));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    //    tokens.push_back(Token(Token::Delimiter, ","));
    tokens.push_back(Token(Token::StringLiteral, "\"age\""));
    tokens.push_back(Token(Token::Delimiter, ":"));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Operator, "/"));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Delimiter, "}"));
    tokens.push_back(Token(Token::Delimiter, ";"));

    TokenStream stream(tokens);

    try {
        Parser::ExpressionStatement(&stream);
        std::cout << "Done!" << std::endl;
    } catch(Throwable e) {
        std::cout << "error:" << std::endl;
        std::cout << e << std::endl;
        e.printTrack();
    }

}

void test_8()
{
    // function (int a, int b) -> int { return a; }
    vector<Token> tokens;

    tokens.push_back(Token(Token::KeyWord, "function"));
    tokens.push_back(Token(Token::Delimiter, "("));
    tokens.push_back(Token(Token::TypeName, "int"));
    tokens.push_back(Token(Token::Identity, "a"));
    tokens.push_back(Token(Token::Delimiter, ","));
    tokens.push_back(Token(Token::TypeName, "int"));
    tokens.push_back(Token(Token::Identity, "b"));
    tokens.push_back(Token(Token::Delimiter, ")"));

    tokens.push_back(Token(Token::Delimiter, "->"));

    tokens.push_back(Token(Token::TypeName, "int"));


    tokens.push_back(Token(Token::Delimiter, "{"));

    tokens.push_back(Token(Token::KeyWord, "return"));

    tokens.push_back(Token(Token::Identity, "a"));
    tokens.push_back(Token(Token::Delimiter, ";"));

    tokens.push_back(Token(Token::Delimiter, "}"));

    TokenStream stream(tokens);

    try {
        Parser::Lambda(&stream);
        std::cout << "Done!" << std::endl;
    } catch(Throwable e) {
        std::cout << "error:" << std::endl;
        std::cout << e << std::endl;
        e.printTrack();
    }
}

void test_9()
{
    // int a = 10, b = 11;
    vector<Token> tokens;

    tokens.push_back(Token(Token::TypeName, "int"));
    tokens.push_back(Token(Token::Identity, "a"));
    tokens.push_back(Token(Token::Operator, "="));
    tokens.push_back(Token(Token::NumberLiteral, "10"));
    tokens.push_back(Token(Token::Delimiter, ","));
    tokens.push_back(Token(Token::TypeName, "int"));
    tokens.push_back(Token(Token::Identity, "b"));
    tokens.push_back(Token(Token::Operator, "="));

    tokens.push_back(Token(Token::NumberLiteral, "10"));

    tokens.push_back(Token(Token::Delimiter, ";"));

    TokenStream stream(tokens);

    try {
        Parser::DeclarationStatement(&stream);
        std::cout << "Done!" << std::endl;
    } catch(Throwable e) {
        std::cout << "error:" << std::endl;
        std::cout << e << std::endl;
        e.printTrack();
    }
}

void test_10()
{
    // function add(int a, int b) -> int { return a + b ; }
    // int result = add(1, 1);
    vector<Token> tokens;

    tokens.push_back(Token(Token::KeyWord, "function"));

    tokens.push_back(Token(Token::Identity, "add"));
    tokens.push_back(Token(Token::Delimiter, "("));
    tokens.push_back(Token(Token::TypeName, "int"));
    tokens.push_back(Token(Token::Identity, "a"));
    tokens.push_back(Token(Token::Delimiter, ","));
    tokens.push_back(Token(Token::TypeName, "int"));
    tokens.push_back(Token(Token::Identity, "b"));
    tokens.push_back(Token(Token::Delimiter, ")"));
    tokens.push_back(Token(Token::Delimiter, "->"));
    tokens.push_back(Token(Token::TypeName, "int"));
    tokens.push_back(Token(Token::Delimiter, "{"));
    tokens.push_back(Token(Token::KeyWord, "return"));

    tokens.push_back(Token(Token::Identity, "a"));
    tokens.push_back(Token(Token::Operator, "+"));
    tokens.push_back(Token(Token::Identity, "b"));

    tokens.push_back(Token(Token::Delimiter, ";"));
    tokens.push_back(Token(Token::Delimiter, "}"));

    tokens.push_back(Token(Token::TypeName, "int"));
    tokens.push_back(Token(Token::Identity, "result"));
    tokens.push_back(Token(Token::Operator, "="));

    tokens.push_back(Token(Token::Identity, "add"));

    tokens.push_back(Token(Token::Delimiter, "("));

    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Delimiter, ","));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Delimiter, ")"));

    tokens.push_back(Token(Token::Delimiter, ";"));

    TokenStream stream(tokens);

    try {
        Parser::Program(&stream);
        std::cout << "Done!" << std::endl;
    } catch(Throwable e) {
        std::cout << "error:" << std::endl;
        std::cout << e << std::endl;
        e.printTrack();
    }
}

void test_11()
{
    // function add(int a, int b) -> var { return function () -> int { return a + b ; } ; }
    // int result = add(1, 1)();

    vector<Token> tokens;

    tokens.push_back(Token(Token::KeyWord, "function"));

    tokens.push_back(Token(Token::Identity, "add"));
    tokens.push_back(Token(Token::Delimiter, "("));
    tokens.push_back(Token(Token::TypeName, "int"));
    tokens.push_back(Token(Token::Identity, "a"));
    tokens.push_back(Token(Token::Delimiter, ","));
    tokens.push_back(Token(Token::TypeName, "int"));
    tokens.push_back(Token(Token::Identity, "b"));
    tokens.push_back(Token(Token::Delimiter, ")"));
    tokens.push_back(Token(Token::Delimiter, "->"));
    tokens.push_back(Token(Token::TypeName, "var"));
    tokens.push_back(Token(Token::Delimiter, "{"));
    tokens.push_back(Token(Token::KeyWord, "return"));

    tokens.push_back(Token(Token::KeyWord, "function"));

    // tokens.push_back(Token(Token::Identity, "add0"));

    tokens.push_back(Token(Token::Delimiter, "("));
    tokens.push_back(Token(Token::Delimiter, ")"));
    tokens.push_back(Token(Token::Delimiter, "->"));
    tokens.push_back(Token(Token::TypeName, "int"));
    tokens.push_back(Token(Token::Delimiter, "{"));
    tokens.push_back(Token(Token::KeyWord, "return"));

    tokens.push_back(Token(Token::Identity, "a"));
    tokens.push_back(Token(Token::Operator, "+"));
    tokens.push_back(Token(Token::Identity, "b"));

    tokens.push_back(Token(Token::Delimiter, ";"));
    tokens.push_back(Token(Token::Delimiter, "}"));

    tokens.push_back(Token(Token::Delimiter, ";"));
    tokens.push_back(Token(Token::Delimiter, "}"));



    tokens.push_back(Token(Token::TypeName, "int"));
    tokens.push_back(Token(Token::Identity, "result"));
    tokens.push_back(Token(Token::Operator, "="));
    tokens.push_back(Token(Token::Identity, "add"));
    tokens.push_back(Token(Token::Delimiter, "("));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Delimiter, ","));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Delimiter, ")"));
    tokens.push_back(Token(Token::Delimiter, "("));
    tokens.push_back(Token(Token::Delimiter, ")"));

    tokens.push_back(Token(Token::Delimiter, ";"));

    TokenStream stream(tokens);

    try {
        Parser::Program(&stream);
        std::cout << "Done!" << std::endl;
    } catch(Throwable e) {
        std::cout << "error:" << std::endl;
        std::cout << e << std::endl;
        e.printTrack();
    }
}


void test_12()
{
    // function add(int a, int b) -> var { return function () -> int { return a + b ; } ; }
    // function sub(int a, int b) -> var {  return a - b ; }
    // int result = add(1, 1)();
    // int result2 = sub(result, result);

    vector<Token> tokens;

    tokens.push_back(Token(Token::KeyWord, "function"));
    tokens.push_back(Token(Token::Identity, "add"));
    tokens.push_back(Token(Token::Delimiter, "("));
    tokens.push_back(Token(Token::TypeName, "int"));
    tokens.push_back(Token(Token::Identity, "a"));
    tokens.push_back(Token(Token::Delimiter, ","));
    tokens.push_back(Token(Token::TypeName, "int"));
    tokens.push_back(Token(Token::Identity, "b"));
    tokens.push_back(Token(Token::Delimiter, ")"));
    tokens.push_back(Token(Token::Delimiter, "->"));
    tokens.push_back(Token(Token::TypeName, "var"));
    tokens.push_back(Token(Token::Delimiter, "{"));
    tokens.push_back(Token(Token::KeyWord, "return"));
    tokens.push_back(Token(Token::KeyWord, "function"));
    tokens.push_back(Token(Token::Delimiter, "("));
    tokens.push_back(Token(Token::Delimiter, ")"));
    tokens.push_back(Token(Token::Delimiter, "->"));
    tokens.push_back(Token(Token::TypeName, "int"));
    tokens.push_back(Token(Token::Delimiter, "{"));
    tokens.push_back(Token(Token::KeyWord, "return"));
    tokens.push_back(Token(Token::Identity, "a"));
    tokens.push_back(Token(Token::Operator, "+"));
    tokens.push_back(Token(Token::Identity, "b"));
    tokens.push_back(Token(Token::Delimiter, ";"));
    tokens.push_back(Token(Token::Delimiter, "}"));
    tokens.push_back(Token(Token::Delimiter, ";"));
    tokens.push_back(Token(Token::Delimiter, "}"));

    tokens.push_back(Token(Token::KeyWord, "function"));
    tokens.push_back(Token(Token::Identity, "sub"));
    tokens.push_back(Token(Token::Delimiter, "("));
    tokens.push_back(Token(Token::TypeName, "int"));
    tokens.push_back(Token(Token::Identity, "a"));
    tokens.push_back(Token(Token::Delimiter, ","));
    tokens.push_back(Token(Token::TypeName, "int"));
    tokens.push_back(Token(Token::Identity, "b"));
    tokens.push_back(Token(Token::Delimiter, ")"));
    tokens.push_back(Token(Token::Delimiter, "->"));
    tokens.push_back(Token(Token::TypeName, "var"));
    tokens.push_back(Token(Token::Delimiter, "{"));
    tokens.push_back(Token(Token::KeyWord, "return"));
    tokens.push_back(Token(Token::Identity, "a"));
    tokens.push_back(Token(Token::Operator, "-"));
    tokens.push_back(Token(Token::Identity, "b"));
    tokens.push_back(Token(Token::Delimiter, ";"));
    tokens.push_back(Token(Token::Delimiter, "}"));

    tokens.push_back(Token(Token::TypeName, "int"));
    tokens.push_back(Token(Token::Identity, "result"));
    tokens.push_back(Token(Token::Operator, "="));
    tokens.push_back(Token(Token::Identity, "add"));
    tokens.push_back(Token(Token::Delimiter, "("));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Delimiter, ","));
    tokens.push_back(Token(Token::NumberLiteral, "1"));
    tokens.push_back(Token(Token::Delimiter, ")"));
    tokens.push_back(Token(Token::Delimiter, "("));
    tokens.push_back(Token(Token::Delimiter, ")"));
    tokens.push_back(Token(Token::Delimiter, ";"));

    tokens.push_back(Token(Token::TypeName, "int"));
    tokens.push_back(Token(Token::Identity, "result0"));
    tokens.push_back(Token(Token::Operator, "="));
    tokens.push_back(Token(Token::Identity, "sub"));
    tokens.push_back(Token(Token::Delimiter, "("));
    tokens.push_back(Token(Token::NumberLiteral, "result"));
    tokens.push_back(Token(Token::Delimiter, ","));
    tokens.push_back(Token(Token::NumberLiteral, "result"));
    tokens.push_back(Token(Token::Delimiter, ")"));
    tokens.push_back(Token(Token::Delimiter, ";"));

    TokenStream stream(tokens);

    try {
        Parser::Program(&stream);
        std::cout << "Done!" << std::endl;
    } catch(Throwable e) {
        std::cout << "error:" << std::endl;
        std::cout << e << std::endl;
        e.printTrack();
    }
}

void test_13()
{
    // a = b;
    vector<Token> tokens;

    tokens.push_back(Token(Token::Identity, "a"));
    tokens.push_back(Token(Token::Operator, "="));
    tokens.push_back(Token(Token::Identity, "b"));
    tokens.push_back(Token(Token::Delimiter, ";"));

    TokenStream stream(tokens);

    try {
        Parser::Program(&stream);
        std::cout << "Done!" << std::endl;
    } catch(Throwable e) {
        std::cout << "error:" << std::endl;
        std::cout << e << std::endl;
        e.printTrack();
    }
}

void test_14()
{
    std::string code = "     try { if ( 1 ) { while ( 1.02312 ) {"
                       "     A . B . C [ \"D\" ] ( 1, 2, 3 ) [ \"A\" ] = { \"name\" : 1 , \"age\": 1/1 , \"array\" : [ \"age\", 1, ] } ;"
                       "     } } } catch ( 1 ) { for ( 1 ; 1 ; 1 ) { do { } while ( 1 ) ; } } ";
    CodeStream stream(code);
    try {
        std::vector<Token> tokens = Lexer::parser(&stream);
        TokenStream stream(tokens);
        std::cout << "Lexer Done!" << std::endl;

        Parser::Program(&stream);
        std::cout << "Parser Done!" << std::endl;

    } catch(Throwable e) {
        e.printTrack();
        std::cout << e.getError()  << std::endl;
    }
}

void test_15()
{
    std::string code = "     function add(int a, int b) -> var { return function () -> int { return a + b ; } ; }"
                       "     function sub(int a, int b) -> var {  return a - b ; }"
                       "     int result = add(1, 1)();"
                       "     int result2 = sub(result, result); ";
    CodeStream stream(code);
    try {
        std::vector<Token> tokens = Lexer::parser(&stream);
        TokenStream stream(tokens);
        std::cout << "Lexer Done!" << std::endl;

        Parser::Program(&stream);
        std::cout << "Parser Done!" << std::endl;

    } catch(Throwable e) {
        e.printTrack();
        std::cout << e.getError()  << std::endl;
    }
}
