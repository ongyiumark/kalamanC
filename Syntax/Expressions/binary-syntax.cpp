#include "syntax-expressions.h"

using namespace Syntax;

// This is for binary operations.
BinaryExpressionSyntax::BinaryExpressionSyntax(SyntaxNode* left, SyntaxToken op_token, SyntaxNode* right)
    : _left(left), _op_token(op_token), _right(right) {}

BinaryExpressionSyntax::~BinaryExpressionSyntax()
{
    delete _left;
    delete _right;
}

SyntaxKind BinaryExpressionSyntax::kind() const
{
    return SyntaxKind::BinaryExpression;
}

SyntaxNode* BinaryExpressionSyntax::get_left()
{
    return _left;
}

SyntaxToken* BinaryExpressionSyntax::get_op_token()
{
    return &_op_token;
}

SyntaxNode* BinaryExpressionSyntax::get_right()
{
    return _right;
}