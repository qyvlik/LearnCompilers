#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <stack>
#include <assert.h>

#include <functional>
#include <sys/time.h>

using namespace std;

// std::stoi



long timeTick(const std::function<void(void)>& t)
{
    timeval start, end;

    gettimeofday( &start, NULL );

    t();

    gettimeofday( &end, NULL );

    //long timeuse =
    return 1000000 * ( end.tv_sec - start.tv_sec ) + end.tv_usec - start.tv_usec;
    //return end.tv_usec - start.tv_usec;
}

class LexerStream
{
public:
    LexerStream(const vector<string> l):
        l(l),
        index(0)
    {}

    void next() {
        index < l.size()-1 ? index ++ : 0;
    }

    void rockBack() {
        index > 0 ? index -- : 0;
    }

    string current() const {
        return l.at(index);
    }

private:
    vector<string> l;
    size_t index;
};

// expr -> expr + term
//       | expr - term
//       | term

// term -> term * factor
//       | term / factor
//       | factor

// factor -> ( expr )
//         | number
//         | id


// expr     -> term trest

// trest    -> + term
//           | - term
//           | ε

// term     -> factor frest

// frest    -> * factor
//           | / factor
//           | ε

// factor   -> NUMBER
//           | ( expr )

class Calculator {
public:
    Calculator()
    {}

    Calculator(const vector<string>& code):
        code(code)
    { }

    // op 后必定有两个操作数

    bool isOperator(const string& s) {
        return s == "+" || s == "-" || s == "*" || s == "/";
    }

    string result() {

        string current;

        do {
            current = code.front();
            code.erase(code.begin());

            if(!isOperator(current)) {
                calc.push(current);
            } else {

                string arg0 = calc.top();
                calc.pop();
                string arg1 = calc.top();
                calc.pop();

                calc.push(calcula(current, arg0, arg1));
            }
        } while(!code.empty());

        return calc.top();
    }

    string calcula(const string& operation, const string& arg0, const string& arg1) {

        if(operation == "+") {

            return add(arg0, arg1);
        } else if(operation == "-") {

            return sub(arg0, arg1);
        } else if(operation == "*") {

            return mul(arg0, arg1);
        } else if(operation == "/") {

            return div(arg0, arg1);
        } else {
            assert(0);
            return string("0");
        }
    }

    string add(const string& arg0, const string& arg1) {
        int a0 = std::stoi(arg0);
        int a1 = std::stoi(arg1);
        return std::to_string(a0 + a1);
    }

    string sub(const string& arg0, const string& arg1) {
        int a0 = std::stoi(arg0);
        int a1 = std::stoi(arg1);
        return std::to_string(a0 - a1);
    }

    string mul(const string& arg0, const string& arg1) {
        int a0 = std::stoi(arg0);
        int a1 = std::stoi(arg1);
        return std::to_string(a0 * a1);
    }

    string div(const string& arg0, const string& arg1) {
        int a0 = std::stoi(arg0);
        int a1 = std::stoi(arg1);

        assert(a1 != 0);

        return std::to_string(a0 / a1);
    }

    vector<string> code;
    stack<string> calc;
};


class Parser {
public:
    void expr(LexerStream* lexers)
    {
        term(lexers);
        while(lexers->current() == "+" || lexers->current() == "-") {

            string op = lexers->current();

            lexers->next();
            term(lexers);

            cout << op;     // PUSH
            code.push_back(op);
        }
    }

    void term(LexerStream* lexers)
    {
        factor(lexers);

        while(lexers->current() == "*" || lexers->current() == "/") {

            string op = lexers->current();

            lexers->next();
            factor(lexers);

            cout << op;     // PUSH
            code.push_back(op);
        }
    }

    void factor(LexerStream* lexers)
    {
        string current = lexers->current();

        if(current == "(") {
            lexers->next();

            expr(lexers);
            lexers->next();

            string e = lexers->current();
            if(e != ")") {
                cout << "lost `(" << endl;
            }
        } else {
            cout << current;      // PUSH

            code.push_back(current);

            lexers->next();
        }
    }

    vector<string> code;
};


int main()
{

    // 1 + 1
    //    vector<string> l;
    //    l.push_back("1");
    //    l.push_back("+");
    //    l.push_back("1");

    // 2 * ( 1 + 1 )
    vector<string> l;
    l.push_back("2");
    l.push_back("*");
    l.push_back("(");
    l.push_back("1");
    l.push_back("+");
    l.push_back("1");
    l.push_back(")");


    LexerStream stream(l);

    Parser parser;
    parser.expr(&stream);

    long usetime = timeTick([&](){
         Calculator calculator;
         int count = 100000;
         while(count-- != 0) {
             calculator.code = parser.code;
             calculator.result();
         }
     });

     cout << endl;

     cout << "usetime:" << usetime;
}
