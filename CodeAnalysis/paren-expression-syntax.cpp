#include "code-analysis.h"

using namespace CodeAnalysis;

ParenExpressionSyntax::ParenExpressionSyntax(SyntaxToken* lparen_token, ExpressionSyntax* expression, SyntaxToken* rparen_token)
    : _lparen_token(lparen_token), _expression(expression), _rparen_token(rparen_token) 
{
    _children = {_lparen_token, _expression, _rparen_token};
}

SyntaxKind ParenExpressionSyntax::get_kind() const
{
    return SyntaxKind::ParenExpression;
}

ExpressionSyntax* ParenExpressionSyntax::get_expression() const
{
    return _expression;
}