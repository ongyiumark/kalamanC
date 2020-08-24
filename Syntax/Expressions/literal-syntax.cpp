#include "syntax-expressions.h"

using namespace Syntax;

// This is for literals such at booleans, integers, doubles, and strings.
LiteralExpressionSyntax::LiteralExpressionSyntax(Objects::Object* value)
    : _value(value) {}

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