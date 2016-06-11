#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <iostream>
#include <string>

class Token
{
public:
    enum Type {
        Unknow,
        KeyWord,
        Delimiter,              // [, ], {, }, (, )
        Operator,               // +,-,*,/,%,+=,*=,-=,/=,%=,>,<,>=,<=,==,!=,!,.
        Identity,

        TypeName,

        Lterial,
        StringLterial,
        NumberLterial,

        BoolLterial,
        IntegerLterial,
        DoubleLterial,

        ArrayLterial,
        KeyValuesLterial,
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
        case Token::Lterial:
            os << "( Lterial, ";
            break;
        case Token::StringLterial:
            os << "( StringLterial, ";
            break;
        case Token::NumberLterial:
            os << "( NumberLterial, ";
            break;
        case Token::ArrayLterial:
            os << "( ArrayLterial, ";
            break;
        case Token::KeyValuesLterial:
            os << "( KeyValuesLterial, ";
            break;
        case Token::BoolLterial:
            os << "( BoolLterial, ";
            break;
        case Token::IntegerLterial:
            os << "( IntegerLterial, ";
            break;
        case Token::DoubleLterial:
            os << "( DoubleLterial, ";
            break;
        default:
            os << "( Unknow, ";
            break;
        }
        os << thiz.value << " )";
        return os;
    }
};

#endif // TOKEN_HPP

