#ifndef EXPRESSIONPARSER_H
#define EXPRESSIONPARSER_H

#include "tokenstream.hpp"

class ExpressionParser
{
public:
    static void ExpressionStatement(TokenStream* stream) throw(std::string)
    {}

    static void Expression(TokenStream* stream) throw(std::string)
    {}

    static void ArithmeticExpression(TokenStream* stream) throw(std::string)
    {}

    static void TermExpression(TokenStream* stream) throw(std::string)
    {}

    static void FactorExpression(TokenStream* stream) throw(std::string)
    {}

    static void RightValue(TokenStream* stream) throw(std::string)
    {}

    static void LeftValue(TokenStream* stream) throw(std::string)
    {}

    static void CallArgumentList(TokenStream* stream) throw(std::string)
    {}

    static void Lterial(TokenStream* stream) throw(std::string)
    {}

    static void NumberLterial(TokenStream* stream) throw(std::string)
    {}
};

#endif // EXPRESSIONPARSER_H
