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
            // 为了处理临时变量
            Static<ParserContext>::Object.enterExpressionStatement();

            ExpressionStatement(stream);

            Static<ParserContext>::Object.exitExpressionStatement();
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

                std::string dst = Static<ParserContext>::Object.popToken().value;

                BoolExpression(stream);

                std::string arg0 = Static<ParserContext>::Object.popToken().value;
                // assign arg0, dst, temp_arg1
                std::string temp_arg1 = "$" + std::to_string( Static<ParserContext>::Object.getTemporaryId() );
                std::cout << "Assign" << current << ", " << arg0 << ", " <<  dst << " ," << temp_arg1 << std::endl;
                Static<ParserContext>::Object.pushToken(Token(Token::TemporaryId, temp_arg1 ));

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

                std::string dst = Static<ParserContext>::Object.popToken().value;

                // TODO change to BoolExpression
                BoolExpression(stream);

                std::string arg0 = Static<ParserContext>::Object.popToken().value;
                // op arg0, dst, temp_arg1
                std::string temp_arg1 = "$" + std::to_string( Static<ParserContext>::Object.getTemporaryId() );
                std::cout << current << ", " << arg0 << ", " <<  dst << " ," << temp_arg1 << std::endl;
                Static<ParserContext>::Object.pushToken(Token(Token::TemporaryId, temp_arg1 ));

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

            std::string dst = Static<ParserContext>::Object.popToken().value;

            ArithmeticExpression(stream);

            std::string arg0 = Static<ParserContext>::Object.popToken().value;
            // op arg0, dst, temp_arg1
            std::string temp_arg1 = "$" + std::to_string( Static<ParserContext>::Object.getTemporaryId() );
            std::cout << current << ", " << arg0 << ", " <<  dst << " ," << temp_arg1 << std::endl;
            Static<ParserContext>::Object.pushToken(Token(Token::TemporaryId, temp_arg1 ));

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

            std::string dst = Static<ParserContext>::Object.popToken().value;

            TermExpression(stream);

            std::string arg0 = Static<ParserContext>::Object.popToken().value;
            // op arg0, dst, temp_arg1
            std::string temp_arg1 = "$" + std::to_string( Static<ParserContext>::Object.getTemporaryId() );
            std::cout << current << ", " << arg0 << ", " <<  dst << " ," << temp_arg1 << std::endl;
            Static<ParserContext>::Object.pushToken(Token(Token::TemporaryId, temp_arg1 ));

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

            std::string dst = Static<ParserContext>::Object.popToken().value;

            FactorExpression(stream);

            std::string arg0 = Static<ParserContext>::Object.popToken().value;
            // op arg0, dst, temp_arg1
            std::string temp_arg1 = "$" + std::to_string( Static<ParserContext>::Object.getTemporaryId() );
            std::cout << current << ", " << arg0 << ", " <<  dst << " ," << temp_arg1 << std::endl;
            Static<ParserContext>::Object.pushToken(Token(Token::TemporaryId, temp_arg1 ));

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

        std::string op = "Unary" + stream->current().value;

        stream->next(); // eat `+ or `- or `!

        BoolExpression(stream);

        std::string dst = "$" + std::to_string(Static<ParserContext>::Object.getTemporaryId());
        std::cout << op << ", " << Static<ParserContext>::Object.popToken().value << ", " << dst << std::endl;
        Static<ParserContext>::Object.pushToken(Token(Token::TemporaryId, dst));
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

                std::string arg1 = Static<ParserContext>::Object.popToken().value;
                std::string arg0 = Static<ParserContext>::Object.popToken().value;
                std::string dst  = "$" + std::to_string(Static<ParserContext>::Object.getTemporaryId());
                std::cout << "Access" << ", " << arg0 << ", " << arg1 << ", " << dst << std::endl;

                Static<ParserContext>::Object.pushToken(Token(Token::TemporaryId, dst));


            } else if(current == "[") {
                stream->next();

                Expression(stream);

                if(stream->current().value != "]") {
                    std::cout << stream->current() << std::endl;
                    throw Throwable(0, "ObjectExpression:  lost `]");
                }

                stream->next();             // eat `]

                std::string arg1 = Static<ParserContext>::Object.popToken().value;
                std::string arg0 = Static<ParserContext>::Object.popToken().value;
                std::string dst  = "$" + std::to_string(Static<ParserContext>::Object.getTemporaryId());
                std::cout << "Access" << ", " << arg0 << ", " << arg1 << ", " << dst << std::endl;

                Static<ParserContext>::Object.pushToken(Token(Token::TemporaryId, dst));

            } else if(current == "(") {
                stream->next();

                std::string call_name =  Static<ParserContext>::Object.popToken().value ;

                CallArgumentList(stream);

                if(stream->current().value != ")") {
                    std::cout << stream->current() << std::endl;
                    throw Throwable(0, "ObjectExpression: in CallArgumentList lost `)");
                }

                stream->next();             // eat `)

                std::cout << "Call, " << call_name << std::endl;

                // std::string dst  = "$" + std::to_string(Static<ParserContext>::Object.getTemporaryId());
                Static<ParserContext>::Object.pushToken(Token(Token::TemporaryId, "$CallResult"));

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

            std::cout << "Param, " << Static<ParserContext>::Object.popToken().value << std::endl;

            while(stream->current().value == ",") {
                stream->next();
                Expression(stream);

                std::cout << "Param, " << Static<ParserContext>::Object.popToken().value << std::endl;

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

        std::string this_key_values_literal = "$" + std::to_string( Static<ParserContext>::Object.getTemporaryId() );

        while(stream->current().value != "}") {

            if(stream->current().type != Token::StringLiteral) {
                std::cout << stream->current() << std::endl;
                throw Throwable(0, "KeyValuesLiteral Map Object Key Not StringLiteral, current Token " + stream->current().value);
            }

            // `Key
            StringLiteral(stream);

            std::string key = Static<ParserContext>::Object.popToken().value;

            if(stream->current().value != ":") {
                std::cout << stream->current() << std::endl;
                throw Throwable(0, "KeyValuesLiteral Map Object Lost `:, current Token " + stream->current().value);
            }

            stream->next();

            // `Value
            Expression(stream);

            std::string value = Static<ParserContext>::Object.popToken().value;


            if(stream->current().value == ",") {
                stream->next();
            } else {
                if(stream->current().value != "}") {
                    std::cout << stream->current() << std::endl;
                    throw Throwable(0, "KeyValuesLiteral Key-Value lost `, current token " + stream->current().value);
                }
            }


            // append_pair key, value, this_key_values
            std::cout << "append_pair " << key << ", " << value << ", " << this_key_values_literal << std::endl;
        }

        stream->next();

        Static<ParserContext>::Object.pushToken(Token(Token::KeyValuesLiteral, this_key_values_literal));

    }


    // ArrayLiteral ::= "[" { Expression "," } "]"                                 // 数组，不能对此进行函数调用
    static void ArrayLiteral(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        // first `[
        stream->next();

        std::string this_array_literal = "$" + std::to_string( Static<ParserContext>::Object.getTemporaryId() );

        while(stream->current().value != "]") {
            Expression(stream);

            std::string value = Static<ParserContext>::Object.popToken().value;

            if(stream->current().value == ",") {
                stream->next();
            }

            // append_pair value, , this_key_values
            std::cout << "append_array_item " << value << ", " << ", " << this_array_literal << std::endl;
        }
        stream->next();

        Static<ParserContext>::Object.pushToken(Token(Token::ArrayLiteral, this_array_literal));
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

        Static<ParserContext>::Object.pushToken(stream->current());

        stream->next();
    }


    static void DoubleLiteral(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        Static<ParserContext>::Object.pushToken(stream->current());

        stream->next();
    }

    static void StringLiteral(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        Static<ParserContext>::Object.pushToken(stream->current());

        stream->next();
    }


    static void Identity(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        // TODO 检查声明情况与所在代码块
        if(stream->current().type != Token::Identity) {
            std::cout << stream->current() << std::endl;
            throw Throwable(0,stream->current().value + " isn't' Identity");
        }

        Static<ParserContext>::Object.pushToken(stream->current());

        stream->next();
    }


    // IfStatement ::= "if" "(" Expression ")" Statement [ ElseStatement ]
    static void IfStatement(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        int label0, label1;

        label0 = Static<ParserContext>::Object.getNewLabel();
        label1 = Static<ParserContext>::Object.getNewLabel();

        // std::cout << "IfStatement: " << label0 << ", " << label1 << std::endl;

        Static<ParserContext>::Object.pushLabel(label0);
        Static<ParserContext>::Object.pushLabel(label1);

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

        std::cout << "Jump_If_False " << label1 << std::endl;

        std::cout << "lable " << label0 << " :"  << std::endl;

        Statement(stream);

        std::string ahead = stream->current().value;

        std::cout << "lable " << label1 << " :" << std::endl;

        Static<ParserContext>::Object.popLable();
        Static<ParserContext>::Object.popLable();

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

        int label0, label1, label2;

        label0 = Static<ParserContext>::Object.getNewLabel();
        label1 = Static<ParserContext>::Object.getNewLabel();
        label2 = Static<ParserContext>::Object.getNewLabel();

        // std::cout << "DoWhileStatement: " << label0 << ", " << label1 << ", " << label2 << std::endl;

        Static<ParserContext>::Object.pushLoopStartLabel(label0);
        Static<ParserContext>::Object.pushLabel(label1);
        Static<ParserContext>::Object.pushLoopEndLabel(label2);

        // first is `do
        stream->next();

        std::cout << "label " << label0  << " :" << std::endl;

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

        std::cout << "lable " << label1  << " :" << std::endl;

        Expression(stream);

        std::cout << "goto " << "E ? " << label0 << " : " << label2 << std::endl;

        if(stream->current().value != ")") {
            throw Throwable(0,"DoWhileStatement while lost `)");
        }

        stream->next();

        if(stream->current().value != ";") {
            throw Throwable(0,"DoWhileStatement while lost `;");
        }

        stream->next();

        std::cout << "lable " << label2  << " :" << std::endl;

        Static<ParserContext>::Object.popLoopEndLabel();
        Static<ParserContext>::Object.popLable();
        Static<ParserContext>::Object.popLoopStartLabel();

        Static<ParserContext>::Object.exitLoopStatement();
    }


    // WhileStatement ::= "while" "(" Expression ")" Statement
    static void WhileStatement(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        Static<ParserContext>::Object.enterLoopStatement();

        int label0, label1, label2;

        label0 = Static<ParserContext>::Object.getNewLabel();
        label1 = Static<ParserContext>::Object.getNewLabel();
        label2 = Static<ParserContext>::Object.getNewLabel();

        // std::cout << "WhileStatement: " << label0 << ", " << label1 << ", " << label2 << std::endl;

        Static<ParserContext>::Object.pushLoopStartLabel(label0);
        Static<ParserContext>::Object.pushLabel(label1);
        Static<ParserContext>::Object.pushLoopEndLabel(label2);

        // first is `while
        stream->next();

        if(stream->current().value != "(") {
            throw Throwable(0,"WhileStatement lost `(");
        }

        stream->next();

        std::cout << "label " << label0  << " :" << std::endl;

        Expression(stream);

        // std::cout << "goto " << "E ? " << label1 << " : " << label2 << std::endl;

        std::cout << "Jump_If_False " << label2 << std::endl;

        if(stream->current().value != ")") {
            throw Throwable(0,"WhileStatement lost `)");
        }

        stream->next();

        std::cout << "label " << label1  << " :" << std::endl;

        Statement(stream);

        std::cout << "label " << label2  << " :" << std::endl;

        std::cout << "Jump " << label0 << std::endl;

        Static<ParserContext>::Object.popLoopEndLabel();
        Static<ParserContext>::Object.popLable();
        Static<ParserContext>::Object.popLoopStartLabel();

        Static<ParserContext>::Object.exitLoopStatement();
    }


    // ForStatement ::= "for" "(" Expression ";" Expression ";" Expression ")" Statement
    static void ForStatement(TokenStream* stream) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        Static<ParserContext>::Object.enterLoopStatement();

        int label0, label1, label2, label3;

        label0 = Static<ParserContext>::Object.getNewLabel();
        label1 = Static<ParserContext>::Object.getNewLabel();
        label2 = Static<ParserContext>::Object.getNewLabel();
        label3 = Static<ParserContext>::Object.getNewLabel();

        // std::cout << "ForStatement: " << label0 << ", " << label1 << ", " << label2 << ", " << label3 << std::endl;

        Static<ParserContext>::Object.pushLoopStartLabel(label0);
        Static<ParserContext>::Object.pushLabel(label1);
        Static<ParserContext>::Object.pushLabel(label2);
        Static<ParserContext>::Object.pushLoopEndLabel(label3);

        // first is `for

        stream->next();

        if(stream->current().value != "(") {
            throw Throwable(0,"ForStatement lost `(");
        }

        stream->next();

        Expression(stream);                         // E0

        if(stream->current().value != ";") {
            throw Throwable(0,"ForStatement lost frist `;");
        }

        stream->next();

        std::cout << "label " << label0  << " :" << std::endl;

        Expression(stream);                         // E1

        if(stream->current().value != ";") {
            throw Throwable(0,"ForStatement lost second `;");
        }

        std::cout << "goto " << "E ? " << label2 << " : " << label3 << std::endl;

        stream->next();

        std::cout << "label " << label1  << " :" << std::endl;

        Expression(stream);                         // E2

        std::cout << "Jump " << label0 << std::endl;

        if(stream->current().value != ")") {
            throw Throwable(0,"ForStatement lost `)");
        }

        stream->next();

        std::cout << "label " << label2  << " :" << std::endl;

        Statement(stream);

        std::cout << "Jump " << label1 << std::endl;


        std::cout << "label " << label3  << " :" << std::endl;

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

        std::cout << "Jump " << Static<ParserContext>::Object.getLoopEndLabel() << std::endl;

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

        std::cout << "Jump " << Static<ParserContext>::Object.getLoopStartLabel() << std::endl;

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

        // TODO 记录函数特征
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
            throw Throwable(0,"Function lost `(; current token " + stream->current().value);
        }

        stream->next();

        FunctionArgumentsList(stream);

        if(stream->current().value != ")") {
            std::cout << stream->current() << std::endl;
            throw Throwable(0,"Function lost `); current token " + stream->current().value);
        }

        stream->next();

        if(stream->current().value != "->") {
            std::cout << stream->current() << std::endl;
            throw Throwable(0,"Function lost `->; current token " + stream->current().value);
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
