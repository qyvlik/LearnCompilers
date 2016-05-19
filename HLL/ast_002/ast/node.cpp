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

void TypeNameNode::accept(Visitor *visitor) throw(TypeError, ParserError)
{ visitor->visit(this); }

TypeDefineNode::TypeDefineNode(){
    kind = K;
}

TypeDefineNode::~TypeDefineNode()
{}

void TypeDefineNode::accept(Visitor *visitor) throw(TypeError, ParserError)
{ visitor->visit(this); }

TemplateTypeSpecifierNode::TemplateTypeSpecifierNode(){
    kind = K;
}

void TemplateTypeSpecifierNode::accept(Visitor *visitor) throw(TypeError, ParserError)
{ visitor->visit(this);  }

TypeSpecifierNode::TypeSpecifierNode()
{
    kind = K;
}

TypeSpecifierNode::~TypeSpecifierNode()
{  }

void TypeSpecifierNode::accept(Visitor *visitor) throw(TypeError, ParserError)
{ visitor->visit(this); }

TypeDefinesNode::TypeDefinesNode() {
    kind = K;
}

void TypeDefinesNode::accept(Visitor *visitor) throw(TypeError, ParserError)
{
    visitor->visit(this);
}

}
}
