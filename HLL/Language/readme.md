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

Break ::= "break"

Continue ::= "continue"

BreakStatement ::= Break ";"

ContinueStatement ::= Continue ";"

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

ObjectExpression ::= ObjectExpression "." Identity
                   | ObjectExpression "[" Expression "]"
                   | ObjectExpression "(" CallArgumentList ")"
                   | Identity

CallArgumentList ::= ε | Expression { "," Expression }

Literal ::= StringLiteral | NumberLiteral | ArrayLiteral | KeyValuesLiteral

KeyValuesLiteral ::= "{" { StringLiteral ":" Expression "," } "}"           // map 或者说是对象，不能对此进行函数调用

ArrayLiteral ::= "[" { Expression "," } "]"                                 // 数组，不能对此进行函数调用

Lambda ::= "function" "(" FunctionArgumentsList ")" "->" TypeName Body

TypeName ::= "bool" | "int" | "string" | "real" | "var"

Function ::= "function" Identity "(" FunctionArgumentsList ")" "->" TypeName Block

FunctionArgumentsList ::= ε | TypeName Identity { "," TypeName Identity }

Return ::= "return"

ReturnStatement ::= Return ExpressionStatement

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

    由于 `Block` 优先级大于 `KeyValuesLiteral`
