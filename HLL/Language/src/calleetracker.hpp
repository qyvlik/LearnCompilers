#ifndef CALLEETRACKER_H
#define CALLEETRACKER_H

#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <memory>

struct StackFrame {
    StackFrame(const char* f, int l,const char* c):
        file(f),
        line(l),
        callname(c)
    {}

    std::string file;
    int line;
    std::string callname;

    void print() const {
        std::cout << file << ":" << line << "@" << callname;
    }
};

class CalleeTracker
{
public:
    class DoDestory
    {
    public:
        DoDestory(const char* f, int l, const char* c)
        { CalleeTracker::shared()->push(StackFrame(f,l,c)); }

        ~DoDestory()
        { CalleeTracker::shared()->pop(); }
    };

    static std::shared_ptr<CalleeTracker> shared() {
        static std::shared_ptr<CalleeTracker> s(new CalleeTracker);
        return s;
    }

    void push(const StackFrame& sFrame) {
        stackframes.push_back(sFrame);
    }

    void pop() {
        if(!stackframes.empty()) {
            stackframes.pop_back();
        }
    }

    void printTrack() const {
        auto iter = stackframes.begin();
        auto end = stackframes.end();
        while(iter != end) {
            (*iter).print();
            std::cout << std::endl;
            iter++;
        }
    }

    std::vector<StackFrame> getCallStack() const {
        return stackframes;
    }

protected:
    CalleeTracker()
    {}

private:
    std::vector<StackFrame> stackframes;
};


#define CALLEE_PUSH_TRACK_ \
    CalleeTracker::DoDestory nil(__FILE__, __LINE__, __PRETTY_FUNCTION__); (void)nil;

#define CALLEE_PRINT_TRACK \
    CalleeTracker::shared()->printTrack();


#endif // CALLEETRACKER_H
