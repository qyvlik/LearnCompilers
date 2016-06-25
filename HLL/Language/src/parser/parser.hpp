#ifndef PARSER_H
#define PARSER_H

#include "tokenstream.hpp"
#include "parsercontext.hpp"

namespace qyvlik {

class Parser
{
public:

    // Program ::= { Function | Statements }
    static void Program(TokenStream* stream, ParserContext* context) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        context->enterBlock();

        std::cout << "Start Program" << std::endl;

        while(!stream->atEnd()) {

            if(stream->current().value == "function") {
                Function(stream, context);
            } else {
                Statements(stream, context);
            }
        }

        std::cout << "End Program" << std::endl;

        context->exitBlock();
    }


    // Statements ::= { Statement }
    static void Statements(TokenStream* stream, ParserContext* context) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        while(!stream->atEnd()) {
            Statement(stream, context);
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
    static void Statement(TokenStream* stream, ParserContext* context) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        const int type = stream->current().type;
        std::string ahead = stream->current().value;

        if(ahead == "if") {
            IfStatement(stream, context);
        } else if(ahead == "do") {
            DoWhileStatement(stream, context);
        } else if(ahead == "while") {
            WhileStatement(stream, context);
        } else if(ahead == "for") {
            ForStatement(stream, context);
        } else if(ahead == "continue") {
            ContinueStatement(stream, context);
        } else if(ahead == "break") {
            BreakStatement(stream, context);
        } else if(ahead == "{") {
            Block(stream, context);
        }  else if(ahead == "try") {
            TryStatement(stream, context);
        } else if(ahead == "throw"){
            ThrowStatement(stream, context);
        } else if(ahead == "return") {
            // TODO if current statement is function body
            ReturnStatement(stream, context);
        } else if(type == Token::TypeName) {
            // delcara every where
            DeclarationStatement(stream, context);
        }else{
            // 为了处理临时变量
            context->enterExpressionStatement();

            ExpressionStatement(stream, context);

            context->exitExpressionStatement();

            std::cout << "\tClearEvaluationStack" << std::endl;
        }
    }


    // ExpressionStatement ::= Expression ";" | ";"
    static void ExpressionStatement(TokenStream* stream, ParserContext* context) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        // TODO if current statement is return

        if(stream->current().value == ";") {
            stream->next();
        } else {
            Expression(stream, context);

            if(stream->current().value != ";") {
                std::cout << stream->current() << std::endl;
                throw Throwable(0, "ExpressionStatement: lost `;");
            }
            stream->next();
        }
    }


    // DeclarationStatement ::= TypeName Identity [ "=" Expression ] { "," Identity [ "=" Expression ] } ";"
    static void DeclarationStatement(TokenStream* stream, ParserContext* context) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        context->enterDeclarationStatement();

        std::cout << "\tStartDeclara " << context->currentDeclarationStatementId() << std::endl;

        TypeName(stream, context);

        do {
            if(stream->current().value == ",") {
                stream->next();
            }

            std::string id;
            int block_id = context->currentBlockId();

            Identity(stream, context);

            id = context->popToken().value;

            if(!context->insertSymbol(block_id, id)) {
                std::cerr << id + " Already Exists !" << std::endl;
            }

            if(stream->current().value == "=") {
                stream->next();
                Expression(stream, context);

                std::string value =  context->popToken().value;
                if(value.size() !=0 && value.at(0) != '$') {
                    std::cout << "\tPush " << value << std::endl;
                }

                std::cout << "\tDeclara " << id << std::endl;
                std::cout << "\t=" << std::endl;
                std::cout << "\tClearEvaluationStack" << std::endl;
            } else {
                std::cout << "\tDeclara " << id << std::endl;
                std::cout << "\tClearEvaluationStack" << std::endl;
            }

        } while( stream->current().value == "," );

        if(stream->current().value != ";") {
            std::cout << stream->current() << std::endl;
            throw Throwable(0, "DeclarationStatement: lost `;");
        }

        stream->next();

        std::cout << "\tEndDeclara " << context->currentDeclarationStatementId() << std::endl;

        context->exitDeclarationStatement();
    }


    // Expression ::= ObjectExpression { ( "=" | "+=" | "-=" | "*=" | "%=" | "/=" ) BoolExpression }
    //              | ObjectExpression { ( ">" | "<" | ">=" | "<=" | "==" | "!=" | "+" | "-" | "*" | "/" | "%" ) Expression }
    //              | BoolExpression
    static void Expression(TokenStream* stream, ParserContext* context) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        if(stream->current().type == Token::Identity) {
            ObjectExpression(stream, context);

            std::string current = stream->current().value;

            while( current == "="  || current == "+="  ||
                   current == "-=" || current == "*=" ||
                   current == "/=" || current == "%="
                   )
            {
                stream->next();

                std::string dst = context->popToken().value;

                BoolExpression(stream, context);

                std::string arg0 = context->popToken().value;
                // assign arg0, dst, temp_arg1
                std::string temp_arg1 = "$" + std::to_string( context->getTemporaryId() );

                if(arg0.size() != 0 && arg0.at(0) != '$') {
                    std::cout << "\tPush " << arg0 << std::endl;
                }

                std::cout << "\tPush " << dst << std::endl;

                std::cout << "\t" << current << std::endl;
                std::cout << "\t\t" << "<---- top:" << temp_arg1 << std::endl;

                context->pushToken(Token(Token::TemporaryId, temp_arg1 ));

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

                std::string arg0 = context->popToken().value;

                // TODO change to BoolExpression
                BoolExpression(stream, context);

                std::string arg1 = context->popToken().value;
                // op arg0, dst, temp_arg1
                std::string temp_arg1 = "$" + std::to_string( context->getTemporaryId() );

                std::cout << "\tPush " << arg0 << std::endl;
                std::cout << "\tPush " << arg1 << std::endl;

                std::cout << "\t" << current << std::endl;
                std::cout << "\t\t" << "<---- top:" << temp_arg1 << std::endl;

                context->pushToken(Token(Token::TemporaryId, temp_arg1 ));

                current = stream->current().value;
            }
        } else {
            BoolExpression(stream, context);
        }
    }


    // BoolExpression ::= ArithmeticExpression { ( ">" | "<" | ">=" | "<=" | "==" | "!=" ) ArithmeticExpression }
    static void BoolExpression(TokenStream* stream, ParserContext* context) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        ArithmeticExpression(stream, context);

        std::string current = stream->current().value;
        while( current == ">"  || current == "<"  ||
               current == ">=" || current == "<=" ||
               current == "==" || current == "!="
               )
        {
            stream->next();

            std::string dst = context->popToken().value;

            ArithmeticExpression(stream, context);

            std::string arg0 = context->popToken().value;
            // op arg0, dst, temp_arg1
            std::string temp_arg1 = "$" + std::to_string( context->getTemporaryId() );

            if(dst.size() != 0 && dst.at(0) != '$') {
                std::cout << "\tPush " << dst << std::endl;
            }

            if(arg0.size() != 0 && arg0.at(0) != '$') {
                std::cout << "\tPush " << arg0 << std::endl;
            }

            std::cout << "\t" << current << std::endl;
            std::cout << "\t\t" << "<---- top:" << temp_arg1 << std::endl;

            context->pushToken(Token(Token::TemporaryId, temp_arg1 ));

            current = stream->current().value;
        }
    }


    // ArithmeticExpression ::= TermExpression { ( "+" | "-" ) TermExpression }
    static void ArithmeticExpression(TokenStream* stream, ParserContext* context) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        TermExpression(stream, context);
        std::string current = stream->current().value;
        while( current == "+" || current == "-" ) {
            stream->next();

            std::string dst = context->popToken().value;

            TermExpression(stream, context);

            std::string arg0 = context->popToken().value;
            // op arg0, dst, temp_arg1
            std::string temp_arg1 = "$" + std::to_string( context->getTemporaryId() );

            if(dst.size() != 0 && dst.at(0) != '$') {
                std::cout << "\tPush " << dst << std::endl;
            }

            if(arg0.size() != 0 && arg0.at(0) != '$') {
                std::cout << "\tPush " << arg0 << std::endl;
            }

            std::cout << "\t" << current << std::endl;
            std::cout << "\t\t" << "<---- top:" << temp_arg1 << std::endl;

            context->pushToken(Token(Token::TemporaryId, temp_arg1 ));

            current = stream->current().value;
        }
    }


    // TermExpression ::= FactorExpression { ( "*" | "/" | "%" ) FactorExpression }
    static void TermExpression(TokenStream* stream, ParserContext* context) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        FactorExpression(stream, context);

        // std::cout << "Push " << context->topToken().value << std::endl;

        std::string current = stream->current().value;
        while( current == "*" || current == "/" || current == "%" ) {
            stream->next();

            std::string dst = context->popToken().value;

            FactorExpression(stream, context);

            std::string arg0 = context->popToken().value;
            // op arg0, dst, temp_arg1
            std::string temp_arg1 = "$" + std::to_string( context->getTemporaryId() );

            if(dst.size() != 0 && dst.at(0) != '$') {
                std::cout << "\tPush " << dst << std::endl;
            }

            if(arg0.size() != 0 && arg0.at(0) != '$') {
                std::cout << "\tPush " << arg0 << std::endl;
            }

            std::cout << "\t" << current << std::endl;
            std::cout << "\t\t" << "<---- top:" << temp_arg1 << std::endl;

            context->pushToken(Token(Token::TemporaryId, temp_arg1 ));

            current = stream->current().value;
        }
    }


    // FactorExpression ::= "(" Expression ")" | ObjectExpression | Literal | Lambda | UnaryExpression
    static void FactorExpression(TokenStream* stream, ParserContext* context) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        std::string current = stream->current().value;
        if(current == "(") {
            stream->next();

            Expression(stream, context);

            if(stream->current().value != ")") {
                std::cout << stream->current() << std::endl;
                throw Throwable(0, "FactorExpression: lost `)");
            }

            stream->next();             // eat `)
        } else if(stream->current().type > Token::Literal ||
                  current == "{" ||
                  current == "[" ) {
            Literal(stream, context);
        } else if(current == "function") {
            Lambda(stream, context);
        } else if(current == "+" || current == "-" || current == "!") {
            UnaryExpression(stream, context);
        } else {
            ObjectExpression(stream, context);
        }
    }

    // UnaryExpression ::= { ( "+" | "-" | "!" ) } BoolExpression
    static void UnaryExpression(TokenStream* stream, ParserContext* context) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        std::string op = "\tUnary" + stream->current().value;

        stream->next(); // eat `+ or `- or `!

        BoolExpression(stream, context);

        std::string temp_arg1 = "$" + std::to_string(context->getTemporaryId());

        std::cout << "\tPush " << context->popToken().value << std::endl;

        std::cout << "\tUnary: " << op << std::endl;
        std::cout << "\t\t" << "<---- top:" << temp_arg1 << std::endl;

        context->pushToken(Token(Token::TemporaryId, temp_arg1));
    }


    // ObjectExpression ::= ObjectExpression "." Identity
    //                    | ObjectExpression "[" Expression "]"
    //                    | ObjectExpression "(" CallArgumentList ")"
    //                    | Identity
    static void ObjectExpression(TokenStream* stream, ParserContext* context) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        if(stream->current().type != Token::Identity) {
            std::cout << stream->current() << std::endl;
            throw Throwable(0, "ObjectExpression: not start of Identity " + stream->current().value);
        }

        Identity(stream, context);

        std::string current = stream->current().value;
        while(current == "."
              || current == "["
              || current == "("
              )
        {
            std::string id_or_method_name = context->popToken().value ;

            if(id_or_method_name.size() != 0 && id_or_method_name.at(0) != '$') {
                std::cout << "\tPush " << id_or_method_name << std::endl;
            }


            if(current == ".") {

                stream->next();

                Identity(stream, context);

                std::string arg1 = context->popToken().value;
                // std::string arg0 = context->popToken().value;
                std::string dst  = "$" + std::to_string(context->getTemporaryId());

                // std::cout << "Push " << arg0 << std::endl;
                std::cout << "\tPush " << arg1 << std::endl;

                std::cout << "\t" << current << std::endl;
                std::cout << "\t\t" << "<---- top:" << dst << std::endl;

                context->pushToken(Token(Token::TemporaryId, dst));


            } else if(current == "[") {
                stream->next();

                Expression(stream, context);

                if(stream->current().value != "]") {
                    std::cout << stream->current() << std::endl;
                    throw Throwable(0, "ObjectExpression:  lost `]");
                }

                stream->next();             // eat `]

                std::string arg1 = context->popToken().value;
                // std::string arg0 = context->popToken().value;
                std::string dst  = "$" + std::to_string(context->getTemporaryId());

                // std::cout << "Push " << arg0 << std::endl;
                std::cout << "\tPush " << arg1 << std::endl;

                std::cout << "\t" << current << std::endl;
                std::cout << "\t\t" << "<---- top:" << dst << std::endl;


                context->pushToken(Token(Token::TemporaryId, dst));

            } else if(current == "(") {
                stream->next();

                // std::string call_name = context->popToken().value ;
                std::string call_name = id_or_method_name;

                std::cout << "\tBeforeCall " << call_name << std::endl;

                CallArgumentList(stream, context);

                if(stream->current().value != ")") {
                    std::cout << stream->current() << std::endl;
                    throw Throwable(0, "ObjectExpression: in CallArgumentList lost `)");
                }

                stream->next();             // eat `)

                std::cout << "\tCall " << call_name << std::endl;

                // std::string dst  = "$" + std::to_string(context->getTemporaryId());
                context->pushToken(Token(Token::TemporaryId, "$CallResult"));

                std::cout << "\t\t<---- top: $CallResult" << std::endl;

            }

            current = stream->current().value;
        }
    }


    // CallArgumentList ::= ε | Expression { "," Expression }
    static void CallArgumentList(TokenStream* stream, ParserContext* context) throw(Throwable)
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
            Expression(stream, context);

            std::string param_value = context->popToken().value;

            if(param_value.size() != 0 && param_value.at(0) != '$') {
                std::cout << "\tPush " << param_value << std::endl;
            }

            std::cout << "\tParam " << std::endl;

            while(stream->current().value == ",") {
                stream->next();
                Expression(stream, context);

                std::cout << "\tParam " << context->popToken().value << std::endl;

            }
        }
    }


    // Literal ::= StringLiteral | NumberLiteral | ArrayLiteral | ObjectLiteral
    static void Literal(TokenStream* stream, ParserContext* context) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        Token current = stream->current();
        if(current.type == Token::ObjectLiteral) {
            StringLiteral(stream, context);
        } else if(current.type == Token::NumberLiteral || current.type == Token::IntegerLiteral || current.type == Token::DoubleLiteral ) {
            NumberLiteral(stream, context);
        } else if(current.value == "{"){
            ObjectLiteral(stream, context);
        } else if(current.value == "[") {
            ArrayLiteral(stream, context);
        } else {
            throw Throwable(0, "Literal: Token not a `Literal");
        }
    }


    // ObjectLiteral ::= "{" { StringLiteral ":" Expression "," } "}"           // map 或者说是对象，不能对此进行函数调用
    static void ObjectLiteral(TokenStream* stream, ParserContext* context) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        std::string this_key_values_literal = "$" + std::to_string( context->getTemporaryId() );

        std::cout << "\tObject " << this_key_values_literal << std::endl;

        stream->next();

        while(stream->current().value != "}") {

            if(stream->current().type != Token::ObjectLiteral) {
                std::cout << stream->current() << std::endl;
                throw Throwable(0, "ObjectLiteral Map Object Key Not StringLiteral, current Token " + stream->current().value);
            }

            // `Key
            StringLiteral(stream, context);

            std::string key = context->popToken().value;

            if(stream->current().value != ":") {
                std::cout << stream->current() << std::endl;
                throw Throwable(0, "ObjectLiteral Map Object Lost `:, current Token " + stream->current().value);
            }

            stream->next();

            // `Value
            Expression(stream, context);

            std::string value = context->popToken().value;

            if(stream->current().value == ",") {
                stream->next();
            } else {
                if(stream->current().value != "}") {
                    std::cout << stream->current() << std::endl;
                    throw Throwable(0, "ObjectLiteral Key-Value lost `, current token " + stream->current().value);
                }
            }

            std::cout << "\tKey " << key << std::endl;
            std::cout << "\tValue " << value << std::endl;
            std::cout << "\tAppendToObject " << std::endl;
            std::cout << "\t\t" << "<---- top: " << this_key_values_literal << std::endl;
        }

        stream->next();

        std::cout << "\tEndObject " << this_key_values_literal << std::endl;

        context->pushToken(Token(Token::ObjectLiteral, this_key_values_literal));
    }


    // ArrayLiteral ::= "[" { Expression "," } "]"                                 // 数组，不能对此进行函数调用
    static void ArrayLiteral(TokenStream* stream, ParserContext* context) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        std::string this_array_literal = "$" + std::to_string( context->getTemporaryId() );

        std::cout << "\tArray " << this_array_literal << std::endl;

        // first `[
        stream->next();

        while(stream->current().value != "]") {
            Expression(stream, context);

            std::string value = context->popToken().value;

            if(stream->current().value == ",") {
                stream->next();
            }

            std::cout << "\tArrayItem " << value << std::endl;
            std::cout << "\tAppendToArray " << std::endl;
            std::cout << "\t\t" << "<---- top: " << this_array_literal << std::endl;

        }
        stream->next();

        std::cout << "\tEndArray " << this_array_literal << std::endl;

        context->pushToken(Token(Token::ArrayLiteral, this_array_literal));
    }


    // NumberLiteral ::= IntegerLiteral | DoubleLiteral
    static void NumberLiteral(TokenStream* stream, ParserContext* context) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        int type = stream->current().type;
        if(type == Token::IntegerLiteral) {
            IntegerLiteral(stream, context);
        } else if(type == Token::DoubleLiteral) {
            DoubleLiteral(stream, context);
        } else {
            std::cout << stream->current() << std::endl;
            throw Throwable(0, "Current Token is Not a NumberLiteral");
        }
    }


    static void IntegerLiteral(TokenStream* stream, ParserContext* context) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        context->pushToken(stream->current());

        stream->next();
    }


    static void DoubleLiteral(TokenStream* stream, ParserContext* context) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        context->pushToken(stream->current());

        stream->next();
    }

    static void StringLiteral(TokenStream* stream, ParserContext* context) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        context->pushToken(stream->current());

        stream->next();
    }


    static void Identity(TokenStream* stream, ParserContext* context) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        // TODO 检查声明情况与所在代码块
        if(stream->current().type != Token::Identity) {
            std::cout << stream->current() << std::endl;
            throw Throwable(0,stream->current().value + " isn't' Identity");
        }

        context->pushToken(stream->current());

        stream->next();
    }


    // IfStatement ::= "if" "(" Expression ")" Statement [ ElseStatement ]
    static void IfStatement(TokenStream* stream, ParserContext* context) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        int label0, label1;

        label0 = context->getNewLabel();
        label1 = context->getNewLabel();

        // std::cout << "IfStatement: " << label0 << ", " << label1 << std::endl;

        context->pushLabel(label0);
        context->pushLabel(label1);

        // first is `if
        stream->next();

        if(stream->current().value != "(") {
            throw Throwable(0,"IfStatement lost `(");
        }

        stream->next();

        Expression(stream, context);

        if(stream->current().value != ")") {
            throw Throwable(0,"IfStatement lost `)");
        }

        stream->next();

        // std::cout << "Push " << context->popToken().value << std::endl;

        std::cout << "\tJumpIfZero " << label1 << std::endl;

        std::cout << "Lable " << label0 << std::endl;

        Statement(stream, context);

        std::string ahead = stream->current().value;

        std::cout << "Lable " << label1 << std::endl;

        context->popLable();
        context->popLable();

        if(ahead == "else") {
            ElseStatement(stream, context);
        }

    }


    // ElseStatement ::= "else" Statement
    static void ElseStatement(TokenStream* stream, ParserContext* context) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        // first is `else
        stream->next();

        Statement(stream, context);
    }


    // DoWhileStatement ::= "do" Statement "while" "(" Expression ")" ";"
    static void DoWhileStatement(TokenStream* stream, ParserContext* context) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        context->enterLoopStatement();

        int label0, label1;

        label0 = context->getNewLabel();
        label1 = context->getNewLabel();

        // std::cout << "DoWhileStatement: " << label0 << ", " << label1 << ", " << label2 << std::endl;

        context->pushLoopStartLabel(label0);
        context->pushLoopEndLabel(label1);

        // first is `do
        stream->next();

        // for loop continue
        std::cout << "Label " << label0 << std::endl;

        Statement(stream, context);

        // stream->next();

        if(stream->current().value != "while") {
            throw Throwable(0,"DoWhileStatement lost `while");
        }

        stream->next();

        if(stream->current().value != "(") {
            throw Throwable(0,"DoWhileStatement while lost `(");
        }

        stream->next();

        Expression(stream, context);

        if(context->topToken().value.size() != 0 && context->topToken().value.at(0) != '$') {
            std::cout << "\tPush " << context->popToken().value << std::endl;
        }

        std::cout << "\tJumpIfNotZero " << label0 << std::endl;

        if(stream->current().value != ")") {
            throw Throwable(0,"DoWhileStatement while lost `)");
        }

        stream->next();

        if(stream->current().value != ";") {
            throw Throwable(0,"DoWhileStatement while lost `;");
        }

        stream->next();

        std::cout << "Lable " << label1  << " :" << std::endl;

        context->popLoopEndLabel();
        context->popLoopStartLabel();

        context->exitLoopStatement();
    }


    // WhileStatement ::= "while" "(" Expression ")" Statement
    static void WhileStatement(TokenStream* stream, ParserContext* context) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        context->enterLoopStatement();

        int label0, label1;

        label0 = context->getNewLabel();
        label1 = context->getNewLabel();

        // std::cout << "WhileStatement: " << label0 << ", " << label1 << ", " << label2 << std::endl;

        context->pushLoopStartLabel(label0);
        context->pushLoopEndLabel(label1);

        // first is `while
        stream->next();

        if(stream->current().value != "(") {
            throw Throwable(0,"WhileStatement lost `(");
        }

        stream->next();

        // for loop continue
        std::cout << "Label " << label0 << std::endl;

        Expression(stream, context);

        if(context->topToken().value.size() != 0 && context->topToken().value.at(0) != '$') {
            std::cout << "\tPush " << context->popToken().value << std::endl;
        }

        std::cout << "\tJumpIfZero " << label1 << std::endl;

        if(stream->current().value != ")") {
            throw Throwable(0,"WhileStatement lost `)");
        }

        stream->next();

        Statement(stream, context);

        std::cout << "\tJump " << label0 << std::endl;

        std::cout << "Label " << label1  << std::endl;

        context->popLoopEndLabel();
        context->popLoopStartLabel();

        context->exitLoopStatement();
    }

    // ForStatement ::= "for" "(" Expression ";" Expression ";" Expression ")" Statement
    static void ForStatement(TokenStream* stream, ParserContext* context) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        context->enterLoopStatement();

        int label0, label1, label2, label3;

        label0 = context->getNewLabel();
        label1 = context->getNewLabel();
        label2 = context->getNewLabel();
        label3 = context->getNewLabel();

        // std::cout << "ForStatement: " << label0 << ", " << label1 << ", " << label2 << ", " << label3 << std::endl;

        // 注意 label1 才是 continue 在 for 循环重新开始的地方，lable1 后面的语句执行后无条件跳转回 lable0
        context->pushLabel(label0);
        context->pushLoopStartLabel(label1);
        context->pushLabel(label2);
        context->pushLoopEndLabel(label3);

        // first is `for

        stream->next();

        if(stream->current().value != "(") {
            throw Throwable(0,"ForStatement lost `(");
        }

        stream->next();

        Expression(stream, context);                         // E0

        if(stream->current().value != ";") {
            throw Throwable(0,"ForStatement lost frist `;");
        }

        stream->next();

        std::cout << "\tPop " << std::endl;

        std::cout << "Label " << label0  << std::endl;

        Expression(stream, context);                         // E1

        if(stream->current().value != ";") {
            throw Throwable(0,"ForStatement lost second `;");
        }

        stream->next();

        if(context->topToken().value.size() != 0 && context->topToken().value.at(0) != '$') {
            std::cout << "\tPush " << context->popToken().value << std::endl;
        }

        std::cout << "\tJumpIfNotZero " << label2 << std::endl;

        std::cout << "\tJump " << label3 << std::endl;            // 跳出循环

        std::cout << "Label " << label1  << std::endl;

        Expression(stream, context);                         // E2

        std::cout << "\tJump " << label0 << std::endl;

        if(stream->current().value != ")") {
            throw Throwable(0,"ForStatement lost `)");
        }

        stream->next();

        std::cout << "Label " << label2 << std::endl;

        Statement(stream, context);

        std::cout << "\tJump " << label1 << std::endl;

        std::cout << "Label " << label3 << std::endl;

        context->exitLoopStatement();
    }


    // TryStatement ::= "try" Block CatchStatement
    static void TryStatement(TokenStream* stream, ParserContext* context) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        // first is `try
        stream->next();

        Block(stream, context);

        if(stream->current().value != "catch") {
            throw Throwable(0,"TryStatement lost `catch");
        }

        CatchStatement(stream, context);
    }


    // CatchStatement ::= "catch" Block
    static void CatchStatement(TokenStream* stream, ParserContext* context) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        // first is `catch
        stream->next();

        if(stream->current().value != "(") {
            throw Throwable(0,"CatchStatement lost `(");
        }

        stream->next();

        Expression(stream, context);

        if(stream->current().value != ")") {
            throw Throwable(0,"CatchStatement lost `)");
        }

        stream->next();

        Block(stream, context);
    }


    // ThrowStatement ::= "throw" Expression ";"
    static void ThrowStatement(TokenStream* stream, ParserContext* context) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        // first is `throw
        stream->next();

        Expression(stream, context);

        if(stream->current().value != ";") {
            std::cout << stream->current() << std::endl;
            throw Throwable(0, "ThrowStatement: lost `;");
        }

        stream->next();
    }


    // Block ::= "{" Statements "}"
    static void Block(TokenStream* stream, ParserContext* context) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        context->enterBlock();

        std::cout << "\tEnterBlock " << context->currentBlockId() << std::endl;

        // first is `{
        stream->next();

        if(stream->current().value == "}") {
            stream->next();
            return;
        }

        while(!stream->atEnd()) {
            Statement(stream, context);
            if(stream->current().value == "}") {
                break;
                // meaning ObjectLiteral can't declara alone with out Statement
            } else if(stream->current().value == ":") {
                std::cout << stream->current() << std::endl;
                throw Throwable(0, "ObjectLiteral: Can't Declara Alone With Out Statement !");
            }
        }

        stream->next();             // eat `}

        std::cout << "\tExitBlock " << context->currentBlockId() << std::endl;

        context->exitBlock();
    }


    // BreakStatement ::= "break" ";"
    static void BreakStatement(TokenStream* stream, ParserContext* context) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        if(!context->inLoopStatement()) {
            std::cerr << "Token `break Only In Loop Statement" << std::endl;
        }

        stream->next(); // `break

        std::cout << "\tJump " << context->getLoopEndLabel() << std::endl;


        if(stream->current().value != ";") {
            std::cout << stream->current() << std::endl;
            throw Throwable(0,"BreakStatement lost `; current token " + stream->current().value);
        }
        stream->next();
    }


    // ContinueStatement ::= "continue" ";"
    static void ContinueStatement(TokenStream* stream, ParserContext* context) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        if(!context->inLoopStatement()) {
            std::cerr << "Token `continue Only In Loop Statement" << std::endl;
        }

        stream->next(); // `continue

        std::cout << "\tJump " << context->getLoopStartLabel() << std::endl;

        if(stream->current().value != ";") {
            std::cout << stream->current() << std::endl;
            throw Throwable(0,"ContinueStatement lost `; current token " + stream->current().value);
        }
        stream->next();
    }


    // Lambda ::= "function" "(" FunctionArgumentsList ")" "->" TypeName Block
    static void Lambda(TokenStream* stream, ParserContext* context) throw(Throwable)
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
        FunctionArgumentsList(stream, context);

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

        TypeName(stream, context);

        context->enterFunctionBody();

        Block(stream, context);

        context->exitFunctionBody();
    }


    // TypeName ::= "bool" | "int" | "string" | "real" | "var"
    static void TypeName(TokenStream* stream, ParserContext* context) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        (void)context;

        std::string type = stream->current().value;
        if(type == "bool" || type == "int" || type == "string" || type == "real" || type == "var") {

            std::cout << "\tTypeName " << type << std::endl;

            stream->next();
        } else {
            std::cout << stream->current() << std::endl;
            throw Throwable(0,stream->current().value + " Not a Type!");
        }
    }


    // Function ::= "function" Identity "(" FunctionArgumentsList ")" "->" TypeName Body
    static void Function(TokenStream* stream, ParserContext* context) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        // first is `function
        stream->next();

        Identity(stream, context);

        std::cout << "\tFunction " << context->popToken().value << std::endl;

        if(stream->current().value != "(") {
            std::cout << stream->current() << std::endl;
            throw Throwable(0,"Function lost `(; current token " + stream->current().value);
        }

        stream->next();

        context->enterBlock();

        std::cout << "\tEnterBlock " << context->currentBlockId() << std::endl;

        std::cout << "\tParams " << std::endl;

        FunctionArgumentsList(stream, context);

        std::cout << "\tEndParams " << std::endl;

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

        std::cout << "\tReturnType " << std::endl;

        TypeName(stream, context);

        Block(stream, context);

        std::cout << "\tExitBlock " << context->currentBlockId() << std::endl;

        context->exitBlock();
    }


    // FunctionArgumentsList ::= ε | TypeName Identity { "," TypeName Identity }
    static void FunctionArgumentsList(TokenStream* stream, ParserContext* context) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        if(stream->current().value == ")") {
            return ;
        }

        TypeName(stream, context);

        Identity(stream, context);

        std::cout << "\tDeclara " << context->popToken().value << std::endl;

        while(stream->current().value == ",") {
            stream->next();

            TypeName(stream, context);

            Identity(stream, context);

            std::cout << "\tDeclara " << context->popToken().value << std::endl;
        }
    }


    // ReturnStatement ::= "return" ExpressionStatement
    static void ReturnStatement(TokenStream* stream, ParserContext* context) throw(Throwable)
    {
        CALLEE_PUSH_TRACK_;

        stream->next();  // `return

        ExpressionStatement(stream, context);

        std::string value = context->popToken().value;

        if(value.size() != 0) {
            std::cout << "\tReturn " << value << std::endl;
        } else {
            std::cout << "\tReturn " << std::endl;
        }


    }

};

}

#endif // PARSER_H
