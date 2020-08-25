#include "syntax-expressions.h"

using namespace Syntax;
using Diagnostics::Position;
// Assigns a value to an existing variable.
VarAssignExpressionSyntax::VarAssignExpressionSyntax(SyntaxToken identifier, SyntaxNode* value, Position pos)
    : SyntaxNode(pos), _identifier(identifier), _value(value) {}

VarAssignExpressionSyntax::~VarAssignExpressionSyntax()
{
    delete _value;
}

SyntaxKind VarAssignExpressionSyntax::kind() const
{
    return SyntaxKind::VarAssignExpression;
}

SyntaxNode* VarAssignExpressionSyntax::get_value()
{
    return _value;
}

SyntaxToken* VarAssignExpressionSyntax::get_identifier()
{
    return &_identifier;
}