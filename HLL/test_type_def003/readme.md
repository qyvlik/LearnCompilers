# 简单的模仿类型推导系统

给出指定的文法如下：

```
LET
    : 'let'
    ;

COMMA
    : ','
    ;

AS
    : 'as'
    ;

LAngleBrackets
    : '<'
    ;

RAngleBrackets
    : '>'
    ;

TypeSpecifier
    : TypeName
    | TemplateTypeSpecifier
    ;

TypeDefine
    : LET TypeSpecifier AS TypeName
    ;

TypeDefines:
    : (TypeDefine)*
    ;

TemplateTypeSpecifier:
    : TemplateTypeName ( LAngleBrackets TypeSpecifier (COMMA TypeSpecifier)* RAngleBrackets )*
    ;

TemplateTypeName
    :
    ;

TypeName
    :
    ;
```

使用范例如下：

```
let stack<int> as IntStack;
let vector<IntStack> as IntStackVector;
```

如上两条类型声明语句，将 `stack<int>` 声明为 `IntStack`。

在进行声明时会检查此类型名是否是模板，有几个模板参数，是否已经在类型系统中存在等等。

这个 demo 将尽力达到目标。
