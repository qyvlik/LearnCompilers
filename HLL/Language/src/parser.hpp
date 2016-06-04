#ifndef PARSER_H
#define PARSER_H

#include "tokenstream.hpp"

class Parser
{
public:
    // Statements ::= { Statement }
    static void Statements(TokenStream* stream) throw (std::string)
    {}


    // Statement ::= ExpressionStatement
    //             | IfStatement
    //             | WhileStatement
    //             | DoWhileStatement
    //             | ForStatement
    //             | BreakStatement
    //             | ContinueStatement
    //             | TryStatement
    //             | Block
    static void Statement(TokenStream* stream) throw (std::string)
    {}


    // ExpressionStatement ::= Expression ";" | ";"
    static void ExpressionStatement(TokenStream* stream) throw (std::string)
    {}


    // Expression ::= ObjectExpression { ( "=" | "+=" | "-=" | "*=" | "%=" | "/=" ) BoolExpression } | BoolExpression
    static void Expression(TokenStream* stream) throw (std::string)
    {}


    // BoolExpression ::= ArithmeticExpression { ( ">" | "<" | ">=" | "<=" | "==" | "!=" ) ArithmeticExpression }
    static void BoolExpression(TokenStream* stream) throw (std::string)
    {}


    // ArithmeticExpression ::= TermExpression { ( "+" | "-" ) TermExpression }
    static void ArithmeticExpression(TokenStream* stream) throw (std::string)
    {}


    // TermExpression ::= FactorExpression { ( "*" | "/" | "%" ) FactorExpression }
    static void TermExpression(TokenStream* stream) throw (std::string)
    {}


    // FactorExpression ::= "(" Expression ")" | ObjectExpression | Lterial
    static void FactorExpression(TokenStream* stream) throw (std::string)
    {}


    // ObjectExpression ::= ObjectExpression "." Identity
    //                    | ObjectExpression "[" Expression "]"
    //                    | ObjectExpression "(" CallArgumentList ")"
    //                    | Identity
    static void ObjectExpression(TokenStream* stream) throw (std::string)
    {}


    // CallArgumentList ::= ε | Expression { "," Expression }
    static void CallArgumentList(TokenStream* stream) throw (std::string)
    {}


    // Lterial ::= StringLterial | NumberLterial | ArrayLterial | KeyValuesLterial
    static void Lterial(TokenStream* stream) throw (std::string)
    {}


    // KeyValuesLterial ::= "{" { StringLterial ":" Expression "," } "}"           // map 或者说是对象，不能对此进行函数调用
    static void KeyValuesLterial(TokenStream* stream) throw (std::string)
    {}


    // ArrayLterial ::= "[" { Expression "," } "]"                                 // 数组，不能对此进行函数调用
    static void ArrayLterial(TokenStream* stream) throw (std::string)
    {}


    static void NumberLterial(TokenStream* stream) throw(std::string)
    {}


    static void StringLterial(TokenStream* stream) throw(std::string)
    {}


    static void Identity(TokenStream* stream) throw(std::string)
    {}


    // IfStatement ::= "if" "(" Expression ")" Statement [ ElseStatement ]
    static void IfStatement(TokenStream* stream) throw (std::string)
    {}


    // ElseStatement ::= "else" Statement
    static void ElseStatement(TokenStream* stream) throw (std::string)
    {}


    // DoWhileStatement ::= "do" Statement "while" "(" Expression ")" ";"
    static void DoWhileStatement(TokenStream* stream) throw (std::string)
    {}


    // WhileStatement ::= "while" "(" Expression ")" Statement
    static void WhileStatement(TokenStream* stream) throw (std::string)
    {}


    // ForStatement ::= "for" "(" Expression ";" Expression ";" Expression ")" Statement
    static void ForStatement(TokenStream* stream) throw (std::string)
    {}


    // TryStatement ::= "try" Block CatchStatement
    static void TryStatement(TokenStream* stream) throw (std::string)
    {}


    // CatchStatement ::= "catch" Block
    static void CatchStatement(TokenStream* stream) throw (std::string)
    {}


    // Block ::= "{" Statements "}"
    static void Block(TokenStream* stream) throw (std::string)
    {}


    // Break ::= "break"
    static void Break(TokenStream* stream) throw (std::string)
    {}


    // Continue ::= "continue"
    static void Continue(TokenStream* stream) throw (std::string)
    {}


    // BreakStatement ::= Break ";"
    static void BreakStatement(TokenStream* stream) throw (std::string)
    {}


    // ContinueStatement ::= Continue ";"
    static void ContinueStatement(TokenStream* stream) throw (std::string)
    {}

};

#endif // PARSER_H
