# 编译原理值后缀表达式生成与计算

文法参照上一篇[编译原理之消除算术表达式文法的左递归](编译原理之消除算术表达式文法的左递归.md)。

将算数表达式解析为后缀表达式是不少脚本语言的运行时的计算方案。

根据原文法：

```
expr -> expr + term     
      | expr - term     
      | term            

term -> term * factor   
      | term / factor   
      | factor          

factor -> ( expr )      
        | number        
        | id
```

我们先罗列出要实现的函数：

```
class Parser {
public:
    void expr(LexerStream* lexers)
    {
    }

    void term(LexerStream* lexers)
    {
    }

    void factor(LexerStream* lexers)
    {
    }
};
```

> 其中的 `LexerStream` 为词素序列流。

再根据消除左递归的文法实现递归算法：

```
expr     -> term trest

trest    -> + term
          | - term
          | ε

term     -> factor frest

frest    -> * factor
          | / factor
          | ε

factor   -> NUMBER
          | ( expr )
```

先看未消除左递归的 `expr` 产生式和消除左递归 `expr` 和 `trest` 的产生式。原 `expr` 产生式本身是左递归的，所以在递归下降算法中是对应到一个内部循环。

所以先给出 `expr` 的实现：

```
void expr(LexerStream* lexers)
{
    term(lexers);
    while(lexers->current() == "+" || lexers->current() == "-") {

        string op = lexers->current();

        lexers->next();
        term(lexers);

        cout << op;     // PUSH
    }
}
```

其他产生式类似：

```
void term(LexerStream* lexers)
{
    factor(lexers);

    while(lexers->current() == "*" || lexers->current() == "/") {

        string op = lexers->current();

        lexers->next();
        factor(lexers);

        cout << op;     // PUSH
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

        lexers->next();
    }
}
```

先给 `Parser` 这个类一个 `code` 字段，用来保存后缀表达式：

```
class Parser {
public:
    void expr(LexerStream* lexers)
    {
    }

    void term(LexerStream* lexers)
    {
    }

    void factor(LexerStream* lexers)
    {
    }
    vector<string> code;
};
```

```
class Calculator {
public:
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
    
    bool isOperator(const string& s) ;
    string calcula(const string& operation, const string& arg0, const string& arg1);
    string add(const string& arg0, const string& arg1)
    string sub(const string& arg0, const string& arg1);
    string mul(const string& arg0, const string& arg1);
    string div(const string& arg0, const string& arg1);
    
    vector<string> code;
    stack<string> calc;
}
```

具体看 `result` 函数。先取出**后缀表达式**中的 `token`，如果是操作符，就从**运算结果栈**中取出两个 `token`，然后进行计算，将获取到的结果压入**运算结果栈**中；如果不是操作符，就压入**运算结果栈**中。
