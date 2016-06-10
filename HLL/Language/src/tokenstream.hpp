#ifndef TOKENSTREAM_H
#define TOKENSTREAM_H

#include <iostream>
#include <vector>
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
        default:
            os << "( Unknow, ";
            break;
        }
        os << thiz.value << " )";
        return os;
    }
};

class TokenStream
{
public:
    TokenStream(const std::vector<Token> l):
        l(l),
        index(0)        // -1?
    {}

    void next() {
        index < l.size()-1 ? index ++ : 0;
    }

    void rockBack() {
        index > 0 ? index -- : 0;
    }

    Token current() const {
        return l.at(index);
    }

    bool atEnd() const {
        return  l.size() == 0
                || index == l.size()-1;
    }

private:
    std::vector<Token> l;
    size_t index;
};

#endif // TOKENSTREAM_H


