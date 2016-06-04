#ifndef EXPRESSIONPARSER_H
#define EXPRESSIONPARSER_H

#include "tokenstream.hpp"

class ExpressionParser
{
public:
    // ExpressionStatement ::= Expression ";" | ";"
    static void ExpressionStatement(TokenStream* stream) throw(std::string)
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


    // Expression ::= ObjectExpression { ( "=" | "+=" | "-=" | "*=" | "%=" | "/=" ) BoolExpression }
    //              | BoolExpression
    static void Expression(TokenStream* stream) throw(std::string)
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
    static void BoolExpression(TokenStream* stream) throw(std::string)
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
    static void ArithmeticExpression(TokenStream* stream) throw(std::string)
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
    static void TermExpression(TokenStream* stream) throw(std::string)
    {
        FactorExpression(stream);
        std::string current = stream->current().value;
        while( current == "*" || current == "/" || current == "%" ) {
            stream->next();

            FactorExpression(stream);

            current = stream->current().value;
        }

    }


    // FactorExpression ::= "(" Expression ")"
    //                    | ObjectExpression
    //                    | Lterial
    static void FactorExpression(TokenStream* stream) throw(std::string)
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

    static void ObjectExpression(TokenStream* stream) throw(std::string)
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
    static void CallArgumentList(TokenStream* stream) throw(std::string)
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


    // Lterial ::= StringLterial | NumberLterial
    static void Lterial(TokenStream* stream) throw(std::string)
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

    // KeyValues ::= "{" { StringLterial ":" Expression "," } "}"         // map 或者说是对象，不能对此进行函数调用
    static void KeyValuesLterial(TokenStream* stream) throw(std::string)
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
            } else {
                if(stream->current().value != "}") {
                    std::cout << stream->current() << std::endl;
                    throw std::string("KeyValuesLterial Key-Value lost `, current token " + stream->current().value);
                }
            }
        }

        stream->next();
    }

    // LterialArray ::= "[" { Expression "," } "]"                           // 数组，不能对此进行函数调用
    static void ArrayLterial(TokenStream* stream) throw(std::string)
    {
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
};

#endif // EXPRESSIONPARSER_H
