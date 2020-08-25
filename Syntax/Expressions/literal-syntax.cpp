#include "syntax-expressions.h"

using namespace Syntax;

using Diagnostics::Position;

// This is for literals such at booleans, integers, doubles, and strings.
LiteralExpressionSyntax::LiteralExpressionSyntax(Objects::Object* value, Position pos)
    : SyntaxNode(pos), _value(value) {}

LiteralExpressionSyntax::~LiteralExpressionSyntax()
{
    delete _value;
}

SyntaxKind LiteralExpressionSyntax::kind() const
{
    return SyntaxKind::LiteralExpression;
}

Objects::Object* LiteralExpressionSyntax::get_object()
{
    return _value;
}