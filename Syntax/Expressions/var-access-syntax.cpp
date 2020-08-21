#include "syntax-expressions.h"

using namespace Syntax;

VarAccessExpressionSyntax::VarAccessExpressionSyntax(SyntaxToken* identifier)
    : _identifier(identifier)
{
    _children = {identifier};
}

SyntaxKind VarAccessExpressionSyntax::kind() const
{
    return SyntaxKind::VarAccessExpression;
}

SyntaxToken* VarAccessExpressionSyntax::get_identifier() const
{
    return _identifier;
}