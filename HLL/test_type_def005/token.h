#ifndef TOKEN_H
#define TOKEN_H

#include <QSharedDataPointer>

namespace qyvlik {
namespace typer {

//enum Type {
//    Bool,                       // bool
//    Int8,                       // int8
//    Int16,                      // int16
//    Int32,                      // int32 int
//    Int64,                      // int64 long
//    Double,                     // double
//    String,                     // string
//    Function,                   // function
//    Map,                        // map<K,V>
//    Stack,                      // stack<V>
//    Vector,                     // vector<V>
//    List,                       // list<V>
//    HashMap,                    // hashmap<K,V>
//    Set,                        // set<V>
//};


class SourceLocal {
public:
    int64_t start;
    int64_t offset;
    int64_t line;
    int64_t column;
};

class TokenData;
class Token
{
public:
    Token();
    Token(int type, const QString& value);
    Token(const Token &);
    Token &operator=(const Token &);
    ~Token();

    bool operator==(const Token&);
    bool operator!=(const Token&);
    // for map key
    bool operator <(const Token&);

    int type() const;
    void setType(int value);

    QString value() const;
    void setValue(const QString& value);

private:
    QSharedDataPointer<TokenData> data;
};

// interface
class TokenStream
{
public:
    TokenStream() { }
    virtual ~TokenStream() = 0;
    virtual size_t size() const = 0;                // 大小
    virtual size_t index() const = 0;               // 当前位置
    virtual void seek(const size_t i) = 0;    // 跳转到指定位置
    virtual Token current() const = 0;              // 当前词素
    virtual void rockBack() = 0;                    // 回滚
    virtual void next() = 0;                        // 向下
    static TokenStream *getLexerStream(const QList<Token> &lexers);
};


}
} // namespace qyvlik::typer


#endif // TOKEN_H
