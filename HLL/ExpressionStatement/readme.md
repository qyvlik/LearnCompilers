# 表达式构建

## 左值

这里简单的规定，左值为标识符，或者标识符的字段。可被 `=` 赋值的。

## 右值

字面量，以及函数返回值，字面量的字段，字面量函数返回值，左值的函数调用返回值。

```
ExpressionStatement ::= Expression ";" | ";"

Expression ::= LeftValue { ( "=" | "+=" | "-=" | "*=" | "%=" | "/=" ) BoolExpression }
             | BoolExpression
             | RightValue

BoolExpression ::= ArithmeticExpression { ( ">" | "<" | ">=" | "<=" | "==" | "!=" ) ArithmeticExpression }

ArithmeticExpression ::= TermExpression { ( "+" | "*" ) TermExpression }

TermExpression ::= FactorExpression { ( "*" | "/" | "%" ) FactorExpression }

FactorExpression ::= "(" Expression ")"
                   | LeftValue
                   | RightValue

RightValue ::= RightValue "(" CallArgumentList ")"
             | RightValue "[" Expression "]"
             | RightValue "." Identity
             | Lterial
             | LeftValue "(" CallArgumentList ")"
             | "[" { Expression "," } "]"                           // 数组，不能对此进行函数调用
             | "{" { StringLterial ":" Expression "," } "}"         // map 或者说是对象，不能对此进行函数调用

LeftValue ::= LeftValue "." Identity
            | LeftValue "[" Expression "]"
            | Identity

CallArgumentList ::= ε
                   | Expression { "," Expression }

Lterial ::= StringLterial | NumberLterial

```

其中 `RightValue` 和 `LeftValue` 有左递归。消除之：

```
R_LeftValue ::= Identity
              | Identity "." Identity
              | Identity "[" Expression "]"

LeftValue ::= R_LeftValue
            | R_LeftValue "." Identity
            | R_LeftValue "[" Expression "]"

R_RightValue ::= Lterial
               | Lterial "(" CallArgumentList ")"
               | Lterial "[" Expression "]"
               | Lterial "." Identity
               | LeftValue "(" CallArgumentList ")"
               | "[" { Expression "," } "]"                           // 数组，不能对此进行函数调用
               | "{" { StringLterial ":" Expression "," } "}"         // map 或者说是对象，不能对此进行函数调用

RightValue ::= R_RightValue
             | R_RightValue "(" CallArgumentList ")"
             | R_RightValue "[" Expression "]"
             | R_RightValue "." Identity

```




