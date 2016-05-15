#include "token.h"

namespace qyvlik {
namespace typer {

class TokenData : public QSharedData
{
public:
    TokenData():
        type(0),
        value("")
    {}
    TokenData(int t, const QString& v):
        type(t),
        value(v)
    {}
    int type;
    QString value;
};

Token::Token() : data(new TokenData)
{

}

Token::Token(int type, const QString &value):
    data(new TokenData(type, value))
{

}

Token::Token(const Token &rhs) : data(rhs.data)
{

}

Token &Token::operator=(const Token &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

Token::~Token()
{

}

bool Token::operator==(const Token &rhs)
{
    return this->data->type == rhs.data->type && this->data->value == rhs.data->value;
}

bool Token::operator!=(const Token &rhs)
{
    return !(this->data->type == rhs.data->type && this->data->value == rhs.data->value);
}

bool Token::operator <(const Token &rhs)
{
    return this->data->type < rhs.data->type && this->data->value < rhs.data->value;
}

int Token::type() const
{
    return data->type;
}

void Token::setType(int value)
{
    data->type = value;
}

QString Token::value() const
{
    return data->value;
}

void Token::setValue(const QString &value)
{
    data->value = value;
}

TokenStream::~TokenStream()
{  }

class ITokenStream : public TokenStream
{
public:
    ITokenStream(const QList<Token> &lexers):
        m_lexers(lexers),
        m_index(0)
    {}

    virtual ~ITokenStream() {}

    // LexerStream interface
public:
    size_t size() const override
    {
        return m_lexers.size();
    }
    size_t index() const override
    {
        return m_index;
    }
    void seek(const size_t i) override
    {
        if(i < size()) {
            m_index = i;
        }
    }
    Token current() const override
    {
        return m_lexers.at(m_index);
    }
    void rockBack() override
    {
        m_index >= 1 ? m_index-- : m_index;
    }

    void next() override
    {
        m_index < size()-1 ? m_index ++ : m_index;
    }
private:
    QList<Token> m_lexers;
    size_t m_index;
};

TokenStream *TokenStream::getLexerStream(const QList<Token> &lexers)
{
    return new ITokenStream(lexers);
}

}
} // namespace qyvlik::typer
