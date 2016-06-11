#ifndef PARSER_H
#define PARSER_H

#include "tokenstream.hpp"
#include "calleetracker.hpp"

#include "throwable.hpp"

class Parser
{
public:

    // Program ::= { Function | Statements }
    static void Program(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        while(!stream->atEnd()) {

            if(stream->current().value == "function") {
                Function(stream);
            } else {
                Statements(stream);
            }
        }
    }


    // Statements ::= { Statement }
    static void Statements(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

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
    //             | ThrowStatement
    //             | Block
    //             | ReturnStatement
    //             | DeclarationStatement
    static void Statement(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        const int type = stream->current().type;
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
        } else if(ahead == "throw"){
            ThrowStatement(stream);
        } else if(ahead == "return") {
            ReturnStatement(stream);
        } else if(type == Token::TypeName) {
            DeclarationStatement(stream);
        }else{
            ExpressionStatement(stream);
        }
    }


    // ExpressionStatement ::= Expression ";" | ";"
    static void ExpressionStatement(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        if(stream->current().value == ";") {
            stream->next();
        } else {
            Expression(stream);

            if(stream->current().value != ";") {
                std::cout << stream->current() << std::endl;
                throw Throwable(0, "ExpressionStatement: lost `;");
            }
            stream->next();
        }
    }


    // DeclarationStatement ::= TypeName Identity [ "=" Expression ] { "," Identity [ "=" Expression ] } ";"
    static void DeclarationStatement(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        TypeName(stream);

        Identity(stream);

        if(stream->current().value == "=") {
            stream->next();
            Expression(stream);
        }

        while(stream->current().value == ",") {
            stream->next();

            TypeName(stream);

            Identity(stream);

            if(stream->current().value == "=") {
                stream->next();
                Expression(stream);
            }
        }

        if(stream->current().value != ";") {
            std::cout << stream->current() << std::endl;
            throw Throwable(0, "DeclarationStatement: lost `;");
        }
        stream->next();

    }

    // Expression ::= ObjectExpression { ( "=" | "+=" | "-=" | "*=" | "%=" | "/=" ) BoolExpression }
    //              | ObjectExpression { ( ">" | "<" | ">=" | "<=" | "==" | "!=" | "+" | "-" | "*" | "/" | "%" ) Expression }
    //              | BoolExpression
    static void Expression(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

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

            while( current == ">"  || current == "<"  ||
                   current == ">=" || current == "<=" ||
                   current == "==" || current == "!=" ||
                   current == ">"  || current == "<"  ||
                   current == ">=" || current == "<=" ||
                   current == "==" || current == "!=" ||
                   current == "+"  || current == "-"  ||
                   current == "*"  || current == "/"  || current == "%"
                   )
            {
                stream->next();

                Expression(stream);

                current = stream->current().value;
            }
        } else {
            BoolExpression(stream);
        }
    }


    // BoolExpression ::= ArithmeticExpression { ( ">" | "<" | ">=" | "<=" | "==" | "!=" ) ArithmeticExpression }
    static void BoolExpression(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

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
    static void ArithmeticExpression(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        TermExpression(stream);
        std::string current = stream->current().value;
        while( current == "+" || current == "-" ) {
            stream->next();

            TermExpression(stream);

            current = stream->current().value;
        }
    }


    // TermExpression ::= FactorExpression { ( "*" | "/" | "%" ) FactorExpression }
    static void TermExpression(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        FactorExpression(stream);
        std::string current = stream->current().value;
        while( current == "*" || current == "/" || current == "%" ) {
            stream->next();

            FactorExpression(stream);

            current = stream->current().value;
        }
    }


    // FactorExpression ::= "(" Expression ")" | ObjectExpression | Lterial
    static void FactorExpression(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        std::string current = stream->current().value;
        if(current == "(") {
            stream->next();

            Expression(stream);

            if(stream->current().value != ")") {
                std::cout << stream->current() << std::endl;
                throw Throwable(0, "FactorExpression: lost `)");
            }

            stream->next();             // eat `)
        } else if(stream->current().type > Token::Lterial ||
                  stream->current().value == "{" ||
                  stream->current().value == "[" ) {
            Lterial(stream);
        } else if(stream->current().value == "function") {
            Lambda(stream);
        }else  {
            ObjectExpression(stream);
        }
    }


    // ObjectExpression ::= ObjectExpression "." Identity
    //                    | ObjectExpression "[" Expression "]"
    //                    | ObjectExpression "(" CallArgumentList ")"
    //                    | Identity
    static void ObjectExpression(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        if(stream->current().type != Token::Identity) {
            std::cout << stream->current() << std::endl;
            throw Throwable(0, "ObjectExpression: not start of Identity " + stream->current().value);
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
                    throw Throwable(0, "ObjectExpression:  lost `]");
                }

                stream->next();             // eat `]

            } else if(current == "(") {
                stream->next();

                CallArgumentList(stream);

                if(stream->current().value != ")") {
                    std::cout << stream->current() << std::endl;
                    throw Throwable(0, "ObjectExpression: in CallArgumentList lost `)");
                }

                stream->next();             // eat `)
            }

            current = stream->current().value;
        }
    }


    // CallArgumentList ::= ε | Expression { "," Expression }
    static void CallArgumentList(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        Token current = stream->current();
        if(current.value == ")") {
            // don't stream->next();
            return;
        } else if(current.value == "]" || current.value == "}" || current.type == Token::Operator) {
            // +,-,!
            // *,/,%,+=,*=,-=,/=,%=,>,<,>=,<=,==,!=
            // ],}

            std::cout << stream->current() << std::endl;
            throw Throwable(0, "CallArgumentList: have a error Token" + current.value);
        } else {
            Expression(stream);

            while(stream->current().value == ",") {
                stream->next();
                Expression(stream);
            }
        }
    }


    // Lterial ::= StringLterial | NumberLterial | ArrayLterial | KeyValuesLterial
    static void Lterial(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

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
            throw Throwable(0, "Lterial: Token not a `Lterial");
        }
    }


    // KeyValuesLterial ::= "{" { StringLterial ":" Expression "," } "}"           // map 或者说是对象，不能对此进行函数调用
    static void KeyValuesLterial(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        stream->next();

        while(stream->current().value != "}") {

            if(stream->current().type != Token::StringLterial) {
                std::cout << stream->current() << std::endl;
                throw Throwable(0, "KeyValuesLterial Map Object Key Not StringLterial, current Token " + stream->current().value);
            }

            // `Key
            StringLterial(stream);

            if(stream->current().value != ":") {
                std::cout << stream->current() << std::endl;
                throw Throwable(0, "KeyValuesLterial Map Object Lost `:, current Token " + stream->current().value);
            }

            stream->next();

            // `Value
            Expression(stream);

            if(stream->current().value == ",") {
                stream->next();
            } else {
                if(stream->current().value != "}") {
                    std::cout << stream->current() << std::endl;
                    throw Throwable(0, "KeyValuesLterial Key-Value lost `, current token " + stream->current().value);
                }
            }
        }

        stream->next();
    }


    // ArrayLterial ::= "[" { Expression "," } "]"                                 // 数组，不能对此进行函数调用
    static void ArrayLterial(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

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


    static void NumberLterial(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        stream->next();
    }


    static void StringLterial(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        stream->next();
    }


    static void Identity(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        if(stream->current().type != Token::Identity) {
            std::cout << stream->current() << std::endl;
            throw Throwable(0,stream->current().value + " isn't' Identity");
        }

        stream->next();
    }


    // IfStatement ::= "if" "(" Expression ")" Statement [ ElseStatement ]
    static void IfStatement(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        // first is `if
        stream->next();

        if(stream->current().value != "(") {
            throw Throwable(0,"IfStatement lost `(");
        }

        stream->next();

        Expression(stream);

        if(stream->current().value != ")") {
            throw Throwable(0,"IfStatement lost `)");
        }

        stream->next();

        Statement(stream);

        std::string ahead = stream->current().value;

        if(ahead == "else") {
            ElseStatement(stream);
        }
    }


    // ElseStatement ::= "else" Statement
    static void ElseStatement(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        // first is `else
        stream->next();

        Statement(stream);
    }


    // DoWhileStatement ::= "do" Statement "while" "(" Expression ")" ";"
    static void DoWhileStatement(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        // first is `do
        stream->next();

        Statement(stream);

        // stream->next();

        if(stream->current().value != "while") {
            throw Throwable(0,"DoWhileStatement lost `while");
        }

        stream->next();

        if(stream->current().value != "(") {
            throw Throwable(0,"DoWhileStatement while lost `(");
        }

        stream->next();

        Expression(stream);

        if(stream->current().value != ")") {
            throw Throwable(0,"DoWhileStatement while lost `)");
        }

        stream->next();

        if(stream->current().value != ";") {
            throw Throwable(0,"DoWhileStatement while lost `;");
        }

        stream->next();
    }


    // WhileStatement ::= "while" "(" Expression ")" Statement
    static void WhileStatement(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        // first is `while
        stream->next();

        if(stream->current().value != "(") {
            throw Throwable(0,"WhileStatement lost `(");
        }

        stream->next();

        Expression(stream);

        // stream->next();

        if(stream->current().value != ")") {
            throw Throwable(0,"WhileStatement lost `)");
        }

        stream->next();

        Statement(stream);
    }


    // ForStatement ::= "for" "(" Expression ";" Expression ";" Expression ")" Statement
    static void ForStatement(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        // first is `for

        stream->next();

        if(stream->current().value != "(") {
            throw Throwable(0,"ForStatement lost `(");
        }

        stream->next();

        Expression(stream);

        if(stream->current().value != ";") {
            throw Throwable(0,"ForStatement lost frist `;");
        }

        stream->next();

        Expression(stream);

        if(stream->current().value != ";") {
            throw Throwable(0,"ForStatement lost second `;");
        }

        stream->next();

        Expression(stream);

        if(stream->current().value != ")") {
            throw Throwable(0,"ForStatement lost `)");
        }

        stream->next();

        Statement(stream);
    }


    // TryStatement ::= "try" Block CatchStatement
    static void TryStatement(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        // first is `try
        stream->next();

        Block(stream);

        if(stream->current().value != "catch") {
            throw Throwable(0,"TryStatement lost `catch");
        }

        CatchStatement(stream);
    }


    // CatchStatement ::= "catch" Block
    static void CatchStatement(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        // first is `catch
        stream->next();

        if(stream->current().value != "(") {
            throw Throwable(0,"CatchStatement lost `(");
        }

        stream->next();

        Expression(stream);

        if(stream->current().value != ")") {
            throw Throwable(0,"CatchStatement lost `)");
        }

        stream->next();

        Block(stream);
    }


    // ThrowStatement ::= "throw" Expression ";"
    static void ThrowStatement(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        // first is `throw
        stream->next();

        Expression(stream);

        if(stream->current().value != ";") {
            std::cout << stream->current() << std::endl;
            throw Throwable(0, "ThrowStatement: lost `;");
        }

        stream->next();
    }


    // Block ::= "{" Statements "}"
    static void Block(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

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
    static void Break(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        stream->next();
    }


    // Continue ::= "continue"
    static void Continue(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        stream->next();
    }


    // BreakStatement ::= Break ";"
    static void BreakStatement(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        Break(stream);
        if(stream->current().value != ";") {
            std::cout << stream->current() << std::endl;
            throw Throwable(0,"BreakStatement lost `; current token " + stream->current().value);
        }
        stream->next();
    }


    // ContinueStatement ::= Continue ";"
    static void ContinueStatement(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        Continue(stream);
        if(stream->current().value != ";") {
            std::cout << stream->current() << std::endl;
            throw Throwable(0,"ContinueStatement lost `; current token " + stream->current().value);
        }
        stream->next();
    }


    // Lambda ::= "function" "(" FunctionArgumentsList ")" "->" TypeName Block
    static void Lambda(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        // first is `function
        stream->next();

        // if current is Identity throw

        if(stream->current().type == Token::Identity) {
            throw Throwable(0,"Function Can't In Statement!");
        }

        if(stream->current().value != "(") {
            std::cout << stream->current() << std::endl;
            throw Throwable(0,"Lambda lost `(; current token " + stream->current().value);
        }

        stream->next();

        FunctionArgumentsList(stream);

        if(stream->current().value != ")") {
            std::cout << stream->current() << std::endl;
            throw Throwable(0,"Lambda lost `); current token " + stream->current().value);
        }

        stream->next();

        if(stream->current().value != "->") {
            std::cout << stream->current() << std::endl;
            throw Throwable(0,"Lambda lost `->; current token " + stream->current().value);
        }

        stream->next();

        TypeName(stream);

        Block(stream);
    }


    // TypeName ::= "bool" | "int" | "string" | "real" | "var"
    static void TypeName(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        std::string type = stream->current().value;
        if(type == "bool" || type == "int" || type == "string" || type == "real" || type == "var") {
            stream->next();
        } else {
            std::cout << stream->current() << std::endl;
            throw Throwable(0,stream->current().value + " Not a Type!");
        }
    }


    // Function ::= "function" Identity "(" FunctionArgumentsList ")" "->" TypeName Body
    static void Function(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        // first is `function
        stream->next();

        Identity(stream);

        if(stream->current().value != "(") {
            std::cout << stream->current() << std::endl;
            throw Throwable(0,"Lambda lost `(; current token " + stream->current().value);
        }

        stream->next();

        FunctionArgumentsList(stream);

        if(stream->current().value != ")") {
            std::cout << stream->current() << std::endl;
            throw Throwable(0,"Lambda lost `); current token " + stream->current().value);
        }

        stream->next();

        if(stream->current().value != "->") {
            std::cout << stream->current() << std::endl;
            throw Throwable(0,"Lambda lost `->; current token " + stream->current().value);
        }

        stream->next();

        TypeName(stream);

        Block(stream);
    }


    // FunctionArgumentsList ::= ε | TypeName Identity { "," TypeName Identity }
    static void FunctionArgumentsList(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        if(stream->current().value == ")") {
            return ;
        }

        TypeName(stream);

        Identity(stream);

        while(stream->current().value == ",") {
            stream->next();

            TypeName(stream);

            Identity(stream);
        }
    }


    // Return ::= "return"
    static void Return(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        // first is `return
        stream->next();
    }


    // ReturnStatement ::= Return ExpressionStatement
    static void ReturnStatement(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        Return(stream);

        ExpressionStatement(stream);
    }

};

#endif // PARSER_H
