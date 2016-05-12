#include "throwable.h"

#include <QDebug>

namespace qyvlik {
namespace typer {

int Throwable::code() const
{
    return m_code;
}

void Throwable::setCode(int code)
{
    m_code = code;
}

QString Throwable::message() const
{
    return m_message;
}

void Throwable::setMessage(const QString &message)
{
    m_message = message;
}

void Throwable::pritf() const
{
    qDebug() << "code :" << m_code << ", message :" << m_message;
}

Throwable::Throwable():
    m_code(0),
    m_message("")
{}

Throwable::Throwable(int code, const QString &message):
    m_code(code),
    m_message(message)
{}

Throwable::Throwable(const Throwable &throwable):
    m_code(throwable.m_code),
    m_message(throwable.m_message)
{}

Throwable::~Throwable() {}

Error::Error(int code, const QString &message):
    Throwable(code, message)
{

}

Error::~Error()
{

}

bool Error::isError() const {
    return true;
}

Exception::~Exception()
{

}

bool Exception::isError() const {
    return false;
}

}
}
