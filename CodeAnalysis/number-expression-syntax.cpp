#include "code-analysis.h"

using namespace CodeAnalysis;

NumberExpressionSyntax::NumberExpressionSyntax(SyntaxToken* number_token)
    : _number_token(number_token) 
{
    _children = {_number_token};
}

SyntaxToken* NumberExpressionSyntax::get_number_token() const
{
    return _number_token;
}

SyntaxKind NumberExpressionSyntax::get_kind() const
{
    return SyntaxKind::NumberExpression;
}
