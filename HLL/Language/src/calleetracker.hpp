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
    ~StackFrame()
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
        { CalleeTracker::threadSingleton().push(StackFrame(f,l,c)); }

        ~DoDestory()
        { CalleeTracker::threadSingleton().pop(); }
    };

    static CalleeTracker& threadSingleton() {
        thread_local static CalleeTracker r;
        return r;
    }

    void push(const StackFrame& sFrame) {
        stackFrames->push_back(sFrame);
    }

    void pop() {
        if(!stackFrames->empty()) {
            stackFrames->pop_back();
        }
    }

    void printTrack() const {
        auto iter = stackFrames->begin();
        auto end = stackFrames->end();
        while(iter != end) {
            (*iter).print();
            std::cout << std::endl;
            iter++;
        }
    }

    std::vector<StackFrame> getCallStack() const {
        return *stackFrames;
    }

    ~CalleeTracker()
    {}

protected:
    CalleeTracker():
        stackFrames(new std::vector<StackFrame>())
    {}
private:
    bool avoid_fail:1;            // avoid thread_loacl variables destory fail
    std::shared_ptr< std::vector<StackFrame> > stackFrames;
};

#define CALLEE_PUSH_TRACK_ \
    CalleeTracker::DoDestory nil(__FILE__, __LINE__, __PRETTY_FUNCTION__); (void)nil;

#define CALLEE_PRINT_TRACK \
    CalleeTracker::threadSingleton().printTrack();


#endif // CALLEETRACKER_H
