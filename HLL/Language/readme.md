# 提出语言文法

```
IfStatement ::= "if" "(" Expression ")" Statement [ ElseStatement ]

ElseStatement ::= "else" Statement

Break ::= "break"

Continue ::= "continue"

BreakStatement ::= Break ";"

ContinueStatement ::= Continue ";"

DoWhileStatement ::= "do" Statement "while" "(" Expression ")" ";"

WhileStatement ::= "while" "(" Expression ")" Statement

ForStatement ::= "for" "(" Expression ";" Expression ";" Expression ")" Statement

TryStatement ::= "try" Block CatchStatement

CatchStatement ::= "catch" Block

Block ::= "{" Statements "}"

Statements ::= { Statement }

Statement ::= ExpressionStatement
            | IfStatement
            | WhileStatement
            | DoWhileStatement
            | ForStatement
            | BreakStatement
            | ContinueStatement
            | TryStatement
            | Block

ExpressionStatement ::= Expression ";" | ";"

Expression ::= ObjectExpression { ( "=" | "+=" | "-=" | "*=" | "%=" | "/=" ) BoolExpression } | BoolExpression

BoolExpression ::= ArithmeticExpression { ( ">" | "<" | ">=" | "<=" | "==" | "!=" ) ArithmeticExpression }

ArithmeticExpression ::= TermExpression { ( "+" | "-" ) TermExpression }

TermExpression ::= FactorExpression { ( "*" | "/" | "%" ) FactorExpression }

FactorExpression ::= "(" Expression ")"
                   | ObjectExpression
                   | Lterial
                   | Lambda

ObjectExpression ::= ObjectExpression "." Identity
                   | ObjectExpression "[" Expression "]"
                   | ObjectExpression "(" CallArgumentList ")"
                   | Identity

CallArgumentList ::= ε | Expression { "," Expression }

Lterial ::= StringLterial | NumberLterial | ArrayLterial | KeyValuesLterial

KeyValuesLterial ::= "{" { StringLterial ":" Expression "," } "}"           // map 或者说是对象，不能对此进行函数调用

ArrayLterial ::= "[" { Expression "," } "]"                                 // 数组，不能对此进行函数调用

Lambda ::= "function" "(" FunctionArgumentsList ")" "->" TypeName Body

TypeName ::= "bool" | "int" | "string" | "real" | "var"

Function ::= "function" Identity "(" FunctionArgumentsList ")" "->" TypeName Body

FunctionArgumentsList ::= TypeName Identity { "," TypeName Identity }

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
