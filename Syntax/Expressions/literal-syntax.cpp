#include "syntax-expressions.h"

using namespace Syntax;

// This is for literals such at booleans, integers, doubles, and strings.
LiteralExpressionSyntax::LiteralExpressionSyntax(SyntaxToken* literal_token, Objects::Object* value)
    : _literal_token(literal_token), _value(value) {}

LiteralExpressionSyntax::LiteralExpressionSyntax(SyntaxToken* literal_token)
    : _literal_token(literal_token), _value(literal_token->get_object()) 
{
    _children = {literal_token};
}

// Tokens are already deleted by the parser.
// I delete the object here because I create new objects for boolean values.
LiteralExpressionSyntax::~LiteralExpressionSyntax()
{
    delete _value;
}

SyntaxKind LiteralExpressionSyntax::kind() const
{
    return SyntaxKind::LiteralExpression;
}

Objects::Object* LiteralExpressionSyntax::get_object() const
{
    return _value;
}

SyntaxToken* LiteralExpressionSyntax::get_literal_token() const
{
    return _literal_token;
}