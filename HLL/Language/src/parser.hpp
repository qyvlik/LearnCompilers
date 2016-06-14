#ifndef PARSER_H
#define PARSER_H

#include "tokenstream.hpp"
#include "calleetracker.hpp"
#include "throwable.hpp"
#include "parsercontext.hpp"

template<typename T>
class Static
{
public:
    static T Object;
};

template<typename T> T Static<T>::Object;

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
            // TODO if current statement is function body
            ReturnStatement(stream);
        } else if(type == Token::TypeName) {
            // delcara every where
            DeclarationStatement(stream);
        }else{
            ExpressionStatement(stream);
        }
    }


    // ExpressionStatement ::= Expression ";" | ";"
    static void ExpressionStatement(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        // TODO if current statement is return

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

                // TODO change to BoolExpression
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


    // FactorExpression ::= "(" Expression ")" | ObjectExpression | Literal | Lambda | UnaryExpression
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
        } else if(stream->current().type > Token::Literal ||
                  current == "{" ||
                  current == "[" ) {
            Literal(stream);
        } else if(current == "function") {
            Lambda(stream);
        } else if(current == "+" || current == "-" || current == "!") {
            UnaryExpression(stream);
        } else {
            ObjectExpression(stream);
        }
    }

    // UnaryExpression ::= { ( "+" | "-" | "!" ) } BoolExpression
    static void UnaryExpression(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        stream->next(); // eat `+ or `- or `!

        BoolExpression(stream);
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


    // Literal ::= StringLiteral | NumberLiteral | ArrayLiteral | KeyValuesLiteral
    static void Literal(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        Token current = stream->current();
        if(current.type == Token::StringLiteral) {
            StringLiteral(stream);
        } else if(current.type == Token::NumberLiteral || current.type == Token::IntegerLiteral || current.type == Token::DoubleLiteral ) {
            NumberLiteral(stream);
        } else if(current.value == "{"){
            KeyValuesLiteral(stream);
        } else if(current.value == "[") {
            ArrayLiteral(stream);
        } else {
            throw Throwable(0, "Literal: Token not a `Literal");
        }
    }


    // KeyValuesLiteral ::= "{" { StringLiteral ":" Expression "," } "}"           // map 或者说是对象，不能对此进行函数调用
    static void KeyValuesLiteral(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        stream->next();

        while(stream->current().value != "}") {

            if(stream->current().type != Token::StringLiteral) {
                std::cout << stream->current() << std::endl;
                throw Throwable(0, "KeyValuesLiteral Map Object Key Not StringLiteral, current Token " + stream->current().value);
            }

            // `Key
            StringLiteral(stream);

            if(stream->current().value != ":") {
                std::cout << stream->current() << std::endl;
                throw Throwable(0, "KeyValuesLiteral Map Object Lost `:, current Token " + stream->current().value);
            }

            stream->next();

            // `Value
            Expression(stream);

            if(stream->current().value == ",") {
                stream->next();
            } else {
                if(stream->current().value != "}") {
                    std::cout << stream->current() << std::endl;
                    throw Throwable(0, "KeyValuesLiteral Key-Value lost `, current token " + stream->current().value);
                }
            }
        }

        stream->next();
    }


    // ArrayLiteral ::= "[" { Expression "," } "]"                                 // 数组，不能对此进行函数调用
    static void ArrayLiteral(TokenStream* stream) throw(Throwable)
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


    // NumberLiteral ::= IntegerLiteral | DoubleLiteral
    static void NumberLiteral(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        int type = stream->current().type;
        if(type == Token::IntegerLiteral) {
            IntegerLiteral(stream);
        } else if(type == Token::DoubleLiteral) {
            DoubleLiteral(stream);
        } else {
            std::cout << stream->current() << std::endl;
            throw Throwable(0, "Current Token is Not a NumberLiteral");
        }
    }


    static void IntegerLiteral(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        stream->next();
    }


    static void DoubleLiteral(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        stream->next();
    }

    static void StringLiteral(TokenStream* stream) throw(Throwable)
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

        Static<ParserContext>::Object.enterLoopStatement();

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

        Static<ParserContext>::Object.exitLoopStatement();
    }


    // WhileStatement ::= "while" "(" Expression ")" Statement
    static void WhileStatement(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        Static<ParserContext>::Object.enterLoopStatement();

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

        Static<ParserContext>::Object.exitLoopStatement();
    }


    // ForStatement ::= "for" "(" Expression ";" Expression ";" Expression ")" Statement
    static void ForStatement(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        Static<ParserContext>::Object.enterLoopStatement();

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

        Static<ParserContext>::Object.exitLoopStatement();
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

        Static<ParserContext>::Object.enterBlock();

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
                // meaning KeyValuesLiteral can't declara alone with out Statement
            } else if(stream->current().value == ":") {
                std::cout << stream->current() << std::endl;
                throw Throwable(0, "KeyValuesLiteral: Can't Declara Alone With Out Statement !");
            }
        }

        stream->next();             // eat `}

        Static<ParserContext>::Object.exitBlock();
    }


    // BreakStatement ::= "break" ";"
    static void BreakStatement(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        if(!Static<ParserContext>::Object.inLoopStatement()) {
            std::cerr << "Token `continue Only In Loop Statement" << std::endl;
        }

        stream->next(); // `break

        if(stream->current().value != ";") {
            std::cout << stream->current() << std::endl;
            throw Throwable(0,"BreakStatement lost `; current token " + stream->current().value);
        }
        stream->next();
    }


    // ContinueStatement ::= "continue" ";"
    static void ContinueStatement(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        if(!Static<ParserContext>::Object.inLoopStatement()) {
            std::cerr << "Token `continue Only In Loop Statement" << std::endl;
        }

        stream->next(); // `continue

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

        Static<ParserContext>::Object.enterFunctionBody();

        Block(stream);

        Static<ParserContext>::Object.exitFunctionBody();
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

        Static<ParserContext>::Object.enterFunctionBody();

        Block(stream);

        Static<ParserContext>::Object.exitFunctionBody();
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


    // ReturnStatement ::= "return" ExpressionStatement
    static void ReturnStatement(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        stream->next();  // `return

        ExpressionStatement(stream);
    }

};

#endif // PARSER_H
