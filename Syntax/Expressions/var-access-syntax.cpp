#include "syntax-expressions.h"

using namespace Syntax;

// Accesses an existing variable.
VarAccessExpressionSyntax::VarAccessExpressionSyntax(SyntaxToken* identifier)
    : _identifier(identifier) {}
    
SyntaxKind VarAccessExpressionSyntax::kind() const
{
    return SyntaxKind::VarAccessExpression;
}

SyntaxToken* VarAccessExpressionSyntax::get_identifier() const
{
    return _identifier;
}