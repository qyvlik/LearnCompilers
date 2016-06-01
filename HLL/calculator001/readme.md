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

详细代码前往 [LearnCompilers/HLL/calculator001](https://github.com/qyvlik/LearnCompilers/tree/master/HLL/calculator001) 下载。
