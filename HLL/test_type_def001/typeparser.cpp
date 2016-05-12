#include "typeparser.h"

namespace qyvlik {
namespace typer {


QStringList ISymbol::baseTemplateTypeName = std::initializer_list<QString>{"vector", "map", "stack", "function"};

QStringList ISymbol::baseTypeName = std::initializer_list<QString>{"int", "real", "bool", "string"};

QSet<QString> ISymbol::customType;



// start of `let



// start of `TypeName




// start of `,












}
} // namespace qyvlik::lexer

