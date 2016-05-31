#ifndef LEXERSTREAM_H
#define LEXERSTREAM_H

#include <vector>
#include <string>

class LexerStream
{
public:
    LexerStream(const std::vector<std::string> l):
        l(l),
        index(0)        // -1?
    {}

    void next() {
        index < l.size()-1 ? index ++ : 0;
    }

    void rockBack() {
        index > 0 ? index -- : 0;
    }

    std::string current() const {
        return l.at(index);
    }

    bool atEnd() const {
        return  l.size() == 0
                || index == l.size()-1;
    }

private:
    std::vector<std::string> l;
    size_t index;
};

#endif // LEXERSTREAM_H
