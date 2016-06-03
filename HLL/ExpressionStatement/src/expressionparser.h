#ifndef EXPRESSIONPARSER_H
#define EXPRESSIONPARSER_H

#include "tokenstream.hpp"

class ExpressionParser
{
public:
    // ExpressionStatement ::= Expression ";" | ";"
    static void ExpressionStatement(TokenStream* stream) throw(std::string)
    {}


    // Expression ::= ObjectExpression { ( "=" | "+=" | "-=" | "*=" | "%=" | "/=" ) BoolExpression }
    //              | BoolExpression
    static void Expression(TokenStream* stream) throw(std::string)
    {}


    // BoolExpression ::= ArithmeticExpression { ( ">" | "<" | ">=" | "<=" | "==" | "!=" ) ArithmeticExpression }
    static void BoolExpression(TokenStream* stream) throw(std::string)
    {}


    // ArithmeticExpression ::= TermExpression { ( "+" | "-" ) TermExpression }
    static void ArithmeticExpression(TokenStream* stream) throw(std::string)
    {}


    // TermExpression ::= FactorExpression { ( "*" | "/" | "%" ) FactorExpression }
    static void TermExpression(TokenStream* stream) throw(std::string)
    {}


    // FactorExpression ::= "(" Expression ")"
    //                    | ObjectExpression
    //                    | Lterial
    static void FactorExpression(TokenStream* stream) throw(std::string)
    {}

    static void ObjectExpression(TokenStream* stream) throw(std::string)
    {}


    // CallArgumentList ::= ε | Expression { "," Expression }
    static void CallArgumentList(TokenStream* stream) throw(std::string)
    {}


    // Lterial ::= StringLterial | NumberLterial
    static void Lterial(TokenStream* stream) throw(std::string)
    {}

    // KeyValues ::= "{" { StringLterial ":" Expression "," } "}"         // map 或者说是对象，不能对此进行函数调用
    static void KeyValuesLterial(TokenStream* stream) throw(std::string)
    {}

    // LterialArray ::= "[" { Expression "," } "]"                           // 数组，不能对此进行函数调用
    static void ArrayLterial(TokenStream* stream) throw(std::string)
    {}

    static void NumberLterial(TokenStream* stream) throw(std::string)
    {}


    static void StringLterial(TokenStream* stream) throw(std::string)
    {}

    static void Identity(TokenStream* stream) throw(std::string)
    {}
};

#endif // EXPRESSIONPARSER_H
