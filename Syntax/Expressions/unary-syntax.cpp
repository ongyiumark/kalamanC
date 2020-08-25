#include "syntax-expressions.h"

using namespace Syntax;

using Diagnostics::Position;

// This is for unary operations.
UnaryExpressionSyntax::UnaryExpressionSyntax(SyntaxToken op_token, SyntaxNode* operand, Position pos)
    : SyntaxNode(pos), _op_token(op_token), _operand(operand) {}
    
UnaryExpressionSyntax::~UnaryExpressionSyntax()
{
    delete _operand;
}

SyntaxKind UnaryExpressionSyntax::kind() const
{
    return SyntaxKind::UnaryExpression;
}

SyntaxToken* UnaryExpressionSyntax::get_op_token()
{
    return &_op_token;
}

SyntaxNode* UnaryExpressionSyntax::get_operand()
{
    return _operand;
}