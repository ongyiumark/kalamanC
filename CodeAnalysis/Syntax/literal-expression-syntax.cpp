#include "syntax.h"

using namespace CodeAnalysis;

LiteralExpressionSyntax::LiteralExpressionSyntax(SyntaxToken* literal_token, std::any value)
    : _literal_token(literal_token) , _value(value)
{
    _children = {_literal_token};
}

LiteralExpressionSyntax::LiteralExpressionSyntax(SyntaxToken* literal_token)
    : _literal_token(literal_token) , _value(literal_token->get_value())
{
    _children = {_literal_token};
}

SyntaxToken* LiteralExpressionSyntax::get_literal_token() const
{
    return _literal_token;
}

SyntaxKind LiteralExpressionSyntax::get_kind() const
{
    return SyntaxKind::LiteralExpression;
}

std::any LiteralExpressionSyntax::get_value() const
{
    return _value;
}
