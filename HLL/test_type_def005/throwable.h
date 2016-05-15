#ifndef THROWABLE_H
#define THROWABLE_H

#include <QString>

namespace qyvlik {
namespace typer {

class Throwable {
public:
    Throwable();
    Throwable(int code, const QString& message);
    Throwable(const Throwable& throwable);
    virtual ~Throwable() = 0;

    int code() const;

    void setCode(int code);

    QString message() const;

    void setMessage(const QString &message);

    virtual bool isError( )const = 0;

    void pritf() const;
private:
    int m_code;
    QString m_message;
};

class Exception : public Throwable
{
public:
    virtual ~Exception();
    bool isError()const;
};

class Error : public Throwable
{
public:
    Error(int code, const QString& message);
    virtual ~Error();
    bool isError() const;
};


}
}

#endif // THROWABLE_H
