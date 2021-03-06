# 提出语言文法

```
Program ::= { Function | Statements }

Statements ::= { Statement }

Statement ::= ExpressionStatement
            | IfStatement
            | WhileStatement
            | DoWhileStatement
            | ForStatement
            | BreakStatement
            | ContinueStatement
            | TryStatement
            | ThrowStatement
            | Block
            | ReturnStatement
            | DeclarationStatement

IfStatement ::= "if" "(" Expression ")" Statement [ ElseStatement ]

ElseStatement ::= "else" Statement

BreakStatement ::= "break" ";"

ContinueStatement ::= "continue" ";"

DoWhileStatement ::= "do" Statement "while" "(" Expression ")" ";"

WhileStatement ::= "while" "(" Expression ")" Statement

ForStatement ::= "for" "(" Expression ";" Expression ";" Expression ")" Statement

TryStatement ::= "try" Block CatchStatement

CatchStatement ::= "catch" Block

ThrowStatement ::= "throw" Expression ";"

Block ::= "{" Statements "}"

DeclarationStatement ::= TypeName Identity [ "=" Expression ] { "," Identity [ "=" Expression ] } ";"

ExpressionStatement ::= Expression ";" | ";"

Expression ::= ObjectExpression { ( "=" | "+=" | "-=" | "*=" | "%=" | "/=" ) BoolExpression }
             | ObjectExpression { ( ">" | "<" | ">=" | "<=" | "==" | "!=" | "+" | "-" | "*" | "/" | "%" ) Expression }
             | BoolExpression

BoolExpression ::= ArithmeticExpression { ( ">" | "<" | ">=" | "<=" | "==" | "!=" ) ArithmeticExpression }

ArithmeticExpression ::= TermExpression { ( "+" | "-" ) TermExpression }

TermExpression ::= FactorExpression { ( "*" | "/" | "%" ) FactorExpression }

FactorExpression ::= "(" Expression ")"
                   | ObjectExpression
                   | Literal
                   | Lambda
                   | UnaryExpression

UnaryExpression ::= { ( "+" | "-" | "!" ) } BoolExpression

ObjectExpression ::= ObjectExpression "." Identity
                   | ObjectExpression "[" Expression "]"
                   | ObjectExpression "(" CallArgumentList ")"
                   | Identity

CallArgumentList ::= ε | Expression { "," Expression }

Literal ::= StringLiteral | NumberLiteral | ArrayLiteral | KeyValuesLiteral

KeyValuesLiteral ::= "{" { StringLiteral ":" Expression "," } "}"           // map 或者说是对象，不能对此进行函数调用

ArrayLiteral ::= "[" { Expression "," } "]"                                 // 数组，不能对此进行函数调用

NumberLiteral ::= IntegerLiteral | DoubleLiteral

Lambda ::= "function" "(" FunctionArgumentsList ")" "->" TypeName Body

TypeName ::= "bool" | "int" | "string" | "real" | "var"

Function ::= "function" Identity "(" FunctionArgumentsList ")" "->" TypeName Block

FunctionArgumentsList ::= ε | TypeName Identity { "," TypeName Identity }

ReturnStatement ::= "return" ExpressionStatement

```

其中 `ObjectExpression` 有左递归。消除之：

```
_R_ObjectExpression ::= Identity
                      | Identity "." Identity
                      | Identity "[" Expression "]"
                      | Identity "(" CallArgumentList ")"

ObjectExpression ::= _R_ObjectExpression
                   | _R_ObjectExpression "." Identity
                   | _R_ObjectExpression "[" Expression "]"
                   | _R_ObjectExpression "(" CallArgumentList ")"
```

---

1. 冲突，`Block` 和 `KeyValuesLiteral` 的 `FIRST` 集合一样，如何处理？

    由于 `Block` 优先级大于 `KeyValuesLiteral`，所以会解析会报错，所以应该禁止在语句外部单独写 `KeyValuesLiteral` 或者 `ArrayLiteral`。

    在 `Block` 中处理掉 `StringLiteral` 后，下一个字符为 `:`。从 `Block` 出发。

    `Block -> Statement -> ExpressionStatement -> Expression -> BoolExpression -> ArithmeticExpression -> TermExpression -> FactorExpression-> ObjectExpression -> throw('ObjectExpression: not start of Identity');`

    所以应该在 `Block` 设置检测步骤。

2. 具名函数声明应该都要提前声明。并且函数声明必须带上函数实现。

~~3. 还未处理 `-1` ，`-A-B` 这样的语句。~~

4. 由于涉及上的疏忽，短路代码没有进行考虑。貌似没有设计逻辑表达式。

5. for 循环的第一个表达式可以为表达式或者为声明语句

6. for 循环的标签生成

    ```
    for(E0; E1; E2)
       S;

        E0;
    0 :
        E1;
        jump_if_not_zero 2
        jump 3;
    1 :
         E2;
         jump 0;
    2 :
         S;
         jump 1;
    3 :
    ```

    for 循环中的 `continue` 标签是跳转到何处？

6. 作用域，调用栈，计算栈，变量声明
