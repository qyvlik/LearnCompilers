#ifndef STATEMENTPARSER_H
#define STATEMENTPARSER_H

#include "lexerstream.hpp"

#include <iostream>
#include <string>
#include <vector>


/**
Break -> break

Continue -> continue

Expression -> ( true | false )

EchoStatement -> echo Expression ;

ExpressionStatement -> Expression;

IfStatement -> if ( Expression ) Statement ( ElseStatement )+

ElseStatement -> else Statement

BreakStatement -> Break ;

ContinueStatement -> Continue ;

DoWhileStatement -> do Statement while( Expression ) ;

WhileStatement -> while ( Expression ) Statement

ForStatement -> for( Expression, Expression, Expression ) Statement

TryStatement -> try Block CatchStatement

CatchStatement -> catch Block

Block -> { Statements }

Statements -> ( Statement )*

Statement -> EchoStatement
           | ExpressionStatement
           | IfStatement
           | WhileStatement
           | ForStatement
           | BreakStatement
           | ContinueStatement
           | TryStatement
           | Block
*/

class StatementParser
{
public:

    StatementParser()
    { }


    // Statements -> ( Statement )*
    static void Statements(LexerStream* lexers) throw(std::string)
    {}


    //    Statement -> EchoStatement
    //               | ExpressionStatement
    //               | IfStatement
    //               | WhileStatement
    //               | ForStatement
    //               | BreakStatement
    //               | ContinueStatement
    //               | TryStatement
    //               | Block
    static void Statement(LexerStream* lexers) throw(std::string)
    {}

    // EchoStatement -> echo Expression ;
    static void EchoStatement(LexerStream* lexers) throw(std::string)
    {}

    // ExpressionStatement -> Expression;
    static void ExpressionStatement(LexerStream* lexers) throw(std::string)
    {}


    // IfStatement -> if ( Expression ) Statement ( ElseStatement )+
    static void IfStatement(LexerStream* lexers) throw(std::string)
    {}


    // DoWhileStatement -> do Statement while( Expression ) ;
    static void DoWhileStatement(LexerStream* lexers) throw(std::string)
    {}


    // WhileStatement -> while ( Expression ) Statement
    static void WhileStatement(LexerStream* lexers) throw(std::string)
    {}


    // ForStatement -> for( Expression, Expression, Expression ) Statement
    static void ForStatement(LexerStream* lexers) throw(std::string)
    {}


    // BreakStatement -> Break ;
    static void BreakStatement(LexerStream* lexers) throw(std::string)
    {}


    // ContinueStatement -> Continue ;
    static void ContinueStatement(LexerStream* lexers) throw(std::string)
    {}


    // TryStatement -> try Block CatchStatement
    static void TryStatement(LexerStream* lexers) throw(std::string)
    {}


    // CatchStatement -> catch Block
    static void CatchStatement(LexerStream* lexers) throw(std::string)
    {}

    // Block -> { Statements }
    static void Block(LexerStream* lexers) throw(std::string)
    {}


    // Break -> break
    static void Break(LexerStream* lexers) throw(std::string)
    {}


    // Continue -> continue
    static void Continue(LexerStream* lexers) throw(std::string)
    {}


    // Expression -> ( true | false )
    static void Expression(LexerStream* lexers) throw(std::string)
    {}


};

#endif // STATEMENTPARSER_H
