#include "code-analysis.h"

using namespace CodeAnalysis;

LiteralExpressionSyntax::LiteralExpressionSyntax(SyntaxToken* literal_token)
    : _literal_token(literal_token) 
{
    _children = {_literal_token};
}

SyntaxToken* LiteralExpressionSyntax::get_literal_token() const
{
    return _literal_token;
}

SyntaxKind LiteralExpressionSyntax::get_kind() const
{
    return SyntaxKind::NumberExpression;
}
