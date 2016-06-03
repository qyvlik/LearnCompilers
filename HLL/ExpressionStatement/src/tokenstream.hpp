#ifndef TOKENSTREAM_H
#define TOKENSTREAM_H

#include <vector>
#include <string>

class Token
{
public:
    enum {
        Identity,
        Literal,
        StringLterial,
        NumberLterial,
        Other,
    };

    int type;
    std::string value;
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
