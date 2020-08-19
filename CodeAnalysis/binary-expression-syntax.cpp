#include "code-analysis.h"

using namespace CodeAnalysis;

BinaryExpressionSyntax::BinaryExpressionSyntax(ExpressionSyntax* left, SyntaxToken* op_token, ExpressionSyntax* right)
    : _left(left), _op_token(op_token), _right(right) 
{
    _children = {_left, _op_token, _right};
}

SyntaxKind BinaryExpressionSyntax::get_kind() const
{
    return SyntaxKind::BinaryExpression;
}

ExpressionSyntax* BinaryExpressionSyntax::get_left() const
{
    return _left;
}

SyntaxToken* BinaryExpressionSyntax::get_op_token() const
{
    return _op_token;
}

ExpressionSyntax* BinaryExpressionSyntax::get_right() const
{
    return _right;
}
