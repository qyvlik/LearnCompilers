#include <QCoreApplication>

#include <iostream>

class SourceLocal {
public:
    qint64 start;
    qint64 offset;
    qint64 line;
    qint64 column;
};

class Node;
class TypeDefineNode;
class TypeSpecifierNode;
class TemplateTypeSpecifierNode;
class TypeNameNode;

class Visitor {
public:
    virtual ~Visitor();
    virtual void visit(Node*);
    virtual void visit(TypeDefineNode*);
    virtual void visit(TypeSpecifierNode*);
    virtual void visit(TemplateTypeSpecifierNode*);
    virtual void visit(TypeNameNode*);
};

#define DECLARE_AST_NODE(name) \
    enum { K = Kind_##name };

class Node{
public:
    enum Type {
        Kind_TypeDefine,
        Kind_TypeSpecifier,
        Kind_TemplateTypeSpecifier,
        Kind_TypeName
    };

    virtual ~Node()
    {}
    virtual void accept(Visitor* visitor) = 0 ;

    int kind;
};

class TypeSpecifierNode : public Node
{
public:
    DECLARE_AST_NODE(TypeSpecifier)
    TypeSpecifierNode(){
        kind = K;
    }

    ~TypeSpecifierNode()
    {  }
    void accept(Visitor *visitor)  override
    { visitor->visit(this); }
};

// TemplateTypeSpecifier : templateName < TypeSpecifierNode (, TypeSpecifierNode)* >
class TemplateTypeSpecifierNode : public TypeSpecifierNode
{
public:
    DECLARE_AST_NODE(TemplateTypeSpecifier)
    TemplateTypeSpecifierNode(){
        kind = K;
    }
    SourceLocal templateName;
    std::list<TypeSpecifierNode*> nodes;

    void accept(Visitor *visitor)  override
    { visitor->visit(this);  }
};

// 叶子
class TypeNameNode final : public TypeSpecifierNode
{
public:
    DECLARE_AST_NODE(TypeName)
    TypeNameNode(){
        kind = K;
    }
    ~TypeNameNode()
    {}

    void accept(Visitor *visitor)  override
    { visitor->visit(this); }
};

class TypeDefineNode : public Node
{
public:
    DECLARE_AST_NODE(TypeDefine)
    TypeDefineNode(){
        kind = K;
    }
    ~TypeDefineNode()
    {}

    void accept(Visitor *visitor) override
    { visitor->visit(this); }

    SourceLocal let;
    TypeSpecifierNode* typeSpecifierNode;
    SourceLocal as;
    TypeNameNode* typeNameNode;
};

void test();
void test2();

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    test2();



    return 0;

    //return a.exec();
}


Visitor::~Visitor()
{}

void Visitor::visit(Node * node)
{
    Q_ASSERT(node != nullptr);
    std::cout << "Node(" << node << ")" << std::endl;
}

void Visitor::visit(TypeDefineNode * node)
{
    Q_ASSERT(node != nullptr);
    std::cout << "TypeDefineNode(" << node << ")" << std::endl;
    this->visit(node->typeSpecifierNode);
    this->visit(node->typeNameNode);
}

void Visitor::visit(TypeSpecifierNode * node)
{
    Q_ASSERT(node != nullptr);
    //    std::cout << "TypeSpecifierNode(" << node << ")" << std::endl;
    if(node->kind == Node::Kind_TypeName) {
        this->visit(dynamic_cast<TypeNameNode*>(node));
    } else if(node->kind == Node::Kind_TemplateTypeSpecifier) {
        this->visit(dynamic_cast<TemplateTypeSpecifierNode*>(node));
    }

}

void Visitor::visit(TemplateTypeSpecifierNode * node)
{
    Q_ASSERT(node != nullptr);
    std::cout << "TemplateTypeSpecifierNode(" << node << ")" << std::endl;
    auto iter = node->nodes.begin();
    auto end = node->nodes.end();
    while(iter != end) {
        this->visit(*iter);
        iter++;
    }
}

void Visitor::visit(TypeNameNode * node)
{
    Q_ASSERT(node != nullptr);
    std::cout << "TypeNameNode(" << node << ")" << std::endl;
}

void test(){
    Visitor visitor;

    // let int as int8 ;

    std::cout << "let int as int8 ;" << std::endl;

    TypeNameNode int_type ;
    TypeNameNode int8_type ;

    TypeDefineNode typeDefineNode;
    typeDefineNode.typeSpecifierNode = &int_type;
    typeDefineNode.typeNameNode = &int8_type;
    typeDefineNode.accept(&visitor);

}

void test2() {
    Visitor visitor;

    // let stack<int> as IntStack ;
    std::cout << "let stack<int> as IntStack ;" << std::endl;

    TemplateTypeSpecifierNode stack_int;
    TypeNameNode int_type;
    stack_int.nodes.push_back(&int_type);

    TypeNameNode int_stack_type;

    TypeDefineNode typeDefineNode;
    typeDefineNode.typeSpecifierNode = &stack_int;
    typeDefineNode.typeNameNode = &int_stack_type;

    typeDefineNode.accept(&visitor);
}
