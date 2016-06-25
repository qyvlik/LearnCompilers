#ifndef THROWABLE_H
#define THROWABLE_H

#include <iostream>
#include <string>
#include <vector>

#include "../utility/calleetracker.hpp"

namespace qyvlik {

class Throwable
{
public:
    Throwable():
        errorCode(0),
        error("")
    {}

    Throwable(int code, const std::string& err):
        errorCode(code),
        error(err),
        callStack(CalleeTracker::threadSingleton().getCallStack())
    { }

    virtual ~Throwable()
    {}

    int getErrorCode() const
    {
        return errorCode;
    }

    void setErrorCode(int value)
    {
        errorCode = value;
    }

    std::string getError() const
    {
        return error;
    }

    void setError(const std::string &value)
    {
        error = value;
    }

    void printTrack() const {
        auto iter = callStack.begin();
        auto end = callStack.end();
        while(iter != end) {
            (*iter).print();
            std::cout << std::endl;
            iter++;
        }
    }

    friend std::ostream& operator << (std::ostream& os, const Throwable& thiz) {
        os << thiz.getErrorCode() << " " << thiz.getError();
        return os;
    }

private:
    int errorCode;
    std::string error;
    std::vector<StackFrame> callStack;
};

}

#endif // THROWABLE_H
