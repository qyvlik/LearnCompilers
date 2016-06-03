#include "src/expressionparser.h"
#include "src/utility.hpp"

using namespace std;

void test_1();
void test_2();
void test_3();

int main()
{
    test_1();
    test_2();
    test_3();
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
    tokens.push_back(Token(Token::StringLterial, "\"D\""));
    tokens.push_back(Token(Token::Delimiter, "]"));
    tokens.push_back(Token(Token::Delimiter, "("));
    tokens.push_back(Token(Token::NumberLterial, "1"));
    tokens.push_back(Token(Token::Delimiter, ","));
    tokens.push_back(Token(Token::NumberLterial, "1"));
    tokens.push_back(Token(Token::Delimiter, ","));
    tokens.push_back(Token(Token::NumberLterial, "1"));
    tokens.push_back(Token(Token::Delimiter, ")"));
    tokens.push_back(Token(Token::Delimiter, "["));
    tokens.push_back(Token(Token::StringLterial, "\"A\""));
    tokens.push_back(Token(Token::Delimiter, "]"));
    tokens.push_back(Token(Token::Delimiter, ";"));

    TokenStream stream(tokens);

    try {
        ExpressionParser::ExpressionStatement(&stream);
        std::cout << "Done!" << std::endl;
    } catch(std::string e) {
        std::cout << "error:" << std::endl;
        std::cout << e << std::endl;
    }
}

void test_2()
{
    vector<Token> tokens;

    // A = { "name" : 1 , "age": 1/1 } ;

    tokens.push_back(Token(Token::Identity, "A"));
    tokens.push_back(Token(Token::Operator, "="));
    tokens.push_back(Token(Token::Delimiter, "{"));
    tokens.push_back(Token(Token::StringLterial, "\"name\""));
    tokens.push_back(Token(Token::Delimiter, ":"));
    tokens.push_back(Token(Token::NumberLterial, "1"));
    tokens.push_back(Token(Token::Delimiter, ","));
    tokens.push_back(Token(Token::StringLterial, "\"age\""));
    tokens.push_back(Token(Token::Delimiter, ":"));
    tokens.push_back(Token(Token::NumberLterial, "1"));
    tokens.push_back(Token(Token::Operator, "/"));
    tokens.push_back(Token(Token::NumberLterial, "1"));
    tokens.push_back(Token(Token::Delimiter, "}"));
    tokens.push_back(Token(Token::Delimiter, ";"));

    TokenStream stream(tokens);

    try {
        ExpressionParser::ExpressionStatement(&stream);
        std::cout << "Done!" << std::endl;
    } catch(std::string e) {
        std::cout << "error:" << std::endl;
        std::cout << e << std::endl;
    }
}

void test_3()
{
    vector<Token> tokens;

    // A = [ "age", 1, ] ;

    tokens.push_back(Token(Token::Identity, "A"));
    tokens.push_back(Token(Token::Operator, "="));
    tokens.push_back(Token(Token::Delimiter, "["));
    tokens.push_back(Token(Token::StringLterial, "\"age\""));
    tokens.push_back(Token(Token::Delimiter, ","));
    tokens.push_back(Token(Token::NumberLterial, "1"));
    tokens.push_back(Token(Token::Delimiter, ","));
    tokens.push_back(Token(Token::Delimiter, "]"));
    tokens.push_back(Token(Token::Delimiter, ";"));

    TokenStream stream(tokens);

    try {
        ExpressionParser::ExpressionStatement(&stream);
        std::cout << "Done!" << std::endl;
    } catch(std::string e) {
        std::cout << "error:" << std::endl;
        std::cout << e << std::endl;
    }
}
