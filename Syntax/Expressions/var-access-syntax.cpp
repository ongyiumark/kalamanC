#include "syntax-expressions.h"

using namespace Syntax;

// Accesses an existing variable.
VarAccessExpressionSyntax::VarAccessExpressionSyntax(SyntaxToken identifier)
    : _identifier(identifier) {}
    
VarAccessExpressionSyntax::~VarAccessExpressionSyntax() {}

SyntaxKind VarAccessExpressionSyntax::kind() const
{
    return SyntaxKind::VarAccessExpression;
}

SyntaxToken* VarAccessExpressionSyntax::get_identifier()
{
    return &_identifier;
}