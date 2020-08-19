#include "syntax.h"

using namespace CodeAnalysis;

UnaryExpressionSyntax::UnaryExpressionSyntax(SyntaxToken* op_token, ExpressionSyntax* operand)
    :  _op_token(op_token), _operand(operand)
{
    _children = {_op_token, _operand};
}

SyntaxKind UnaryExpressionSyntax::get_kind() const
{
    return SyntaxKind::UnaryExpression;
}

SyntaxToken* UnaryExpressionSyntax::get_op_token() const
{
    return _op_token;
}

ExpressionSyntax* UnaryExpressionSyntax::get_operand() const
{
    return _operand;
}
