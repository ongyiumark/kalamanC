#include "syntax-expressions.h"

using namespace Syntax;
using Diagnostics::Position;
// Accesses an existing variable.
VarAccessExpressionSyntax::VarAccessExpressionSyntax(SyntaxToken identifier, Position pos)
    : SyntaxNode(pos), _identifier(identifier) {}
    
VarAccessExpressionSyntax::~VarAccessExpressionSyntax() {}

SyntaxKind VarAccessExpressionSyntax::kind() const
{
    return SyntaxKind::VarAccessExpression;
}

SyntaxToken* VarAccessExpressionSyntax::get_identifier()
{
    return &_identifier;
}