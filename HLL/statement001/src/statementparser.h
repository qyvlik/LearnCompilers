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

ForStatement -> for ( Expression ; Expression ; Expression ) Statement

TryStatement -> try Block CatchStatement

CatchStatement -> catch Block

Block -> { Statements } | ε

Statements -> ( Statement )*

Statement -> EchoStatement
           | ExpressionStatement
           | IfStatement
           | WhileStatement
           | DoWhileStatement
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
    {
        while(!lexers->atEnd()) {
            Statement(lexers);
        }
    }


    //    Statement -> EchoStatement
    //               | ExpressionStatement
    //               | IfStatement
    //               | WhileStatement
    //               | DoWhileStatement
    //               | ForStatement
    //               | BreakStatement
    //               | ContinueStatement
    //               | TryStatement
    //               | Block
    static void Statement(LexerStream* lexers) throw(std::string)
    {
        //lexers->next();
        std::string ahead = lexers->current();
        //lexers->rockBack();

        if(ahead == "echo") {
            EchoStatement(lexers);
        } else if(ahead == "if") {
            IfStatement(lexers);
        } else if(ahead == "do") {
            DoWhileStatement(lexers);
        } else if(ahead == "while") {
            WhileStatement(lexers);
        } else if(ahead == "for") {
            ForStatement(lexers);
        } else if(ahead == "continue") {
            ContinueStatement(lexers);
        } else if(ahead == "break") {
            BreakStatement(lexers);
        } else if(ahead == "{") {
            Block(lexers);
        }  else if(ahead == "try") {
            TryStatement(lexers);
        } else  {
            ExpressionStatement(lexers);
        }
    }

    // EchoStatement -> echo Expression ;
    static void EchoStatement(LexerStream* lexers) throw(std::string)
    {
        // first is `echo
        lexers->next();

        Expression(lexers);

        // lexers->next();

        if(lexers->current() != ";") {
            throw std::string("EchoStatement lost `;");
        }

        lexers->next();
    }


    // ExpressionStatement -> Expression ;
    static void ExpressionStatement(LexerStream* lexers) throw(std::string)
    {
        Expression(lexers);

        // lexers->next();

        if(lexers->current() != ";") {
            throw std::string("ExpressionStatement lost `;");
        }

        lexers->next();
    }


    // IfStatement -> if ( Expression ) Statement ( ElseStatement )+
    static void IfStatement(LexerStream* lexers) throw(std::string)
    {
        // first is `if
        lexers->next();

        if(lexers->current() != "(") {
            throw std::string("IfStatement lost `(");
        }

        lexers->next();

        Expression(lexers);

        if(lexers->current() != ")") {
            throw std::string("IfStatement lost `)");
        }

        lexers->next();

        Statement(lexers);

        std::string ahead = lexers->current();

        if(ahead == "else") {
            ElseStatement(lexers);
        }
    }

    // ElseStatement -> else Statement
    static void ElseStatement(LexerStream* lexers) throw(std::string)
    {
        // first is `else
        lexers->next();

        Statement(lexers);
    }


    // DoWhileStatement -> do Statement while( Expression ) ;
    static void DoWhileStatement(LexerStream* lexers) throw(std::string)
    {
        // first is `do
        lexers->next();

        Statement(lexers);

        // lexers->next();

        if(lexers->current() != "while") {
            throw std::string("DoWhileStatement lost `while");
        }

        lexers->next();

        if(lexers->current() != "(") {
            throw std::string("DoWhileStatement while lost `(");
        }

        lexers->next();

        Expression(lexers);

        if(lexers->current() != ")") {
            throw std::string("DoWhileStatement while lost `)");
        }

        lexers->next();

        if(lexers->current() != ";") {
            throw std::string("DoWhileStatement while lost `;");
        }

        lexers->next();

    }


    // WhileStatement -> while ( Expression ) Statement
    static void WhileStatement(LexerStream* lexers) throw(std::string)
    {
        // first is `while
        lexers->next();

        if(lexers->current() != "(") {
            throw std::string("WhileStatement lost `(");
        }

        lexers->next();

        Expression(lexers);

        // lexers->next();

        if(lexers->current() != ")") {
            throw std::string("WhileStatement lost `)");
        }

        lexers->next();

        Statement(lexers);
    }


    // ForStatement -> for ( Expression ; Expression ; Expression ) Statement
    static void ForStatement(LexerStream* lexers) throw(std::string)
    {
        // first is `for

        lexers->next();

        if(lexers->current() != "(") {
            throw std::string("ForStatement lost `(");
        }

        lexers->next();

        Expression(lexers);

        if(lexers->current() != ";") {
            throw std::string("ForStatement lost frist `;");
        }

        lexers->next();

        Expression(lexers);

        if(lexers->current() != ";") {
            throw std::string("ForStatement lost second `;");
        }

        lexers->next();

        Expression(lexers);

        if(lexers->current() != ")") {
            throw std::string("ForStatement lost `)");
        }

        lexers->next();

        Statement(lexers);
    }


    // BreakStatement -> Break ;
    static void BreakStatement(LexerStream* lexers) throw(std::string)
    {
        // first is `break
        Break(lexers);

        lexers->next();

        if(lexers->current() != ";") {
            throw std::string("BreakStatement lost `;");
        }

        lexers->next();
    }


    // ContinueStatement -> Continue ;
    static void ContinueStatement(LexerStream* lexers) throw(std::string)
    {
        // first is `continue
        Continue(lexers);

        lexers->next();

        if(lexers->current() != ";") {
            throw std::string("ContinueStatement lost `;");
        }

        lexers->next();
    }


    // TryStatement -> try Block CatchStatement
    static void TryStatement(LexerStream* lexers) throw(std::string)
    {
        // first is `try
        lexers->next();

        Block(lexers);

        if(lexers->current() != "catch") {
            throw std::string("TryStatement lost `catch");
        }

        CatchStatement(lexers);
    }


    // CatchStatement -> catch ( Expression ) Block
    static void CatchStatement(LexerStream* lexers) throw(std::string)
    {
        // first is `catch
        lexers->next();

        if(lexers->current() != "(") {
            throw std::string("CatchStatement lost )");
        }

        lexers->next();

        Expression(lexers);

        if(lexers->current() != ")") {
            throw std::string("CatchStatement lost `)");
        }

        lexers->next();

        Block(lexers);
    }

    // Block -> { Statements } | ε
    static void Block(LexerStream* lexers) throw(std::string)
    {
        // first is `{
        lexers->next();

        if(lexers->current() == "}") {
            lexers->next();
            return;
        }

        while(!lexers->atEnd()) {
            Statement(lexers);
            if(lexers->current() == "}") {
                break;
            }
        }

        lexers->next();
    }


    // Break -> break
    static void Break(LexerStream* lexers) throw(std::string)
    {
        (void)lexers;
    }


    // Continue -> continue
    static void Continue(LexerStream* lexers) throw(std::string)
    {
        (void)lexers;
    }


    // Expression -> ( true | false )
    static void Expression(LexerStream* lexers) throw(std::string)
    {
        (void)lexers;

        std::cout << "Expression :" << lexers->current() << "." << std::endl;

        lexers->next();

    }


};

#endif // STATEMENTPARSER_H
