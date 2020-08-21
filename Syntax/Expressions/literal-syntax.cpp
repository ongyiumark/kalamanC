#include "syntax-expressions.h"

using namespace Syntax;

LiteralExpressionSyntax::LiteralExpressionSyntax(SyntaxToken* literal_token, Objects::Object* value)
    : _literal_token(literal_token), _value(value) 
{
    _children = {literal_token};
}

LiteralExpressionSyntax::LiteralExpressionSyntax(SyntaxToken* literal_token)
    : _literal_token(literal_token), _value(literal_token->get_object()) 
{
    _children = {literal_token};
}

SyntaxKind LiteralExpressionSyntax::kind() const
{
    return SyntaxKind::LiteralExpression;
}

Objects::Object* LiteralExpressionSyntax::get_object() const
{
    return _value;
}