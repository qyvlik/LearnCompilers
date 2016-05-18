#include "node.h"
#include "visitor.h"

namespace qyvlik {
namespace typer {

Node::~Node()
{}

TypeNameNode::TypeNameNode(){
    kind = K;
}

TypeNameNode::~TypeNameNode()
{}

void TypeNameNode::accept(Visitor *visitor)
{ visitor->visit(this); }

TypeDefineNode::TypeDefineNode(){
    kind = K;
}

TypeDefineNode::~TypeDefineNode()
{}

void TypeDefineNode::accept(Visitor *visitor)
{ visitor->visit(this); }

TemplateTypeSpecifierNode::TemplateTypeSpecifierNode(){
    kind = K;
}

void TemplateTypeSpecifierNode::accept(Visitor *visitor)
{ visitor->visit(this);  }

TypeSpecifierNode::TypeSpecifierNode(){
    kind = K;
}

TypeSpecifierNode::~TypeSpecifierNode()
{  }

void TypeSpecifierNode::accept(Visitor *visitor)
{ visitor->visit(this); }

}
}
