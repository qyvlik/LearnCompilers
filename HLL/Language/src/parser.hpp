#ifndef PARSER_H
#define PARSER_H

#include "tokenstream.hpp"

class Parser
{
public:
    // Statements ::= { Statement }
    static void Statements(TokenStream* stream) throw (std::string)
    {
        while(!stream->atEnd()) {
            Statement(stream);
        }
    }


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
    {
        std::string ahead = stream->current().value;

        if(ahead == "if") {
            IfStatement(stream);
        } else if(ahead == "do") {
            DoWhileStatement(stream);
        } else if(ahead == "while") {
            WhileStatement(stream);
        } else if(ahead == "for") {
            ForStatement(stream);
        } else if(ahead == "continue") {
            ContinueStatement(stream);
        } else if(ahead == "break") {
            BreakStatement(stream);
        } else if(ahead == "{") {
            Block(stream);
        }  else if(ahead == "try") {
            TryStatement(stream);
        } else  {
            ExpressionStatement(stream);
        }
    }


    // ExpressionStatement ::= Expression ";" | ";"
    static void ExpressionStatement(TokenStream* stream) throw (std::string)
    {
        if(stream->current().value == ";") {
            stream->next();
        } else {
            Expression(stream);

            if(stream->current().value != ";") {
                std::cout << stream->current() << std::endl;
                throw std::string("ExpressionStatement: lost `;");
            }
            stream->next();
        }
    }


    // Expression ::= ObjectExpression { ( "=" | "+=" | "-=" | "*=" | "%=" | "/=" ) BoolExpression } | BoolExpression
    static void Expression(TokenStream* stream) throw (std::string)
    {
        if(stream->current().type == Token::Identity) {
            ObjectExpression(stream);

            std::string current = stream->current().value;
            while( current == "="  || current == "+="  ||
                   current == "-=" || current == "*=" ||
                   current == "/=" || current == "%="
                   )
            {
                stream->next();

                ArithmeticExpression(stream);

                current = stream->current().value;
            }
        } else {
            BoolExpression(stream);
        }
    }


    // BoolExpression ::= ArithmeticExpression { ( ">" | "<" | ">=" | "<=" | "==" | "!=" ) ArithmeticExpression }
    static void BoolExpression(TokenStream* stream) throw (std::string)
    {
        ArithmeticExpression(stream);

        std::string current = stream->current().value;
        while( current == ">"  || current == "<"  ||
               current == ">=" || current == "<=" ||
               current == "==" || current == "!="
               )
        {
            stream->next();

            ArithmeticExpression(stream);

            current = stream->current().value;
        }
    }


    // ArithmeticExpression ::= TermExpression { ( "+" | "-" ) TermExpression }
    static void ArithmeticExpression(TokenStream* stream) throw (std::string)
    {
        TermExpression(stream);
        std::string current = stream->current().value;
        while( current == "+" || current == "-" ) {
            stream->next();

            TermExpression(stream);

            current = stream->current().value;
        }
    }


    // TermExpression ::= FactorExpression { ( "*" | "/" | "%" ) FactorExpression }
    static void TermExpression(TokenStream* stream) throw (std::string)
    {
        FactorExpression(stream);
        std::string current = stream->current().value;
        while( current == "*" || current == "/" || current == "%" ) {
            stream->next();

            FactorExpression(stream);

            current = stream->current().value;
        }
    }


    // FactorExpression ::= "(" Expression ")" | ObjectExpression | Lterial
    static void FactorExpression(TokenStream* stream) throw (std::string)
    {
        std::string current = stream->current().value;
        if(current == "(") {
            stream->next();

            Expression(stream);

            if(stream->current().value != ")") {
                std::cout << stream->current() << std::endl;
                throw std::string("FactorExpression: lost `)");
            }

            stream->next();             // eat `)
        } else if(stream->current().type > Token::Lterial ||
                  stream->current().value == "{" ||
                  stream->current().value == "[" ) {
            Lterial(stream);
        } else {
            ObjectExpression(stream);
        }
    }


    // ObjectExpression ::= ObjectExpression "." Identity
    //                    | ObjectExpression "[" Expression "]"
    //                    | ObjectExpression "(" CallArgumentList ")"
    //                    | Identity
    static void ObjectExpression(TokenStream* stream) throw (std::string)
    {
        if(stream->current().type != Token::Identity) {
            std::cout << stream->current() << std::endl;
            throw std::string("ObjectExpression: not start of Identity " + stream->current().value);
        }

        Identity(stream);

        std::string current = stream->current().value;
        while(current == "."
              || current == "["
              || current == "("
              )
        {
            if(current == ".") {
                stream->next();

                Identity(stream);

            } else if(current == "[") {
                stream->next();

                Expression(stream);

                if(stream->current().value != "]") {
                    std::cout << stream->current() << std::endl;
                    throw std::string("ObjectExpression:  lost `]");
                }

                stream->next();             // eat `]

            } else if(current == "(") {
                stream->next();

                CallArgumentList(stream);

                if(stream->current().value != ")") {
                    std::cout << stream->current() << std::endl;
                    throw std::string("ObjectExpression: in CallArgumentList lost `)");
                }

                stream->next();             // eat `)
            }

            current = stream->current().value;
        }
    }


    // CallArgumentList ::= ε | Expression { "," Expression }
    static void CallArgumentList(TokenStream* stream) throw (std::string)
    {
        Token current = stream->current();
        if(current.value == ")") {
            // don't stream->next();
            return;
        } else if(current.value == "]" || current.value == "}" || current.type == Token::Operator) {
            // +,-,!
            // *,/,%,+=,*=,-=,/=,%=,>,<,>=,<=,==,!=
            // ],}

            std::cout << stream->current() << std::endl;
            throw std::string("CallArgumentList: have a error Token" + current.value);
        } else {
            Expression(stream);

            while(stream->current().value == ",") {
                stream->next();
                Expression(stream);
            }
        }
    }


    // Lterial ::= StringLterial | NumberLterial | ArrayLterial | KeyValuesLterial
    static void Lterial(TokenStream* stream) throw (std::string)
    {
        Token current = stream->current();
        if(current.type == Token::StringLterial) {
            StringLterial(stream);
        } else if(current.type == Token::NumberLterial) {
            NumberLterial(stream);
        } else if(current.value == "{"){
            KeyValuesLterial(stream);
        } else if(current.value == "[") {
            ArrayLterial(stream);
        } else {
            throw std::string("Lterial: Token not a `Lterial");
        }
    }


    // KeyValuesLterial ::= "{" { StringLterial ":" Expression "," } "}"           // map 或者说是对象，不能对此进行函数调用
    static void KeyValuesLterial(TokenStream* stream) throw (std::string)
    {
        stream->next();

        while(stream->current().value != "}") {

            if(stream->current().type != Token::StringLterial) {
                std::cout << stream->current() << std::endl;
                throw std::string("KeyValuesLterial Map Object Key Not StringLterial, current Token " + stream->current().value);
            }

            // `Key
            StringLterial(stream);

            if(stream->current().value != ":") {
                std::cout << stream->current() << std::endl;
                throw std::string("KeyValuesLterial Map Object Lost `:, current Token " + stream->current().value);
            }

            stream->next();

            // `Value
            Expression(stream);

            if(stream->current().value == ",") {
                stream->next();
            }
        }

        stream->next();
    }


    // ArrayLterial ::= "[" { Expression "," } "]"                                 // 数组，不能对此进行函数调用
    static void ArrayLterial(TokenStream* stream) throw (std::string)
    {
        // first `[
        stream->next();

        while(stream->current().value != "]") {
            Expression(stream);

            if(stream->current().value == ",") {
                stream->next();
            }
        }
        stream->next();
    }


    static void NumberLterial(TokenStream* stream) throw(std::string)
    {
        stream->next();
    }


    static void StringLterial(TokenStream* stream) throw(std::string)
    {
        stream->next();
    }


    static void Identity(TokenStream* stream) throw(std::string)
    {
        stream->next();
    }


    // IfStatement ::= "if" "(" Expression ")" Statement [ ElseStatement ]
    static void IfStatement(TokenStream* stream) throw (std::string)
    {
        // first is `if
        stream->next();

        if(stream->current().value != "(") {
            throw std::string("IfStatement lost `(");
        }

        stream->next();

        Expression(stream);

        if(stream->current().value != ")") {
            throw std::string("IfStatement lost `)");
        }

        stream->next();

        Statement(stream);

        std::string ahead = stream->current().value;

        if(ahead == "else") {
            ElseStatement(stream);
        }
    }


    // ElseStatement ::= "else" Statement
    static void ElseStatement(TokenStream* stream) throw (std::string)
    {
        // first is `else
        stream->next();

        Statement(stream);
    }


    // DoWhileStatement ::= "do" Statement "while" "(" Expression ")" ";"
    static void DoWhileStatement(TokenStream* stream) throw (std::string)
    {
        // first is `do
        stream->next();

        Statement(stream);

        // stream->next();

        if(stream->current().value != "while") {
            throw std::string("DoWhileStatement lost `while");
        }

        stream->next();

        if(stream->current().value != "(") {
            throw std::string("DoWhileStatement while lost `(");
        }

        stream->next();

        Expression(stream);

        if(stream->current().value != ")") {
            throw std::string("DoWhileStatement while lost `)");
        }

        stream->next();

        if(stream->current().value != ";") {
            throw std::string("DoWhileStatement while lost `;");
        }

        stream->next();
    }


    // WhileStatement ::= "while" "(" Expression ")" Statement
    static void WhileStatement(TokenStream* stream) throw (std::string)
    {
        // first is `while
        stream->next();

        if(stream->current().value != "(") {
            throw std::string("WhileStatement lost `(");
        }

        stream->next();

        Expression(stream);

        // stream->next();

        if(stream->current().value != ")") {
            throw std::string("WhileStatement lost `)");
        }

        stream->next();

        Statement(stream);
    }


    // ForStatement ::= "for" "(" Expression ";" Expression ";" Expression ")" Statement
    static void ForStatement(TokenStream* stream) throw (std::string)
    {
        // first is `for

        stream->next();

        if(stream->current().value != "(") {
            throw std::string("ForStatement lost `(");
        }

        stream->next();

        Expression(stream);

        if(stream->current().value != ";") {
            throw std::string("ForStatement lost frist `;");
        }

        stream->next();

        Expression(stream);

        if(stream->current().value != ";") {
            throw std::string("ForStatement lost second `;");
        }

        stream->next();

        Expression(stream);

        if(stream->current().value != ")") {
            throw std::string("ForStatement lost `)");
        }

        stream->next();

        Statement(stream);
    }


    // TryStatement ::= "try" Block CatchStatement
    static void TryStatement(TokenStream* stream) throw (std::string)
    {
        // first is `try
        stream->next();

        Block(stream);

        if(stream->current().value != "catch") {
            throw std::string("TryStatement lost `catch");
        }

        CatchStatement(stream);
    }


    // CatchStatement ::= "catch" Block
    static void CatchStatement(TokenStream* stream) throw (std::string)
    {
        // first is `catch
        stream->next();

        if(stream->current().value != "(") {
            throw std::string("CatchStatement lost )");
        }

        stream->next();

        Expression(stream);

        if(stream->current().value != ")") {
            throw std::string("CatchStatement lost `)");
        }

        stream->next();

        Block(stream);
    }


    // Block ::= "{" Statements "}"
    static void Block(TokenStream* stream) throw (std::string)
    {
        // first is `{
        stream->next();

        if(stream->current().value == "}") {
            stream->next();
            return;
        }

        while(!stream->atEnd()) {
            Statement(stream);
            if(stream->current().value == "}") {
                break;
            }
        }

        stream->next();             // eat `}
    }


    // Break ::= "break"
    static void Break(TokenStream* stream) throw (std::string)
    {
        stream->next();
    }


    // Continue ::= "continue"
    static void Continue(TokenStream* stream) throw (std::string)
    {
        stream->next();
    }


    // BreakStatement ::= Break ";"
    static void BreakStatement(TokenStream* stream) throw (std::string)
    {
        Break(stream);
        if(stream->current().value != ";") {
            std::cout << stream->current() << std::endl;
            throw std::string("BreakStatement lost `; current token " + stream->current().value);
        }
        stream->next();
    }


    // ContinueStatement ::= Continue ";"
    static void ContinueStatement(TokenStream* stream) throw (std::string)
    {
        Continue(stream);
        if(stream->current().value != ";") {
            std::cout << stream->current() << std::endl;
            throw std::string("ContinueStatement lost `; current token " + stream->current().value);
        }
        stream->next();
    }

};

#endif // PARSER_H
