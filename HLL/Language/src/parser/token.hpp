#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <iostream>
#include <string>

namespace qyvlik {

class Token
{
public:
    enum Type {
        Unknow,
        KeyWord,
        Delimiter,              // [, ], {, }, (, )
        Operator,               // +,-,*,/,%,+=,*=,-=,/=,%=,>,<,>=,<=,==,!=,!,.
        Identity,
        TemporaryId,

        TypeName,

        Literal,
        ObjectLiteral,
        NumberLiteral,

        BoolLiteral,
        IntegerLiteral,
        DoubleLiteral,

        ArrayLiteral,
        KeyValuesLiteral,
    };

    Token():
        type(Unknow),
        value("")
    { }

    Token(Type t, const std::string& val):
        type(t),
        value(val)
    { }

    int type;
    std::string value;

    friend std::ostream& operator << (std::ostream& os, const Token& thiz) {
        os << "Token";
        switch(thiz.type)
        {
        case Token::TypeName:
            os << "( TypeName, ";
            break;
        case Token::KeyWord:
            os << "( KeyWord, ";
            break;
        case Token::Delimiter:
            os << "( Delimiter, ";
            break;
        case Token::Operator:
            os << "( Operator, ";
            break;
        case Token::Identity:
            os << "( Identity, ";
            break;
        case Token::Literal:
            os << "( Literal, ";
            break;
        case Token::ObjectLiteral:
            os << "( StringLiteral, ";
            break;
        case Token::NumberLiteral:
            os << "( NumberLiteral, ";
            break;
        case Token::ArrayLiteral:
            os << "( ArrayLiteral, ";
            break;
        case Token::KeyValuesLiteral:
            os << "( KeyValuesLiteral, ";
            break;
        case Token::BoolLiteral:
            os << "( BoolLiteral, ";
            break;
        case Token::IntegerLiteral:
            os << "( IntegerLiteral, ";
            break;
        case Token::DoubleLiteral:
            os << "( DoubleLiteral, ";
            break;
        default:
            os << "( Unknow, ";
            break;
        }
        os << thiz.value << " )";
        return os;
    }
};

}

#endif // TOKEN_HPP

